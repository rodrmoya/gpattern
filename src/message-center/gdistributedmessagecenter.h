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

#ifndef __G_DISTRIBUTED_MESSAGE_CENTER_H__
#define __G_DISTRIBUTED_MESSAGE_CENTER_H__

#include "gmessagecenter.h"

G_BEGIN_DECLS

#define G_TYPE_DISTRIBUTED_MESSAGE_CENTER         (g_message_center_get_type ())
#define G_DISTRIBUTED_MESSAGE_CENTER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_DISTRIBUTED_MESSAGE_CENTER, GDistributedMessageCenter))
#define G_DISTRIBUTED_MESSAGE_CENTER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_DISTRIBUTED_MESSAGE_CENTER, GDistributedMessageCenterClass))
#define G_IS_DISTRIBUTED_MESSAGE_CENTER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_DISTRIBUTED_MESSAGE_CENTER))
#define G_IS_DISTRIBUTED_MESSAGE_CENTER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_DISTRIBUTED_MESSAGE_CENTER))
#define G_DISTRIBUTED_MESSAGE_CENTER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_DISTRIBUTED_MESSAGE_CENTER, GDistributedMessageCenterClass))

typedef struct _GDistributedMessageCenter        GDistributedMessageCenter;
typedef struct _GDistributedMessageCenterClass   GDistributedMessageCenterClass;
typedef struct _GDistributedMessageCenterPrivate GDistributedMessageCenterPrivate;

/**
 * GDistributedMessageCenter;
 *
 * A system-wide message center that allows different applications
 * to easily communicate with each other without having to know
 * any details about the other parts.
 */

 struct _GDistributedMessageCenter
 {
   GMessageCenter parent;

   /*< private >*/
   GDistributedMessageCenterPrivate *priv;
 };

 struct _GDistributedMessageCenterClass
 {
   GMessageCenterClass parent_class;
 };

GType                      g_distributed_message_center_get_type (void) G_GNUC_CONST;
GDistributedMessageCenter *g_distributed_message_center_get      (void);

void                       g_distributed_message_center_send     (GDistributedMessageCenter *message_center,
                                                                  GMessage *message);

G_END_DECLS

#endif
