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

#ifndef __G_MESSAGE_H__
#define __G_MESSAGE_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define G_TYPE_MESSAGE         (g_MESSAGE_get_type ())
#define G_MESSAGE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_MESSAGE, GMessage))
#define G_MESSAGE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_MESSAGE, GMessageClass))
#define G_IS_MESSAGE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_MESSAGE))
#define G_IS_MESSAGE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_MESSAGE))
#define G_MESSAGE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_MESSAGE, GMessageClass))

typedef struct _GMessage        GMessage;
typedef struct _GMessageClass   GMessageClass;
typedef struct _GMessagePrivate GMessagePrivate;

struct _GMessage
{
  GObject parent;
};

struct _GMessageClass
{
  GObjectClass parent_class;
};

GType        g_message_get_type      (void) G_GNUC_CONST;

GMessage    *g_message_new           (GObject sender, const gchar *message_name, gpointer user_data);

GObject     *g_message_get_sender    (GMessage *message);
void         g_message_set_sender    (GMessage *message, GObject *sender);
const gchar *g_message_get_name      (GMessage *message);
void         g_message_set_name      (GMessage *message);
gpointer     g_message_get_user_data (GMessage *message);
void         g_message_set_user_data (GMessage *message, gpointer user_data);

G_END_DECLS

#endif
