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

/**
 * SECTION:gmessage
 * @title: GMessage
 * @short_description: Represents a message
 */

struct _GMessagePrivate
{
  GObject *sender;
  gchar   *name;
};

G_DEFINE_TYPE (GMessage, g_message, G_TYPE_OBJECT);

static void
g_message_finalize (GObject *object)
{
  GMessage *message = G_MESSAGE (object);

  if (message->priv != NULL) {
    g_clear_object (message->priv->sender);
    g_clear_pointer (&message->priv->name, g_free);
  }

  G_OBJECT_CLASS (g_message_parent_class)->finalize (object);
}

static void
g_message_class_init (GMessageClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_message_finalize;
}

static void
g_message_init (GMessage *message)
{
  message->priv = g_message_get_instance_private (message);
}

/**
 * g_message_new:
 */
GMessage *
g_message_new (GObject *sender, const gchar *message_name)
{
  GMessage *message = g_object_new (G_TYPE_MESSAGE, NULL);

  g_message_set_sender (message, sender);
  g_message_set_name (message, name);
}

/**
 *g_message_get_sender:
 */
GObject *
g_message_get_sender (GMessage *message)
{
  g_return_val_if_fail (G_IS_MESSAGE (message), NULL);

  return message->priv->sender;
}

/**
 * g_message_set_sender:
 */
void
g_message_set_sender (GMessage *message, GObject *sender)
{
  g_return_if_fail (G_IS_MESSAGE (message));

  g_clear_object (&message->priv->sender);
  message->priv->sender = g_object_ref (sender);
}

/**
 *g_message_get_name:
 */
const gchar *
g_message_get_name (GMessage *message)
{
  g_return_val_if_fail (G_IS_MESSAGE (message), NULL);

  return (const gchar *) message->priv->name;
}

/**
 * g_message_set_name:
 */
void
g_message_set_name (GMessage *message, const gchar *name)
{
  g_return_if_fail (G_IS_MESSAGE (message));

  g_clear_pointer (&message->priv->name, g_free);
  message->priv->name = g_strdup (name);
}
