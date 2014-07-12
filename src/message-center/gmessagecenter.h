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

#ifndef __G_MESSAGE_CENTER_H__
#define __G_MESSAGE_CENTER_H__

#include "gmessage.h"

G_BEGIN_DECLS

#define G_TYPE_MESSAGE_CENTER         (g_message_center_get_type ())
#define G_MESSAGE_CENTER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_MESSAGE_CENTER, GMessageCenter))
#define G_MESSAGE_CENTER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_MESSAGE_CENTER, GMessageCenterClass))
#define G_IS_MESSAGE_CENTER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_MESSAGE_CENTER))
#define G_IS_MESSAGE_CENTER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_MESSAGE_CENTER))
#define G_MESSAGE_CENTER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_MESSAGE_CENTER, GMessageCenterClass))

typedef struct _GMessageCenter        GMessageCenter;
typedef struct _GMessageCenterClass   GMessageCenterClass;
typedef struct _GMessageCenterPrivate GMessageCenterPrivate;

/**
 * GMessageCenter;
 *
 * An application-wide message center that allows different parts of the
 * application to easily communicate with each other without having to know
 * any details about the other parts.
 */
struct _GMessageCenter
{
  GObject parent_instance;

  /*< private >*/
  GMessageCenterPrivate *priv;
};

struct _GMessageCenterClass
{
  GObjectClass parent_class;

  void (* message_received) (GMessageCenter message_center, GMessage *message);
};

GType           g_message_center_get_type (void) G_GNUC_CONST;

GMessageCenter *g_message_center_get      (void);
void            g_message_center_send     (GMessageCenter *message_center, GMessage *message);

G_END_DECLS

#endif
