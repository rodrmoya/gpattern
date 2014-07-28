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

struct _GDistributedMessageCenterPrivate
{
};

G_DEFINE_TYPE (GDistributedMessageCenter, g_distributed_message_center, G_TYPE_MESSAGE_CENTER);

static void
g_distributed_message_center_finalize (GObject *object)
{
  G_OBJECT_CLASS (g_distributed_message_center_parent_class)->finalize (object);
}

static void
g_distributed_message_center_class_init (GDistributedMessageCenterClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_distributed_message_center_finalize;
}

static void
g_distributed_message_center_init (GDistributedMessageCenter *message_center)
{
  message_center->priv = g_distributed_message_center_get_instance_private (message_center);
}

/**
 * g_distributed_message_center_get:
 */
GDistributedMessageCenter *
g_distributed_message_center_get ()
{
  static GDistributedMessageCenter *message_center = NULL;

  if (message_center == NULL) {
    message_center = g_object_new (G_TYPE_DISTRIBUTED_MESSAGE_CENTER, NULL);
  }

  return message_center;
}

/**
 * g_distributed_message_center_send:
 */
void
g_distributed_message_center_send (GDistributedMessageCenter *message_center,
                                   GMessage *message)
{
  g_return_if_fail (G_IS_DISTRIBUTED_MESSAGE_CENTER (message_center));
}
