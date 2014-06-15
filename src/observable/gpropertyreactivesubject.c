/* GPattern - GLib software patterns implementation library
 *
 * Copyright (C) 2014 Rodrigo Moya
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

#include "gobserver.h"
#include "greactivesubject.h"

#define G_TYPE_PROPERTY_REACTIVE_SUBJECT         (g_property_reactive_subject_get_type ())
#define G_PROPERTY_REACTIVE_SUBJECT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_PROPERTY_REACTIVE_SUBJECT, GPropertyReactiveSubject))
#define G_PROPERTY_REACTIVE_SUBJECT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_PROPERTY_REACTIVE_SUBJECT, GPropertyReactiveSubjectClass))
#define G_IS_PROPERTY_REACTIVE_SUBJECT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_PROPERTY_REACTIVE_SUBJECT))
#define G_IS_PROPERTY_REACTIVE_SUBJECT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_PROPERTY_REACTIVE_SUBJECT))
#define G_PROPERTY_REACTIVE_SUBJECT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_PROPERTY_REACTIVE_SUBJECT, GPropertyReactiveSubjectClass))

typedef struct {
  GReactiveSubject parent;
  
  GObject *object_watched;
  gchar *property_name;
} GPropertyReactiveSubject;

typedef struct {
  GReactiveSubjectClass parent_class;
} GPropertyReactiveSubjectClass;

G_DEFINE_TYPE(GPropertyReactiveSubject, g_property_reactive_subject, G_TYPE_REACTIVE_SUBJECT)

static void
g_property_reactive_subject_finalize (GObject *object)
{
  GPropertyReactiveSubject *self = G_REACTIVE_SUBJECT (object);

  g_return_if_fail (G_IS_PROPERTY_REACTIVE_SUBJECT (self));

  G_OBJECT_CLASS (g_property_reactive_subject_parent_class)->finalize (object);
}

static void
g_property_reactive_subject_class_init (GPropertyReactiveSubjectClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_property_reactive_subject_finalize;
}

static void
g_property_reactive_subject_init (GPropertyReactiveSubject *self)
{
  self->object_watched = NULL;
}

static void
property_changed_cb (GObject *object, GParamSpec *pspec, gpointer user_data)
{
  GValue *value;
  GPropertyReactiveSubject *self = G_PROPERTY_REACTIVE_SUBJECT (user_data);

  g_return_if_fail (G_IS_PROPERTY_REACTIVE_SUBJECT (self));

  g_value_init (&value, pspec->value_type);
  g_object_get_property (object, self->property_name, &value);
  g_observer_on_next (G_OBSERVER (self), &value);
}

/**
 * g_reactive_subject_new_for_property:
 * @object: the object to watch
 * @property_name: name of the property to watch
 *
 * Creates a new #GReactiveSubject for a given object's property. This
 * will setup the reactive subject to listen for changes in the property
 * and send push notifications to all observers whenever the property
 * value changes.
 *
 * Return value: a new #GReactiveSubject
 */
GReactiveSubject *
g_reactive_subject_new_for_property (GObject *object, const gchar *property_name)
{
  GReactiveSubject *self;
  GValue value;
  gchar *signal_name;

  g_return_val_if_fail (G_IS_OBJECT (object), NULL);
  g_return_val_if_fail (property_name != NULL, NULL);

  self = g_object_new (G_TYPE_PROPERTY_REACTIVE_SUBJECT,
                       "replay", TRUE,
                       "async", FALSE,
                       "cache-size", 1,
                       NULL);

  /* FIXME: add a weak ref to the object to complete the sequence when it goes away */
  self->object_watched = object;
  self->property_name = g_strdup (property_name);

  /* FIXME: need to initialize 'value' with the correct type */
  /* Get the current value and store that in the cache for future observers */
  g_object_get_property (object, self->property_name, &value);
  g_observer_on_next (G_OBSERVER (self), &value);

  signal_name = g_strdup_printf ("notify::%s", self->property_name);
  g_signal_connect (self->object_watched, signal_name,
                    G_CALLBACK (property_changed_cb), self);
  g_free (signal_name);

  return self;
}
