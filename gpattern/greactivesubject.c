/* GPattern - GLib software patterns implementation library
 *
 * Copyright (C) 2013 Rodrigo Moya
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Rodrigo Moya <rodrigo@gnome.org>
 */

#include "config.h"

#include "gobservable.h"
#include "gobserver.h"
#include "greactivesubject.h"

/**
 * SECTION:greactivesubject
 * @title: GReactiveSubject
 * @short_description: Represents an object that is both an observable
 * sequence as well as an observer.
 *
 * GReactiveSubject represents an object that is both an observable as
 * well as an observer. This is due to the fact that implementing a
 * #GObservable involves adding data, setting errors and notifying
 * observers when the sequence of data is finished, which is very similar
 * to the #GObserver interface's on_next, on_error and on_completed methods.
 * #GReactiveSubject is thus used for easily creating objects that
 * implement the #GObservable interface.
 *
 * By default (when using #g_reactive_subject_new for creating the
 * #GReactiveSubject instance), it will store all data added to it for its
 * full lifecycle, and only notify observers of data added after they have
 * subscribed.
 *
 * This default behavior can be changed with the following object properties
 * (all of which can be set at creation time by using #g_reactive_subject_new_full
 * instead of #g_reactive_subject_new):
 *
 * "cache-size": this specifies the number of data items to keep. As data
 * is added to the sequence, older items will be removed if the cache
 * size is exceeded.
 *
 * "replay": when set to TRUE, observers will get older data (data that
 * was added to the sequence before an observer subscribed to it) as soon
 * as they subscribe to the sequence.
 *
 * "async": when set to true, data added to the sequence will be sent to
 * the observers only before the sequence is about to be completed, not
 * when the data is being added.
 */

static void g_reactive_subject_observable_iface_init (GObservableInterface *iface);
static void g_reactive_subject_observer_iface_init (GObserverInterface *iface);

struct _GReactiveSubjectPrivate
{
  GList *items;
  GHashTable *observers;
  gboolean replay;
  gboolean async;
  guint cache_size;
  gboolean completed;
};

G_DEFINE_TYPE_WITH_CODE (GReactiveSubject, g_reactive_subject, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (GReactiveSubject)
                         G_IMPLEMENT_INTERFACE (G_TYPE_OBSERVABLE,
                                                g_reactive_subject_observable_iface_init)
                         G_IMPLEMENT_INTERFACE (G_TYPE_OBSERVER,
                                                g_reactive_subject_observer_iface_init))

enum {
  PROP_0,
  PROP_REPLAY,
  PROP_ASYNC,
  PROP_CACHE_SIZE,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

/* GObservable interface implementation */
static void
g_reactive_subject_subscribe (GObservable *observable, GObserver *observer)
{
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (observable);

  g_return_if_fail (G_IS_REACTIVE_SUBJECT (observable));
  g_return_if_fail (G_IS_OBSERVER (observer));

  /* Check if this observer is already subscribed */
  if (!g_hash_table_lookup (subject->priv->observers, observer))
    {
      g_hash_table_insert (subject->priv->observers, observer, observer);

      /* if set to replay, send all cached data to this new observer */
      if (subject->priv->replay)
        {
          GList *l;

          for (l = subject->priv->items; l != NULL; l = l->next)
            {
              g_observer_on_next (observer, l->data);
            }
        }
    }
}

static void
g_reactive_subject_unsubscribe (GObservable *observable, GObserver *observer)
{
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (observable);
  
  g_return_if_fail (G_IS_REACTIVE_SUBJECT (observable));
  g_return_if_fail (G_IS_OBSERVER (observer));

  g_hash_table_remove (subject->priv->observers, observer);
}

static void
g_reactive_subject_observable_iface_init (GObservableInterface *iface)
{
  iface->subscribe = g_reactive_subject_subscribe;
  iface->unsubscribe = g_reactive_subject_unsubscribe;
}

/* GObserver interface implementation */
static void
g_reactive_subject_on_next (GObserver *observer, GVariant *value)
{
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (observer);

  g_return_if_fail (G_IS_REACTIVE_SUBJECT (observer));
  g_return_if_fail (!subject->priv->completed);

  /* Make sure we don't exceed the cache size */
  if (subject->priv->cache_size > 0)
    {
      while (g_list_length (subject->priv->items) >= subject->priv->cache_size)
        {
          GVariant *v = (GVariant *) subject->priv->items->data;

          subject->priv->items = g_list_remove (subject->priv->items, v);
          g_variant_unref (v);
        }
    }

  /* Store this item in our list of items */
  subject->priv->items = g_list_append (subject->priv->items, g_variant_ref (value));

  /* Notify observers */
  if (!subject->priv->async)
    {
      g_hash_table_iter_init (&iter, subject->priv->observers);
      while (g_hash_table_iter_next (&iter, &key, &value))
        {
          g_observer_on_next (G_OBSERVER (key), value);
        }
    }
}

static void
g_reactive_subject_on_error (GObserver *observer, GError *error)
{
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (observer);

  g_return_if_fail (G_IS_REACTIVE_SUBJECT (observer));
  g_return_if_fail (!subject->priv->completed);

  subject->priv->completed = TRUE;

  g_hash_table_iter_init (&iter, subject->priv->observers);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      g_observer_on_error (G_OBSERVER (key), error);
    }
}

