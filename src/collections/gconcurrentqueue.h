/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/* GIO - GLib Input, Output and Streaming Library
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

#ifndef __G_CONCURRENT_QUEUE_H__
#define __G_CONCURRENT_QUEUE_H__

#include <gio/gio.h>

G_BEGIN_DECLS

#define G_TYPE_CONCURRENT_QUEUE                              (g_CONCURRENT_QUEUE_get_type ())
#define G_CONCURRENT_QUEUE(inst)                             (G_TYPE_CHECK_INSTANCE_CAST ((inst), G_TYPE_CONCURRENT_QUEUE, GConcurrentQueue))
#define G_CONCURRENT_QUEUE_CLASS(class)                      (G_TYPE_CHECK_CLASS_CAST ((class), G_TYPE_CONCURRENT_QUEUE, GConcurrentQueueClass))
#define G_IS_CONCURRENT_QUEUE(inst)                          (G_TYPE_CHECK_INSTANCE_TYPE ((inst), G_TYPE_CONCURRENT_QUEUE))
#define G_IS_CONCURRENT_QUEUE_CLASS(class)                   (G_TYPE_CHECK_CLASS_TYPE ((class), G_TYPE_CONCURRENT_QUEUE))
#define G_CONCURRENT_QUEUE_GET_CLASS(inst)                   (G_TYPE_INSTANCE_GET_CLASS ((inst), G_TYPE_CONCURRENT_QUEUE, GConcurrentQueueClass))

typedef struct _GConcurrentQueue                             GConcurrentQueue;
typedef struct _GConcurrentQueuePrivate                      GConcurrentQueuePrivate;
typedef struct _GConcurrentQueueClass                        GConcurrentQueueClass;

struct _GConcurrentQueueClass
{
  GObjectClass parent_class;
};

struct _GConcurrentQueue
{
  GObject parent_instance;
  GConcurrentQueuePrivate *priv;
};

GLIB_AVAILABLE_IN_ALL
GType             g_concurrent_queue_get_type (void);

GLIB_AVAILABLE_IN_ALL
GConcurrentQueue *g_concurrent_queue_new      (void);

GLIB_AVAILABLE_IN_ALL
void              g_concurrent_queue_push     (GConcurrentQueue *queue, GObject *item);

GLIB_AVAILABLE_IN_ALL
GObject          *g_concurrent_queue_pull     (GConcurrentQueue *queue);

G_END_DECLS

#endif /* __G_CONCURRENT_QUEUE_H__ */
