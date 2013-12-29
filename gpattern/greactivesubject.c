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
 */

static void g_reactive_subject_observable_iface_init (GObservableInterface *iface);
static void g_reactive_subject_observer_iface_init (GObserverInterface *iface);

struct _GReactiveSubjectPrivate
{
  GList *items;
  GHashTable *observers;
  gboolean completed;
};

G_DEFINE_TYPE_WITH_CODE (GReactiveSubject, g_reactive_subject, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (GReactiveSubject)
                         G_IMPLEMENT_INTERFACE (G_TYPE_OBSERVABLE, g_reactive_subject_observable_iface_init)
                         G_IMPLEMENT_INTERFACE (G_TYPE_OBSERVER, g_reactive_subject_observer_iface_init))

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
}

static void
g_reactive_subject_on_error (GObserver *observer, GError *error)
{
}

static void
g_reactive_subject_on_completed (GObserver *observer)
{
}

static void
g_reactive_subject_observer_iface_init (GObserverInterface *iface)
{
  iface->on_next = g_reactive_subject_on_next;
  iface->on_error = g_reactive_subject_on_error;
  iface->on_completed = g_reactive_subject_on_completed;
}

static void
g_reactive_subject_finalize (GObject *object)
{
  GReactiveSubject *subject = G_REACTIVE_SUBJECT (object);

  if (subject->priv)
    {
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

  object_class->finalize = g_reactive_subject_finalize;
}

static void
g_reactive_subject_init (GReactiveSubject *subject)
{
  subject->priv = g_reactive_subject_get_instance_private (subject);

  subject->priv->items = NULL;
  subject->priv->observers = g_hash_table_new (g_direct_hash, g_direct_equal);
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