static void
g_reactive_subject_on_completed (GObserver *observer)
{
  GHashTableIter iter;
  gpointer key, value;
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (observer);

  g_return_if_fail (G_IS_REACTIVE_SUBJECT (observer));
  g_return_if_fail (!subject->priv->completed);

  subject->priv->completed = TRUE;

  g_hash_table_iter_init (&iter, subject->priv->observers);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      if (subject->priv->async)
        {
          GList *l;

          /* if in async mode, send data just before completing */
          for (l = subject->priv->items; l != NULL; l = l->next)
            {
              g_observer_on_next (G_OBSERVER (key), l->data);
            }
        }

      g_observer_on_completed (G_OBSERVER (key));
    }
}

static void
g_reactive_subject_observer_iface_init (GObserverInterface *iface)
{
  iface->on_next = g_reactive_subject_on_next;
  iface->on_error = g_reactive_subject_on_error;
  iface->on_completed = g_reactive_subject_on_completed;
}

static void
g_reactive_subject_get_property (GObject *object,
                                 guint property_id,
                                 GValue *value,
                                 GParamSpec *pspec)
{
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (object);

  switch (property_id)
    {
    case PROP_REPLAY:
      g_value_set_boolean (value, subject->priv->replay);
      break;
    case PROP_ASYNC:
      g_value_set_boolean (value, subject->priv->async);
      break;
    case PROP_CACHE_SIZE:
      g_value_set_uint (value, subject->priv->cache_size);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
g_reactive_subject_set_property (GObject *object,
                                 guint property_id,
                                 const GValue *value,
                                 GParamSpec *pspec)
{
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (object);
  
  switch (property_id)
    {
    case PROP_REPLAY:
      subject->priv->replay = g_value_get_boolean (value);
      break;
    case PROP_REPLAY:
      subject->priv->async = g_value_get_boolean (value);
      break;
    case PROP_CACHE_SIZE:
      subject->priv->cache_size = g_value_get_uint (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
g_reactive_subject_finalize (GObject *object)
{
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (object);

  if (subject->priv)
    {
      if (!subject->priv->completed)
        {
          g_reactive_object_on_completed (G_OBSERVER (subject));
        }

      while (subject->priv->items != NULL)
        {
          GVariant *value = (GVariant *) subject->priv->items->data;

          subject->priv->items = g_list_remove (subject->priv->items, value);
          g_variant_unref (value);
        }

      if (subject->priv->observers != NULL)
        {
          g_clear_pointer (&subject->priv->observers, g_hash_table_unref);
        }
    }

  G_OBJECT_CLASS (g_reactive_subject_parent_class)->finalize (object);
}

static void
g_reactive_subject_class_init (GReactiveSubjectClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = g_reactive_subject_get_property;
  object_class->set_property = g_reactive_subject_set_property;
  object_class->finalize = g_reactive_subject_finalize;
  
  obj_properties[PROP_REPLAY] =
    g_param_spec_bool ("replay",
                       "Whether to replay all data to new subscribers",
                       "Whether to replay all data to new subscribers",
                       FALSE  /* default value */,
                       G_PARAM_READWRITE);
  obj_properties[PROP_ASYNC] =
    g_param_spec_bool ("async",
                       "Async mode",
                       "Whether to work in async mode (and send data to observers just before completing)",
                       FALSE  /* default value */,
                       G_PARAM_READWRITE);
  obj_properties[PROP_CACHE_SIZE] =
    g_param_spec_uint ("cache-size",
                       "Cache size",
                       "Number of items to cache",
                       0,
                       0,
                       0,
                       G_PARAM_READWRITE);
}

static void
g_reactive_subject_init (GReactiveSubject *subject)
{
  subject->priv = g_reactive_subject_get_instance_private (subject);

  subject->priv->items = NULL;
  subject->priv->observers = g_hash_table_new (g_direct_hash, g_direct_equal);
  subject->priv->replay = FALSE;
  subject->priv->async = FALSE;
  subject->priv->cache_size = 0;
  subject->priv->completed = FALSE;
}

/**
 * g_reactive_subject_new:
 *
 * Creates a new #GReactiveObject, which is an object that represents both
 * a #GObservable and a #GObserver. This is mainly to help in the implementation
 * of observables and observers.
 *
 * Return value: a new #GReactiveSubject
 */
GReactiveSubject *
g_reactive_subject_new (void)
{
  return g_object_new (G_TYPE_REACTIVE_SUBJECT, NULL);
}

/**
 * g_reactive_subject_new_full:
 * @replay: whether cached data should be sent to observers when they subscribe.
 * @async: whether to work on async mode and just send data to observers when
 * completing the sequence
 * @cache_size: number of items to cache (0 means keep all)
 *
 * Creates a new #GReactiveObject with the possibility of setting a different value
 * than the default for all the object's properties that affect its behavior.
 *
 * Return value: a new #GReactiveSubject
 */
GReactiveSubject *
g_reactive_subject_new_full (gboolean replay,
                             gboolean async,
                             guint cache_size)
{
  return g_object_new (G_TYPE_REACTIVE_SUBJECT,
                       "replay", replay,
                       "async", async,
                       "cache-size", cache_size,
                       NULL)
}
