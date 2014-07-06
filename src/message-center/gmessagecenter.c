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
 * SECTION:gmessagecenter
 * @title: GMessageCenter
 * @short_description: Application-wide message passing interface
 */

struct _GMessageCenterPrivate
{
};

enum {
  MESSAGE_RECEIVED_SIGNAL,
  LAST_SIGNAL
};
static guint message_center_signals [LAST_SIGNAL] ;

G_DEFINE_TYPE (GMessageCenter, g_message_center, G_TYPE_OBJECT)

static void
g_message_center_finalize (GObject *object)
{
  GMessageCenter *message_center = G_MESSAGE_CENTER (object);

  if (message_center->priv != NULL) {
  }

  G_OBJECT_CLASS (g_message_center_parent_class)->finalize (object);
}

static void
g_message_center_class_init (GMessageCenterClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_message_center_finalize;

  message_center_signals[MESSAGE_RECEIVED_SIGNAL] =
		g_signal_new ("message-received",
                      G_OBJECT_CLASS_TYPE (object_class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GMessageCenterClass, message_received),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__OBJECT_STRING,
                      G_TYPE_NONE, 2,
                      G_TYPE_OBJECT,
                      G_TYPE_STRING);
}

static void
g_message_center_init (GMessageCenter *message_center)
{
  message_center->priv = g_message_center_get_instance_private (message_center);
}

/**
 * g_message_center_get:
 */
GMessageCenter *
g_message_center_get (void)
{
  static GMessageCenter *message_center = NULL;

  if (message_center == NULL) {
    message_center = g_object_new (G_TYPE_MESSAGE_CENTER, NULL);
  }

  return message_center;
}

/**
 * g_message_center_send:
 */
void
g_message_center_send (GMessageCenter *message_center, GObject sender, const gchar *message_name)
{
  g_return_if_fail (G_IS_MESSAGE_CENTER (message_center));
  g_return_if_fail (message_name != NULL);

  g_signal_emit_by_name (message_center, "message-received", sender, message_name);
}
