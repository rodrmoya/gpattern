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

#include "config.h"
#include "gconcurrentqueue.h"

/**
 * SECTION:gconcurrentqueue
 * @short_description: Concurrent queue implementation.
 * @include: gio/gio.h
 *
 * The #GConcurrentQueue implements a concurrent FIFO queue, which apart from being
 * thread-safe, notifies of changes concurrently as described by the #GCollection
 * interface.
 */

struct _GConcurrentQueuePrivate
{
  GMutex mutex;
  GSList *items;
};

static void g_concurrent_queue_collection_interface_init (GCollectionIface *iface);

G_DEFINE_TYPE_WITH_CODE (GConcurrentQueue, g_concurrent_queue, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_COLLECTION, g_concurrent_queue_collection_interface_init))

static void
g_concurrent_queue_finalize (GObject *object)
{
  GConcurrentQueue *queue = G_CONCURRENT_QUEUE (object);

  g_mutex_lock (&queue->priv->mutex);

  while (queue->priv->items != NULL)
    {
      GObject *item = G_OBJECT (queue->priv->items->data);
      queue->priv->items = g_slist_remove (item);
      g_object_unref (item);
    }

  g_mutex_unlock (&queue->priv->mutex);

  g_free (queue->priv);

  G_OBJECT_CLASS (g_concurrent_queue_parent_class)->finalize (object);
}

static void
g_concurrent_queue_class_init (GConcurrentQueueClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_concurrent_queue_finalize;
}

static void
g_concurrent_queue_init (GConcurrentQueue *queue)
{
  queue->priv = g_new0 (GConcurrentQueuePrivate, 1);
}

static gboolean
_collection_add (GCollection *collection, GObject *item)
{
  g_concurrent_queue_push (G_CONCURRENT_QUEUE (collection), item);
  return TRUE;
}

static gboolean
_collection_remove (GCollection *collection, GObject *item)
{
  GConcurrentQueue *queue = G_CONCURRENT_QUEUE (collection);

  g_return_val_if_fail (G_IS_CONCURRENT_QUEUE (queue), FALSE);

  g_mutex_lock (&queue->priv->mutex);
  queue->priv->items = g_slist_remove (item);
  g_mutex_unlock (&queue->priv->mutex);

  return TRUE;
}

static GObject *
_collection_get_item (GCollection *collection, gpointer index)
{
  g_return_val_if_fail (G_IS_CONCURRENT_QUEUE (collection), NULL);
  g_return_val_if_fail (GPOINTER_TO_INT (index) == 0, NULL);

  return g_concurrent_queue_pull (G_CONCURRENT_QUEUE (collection));
}

static void
g_concurrent_queue_collection_interface_init (GCollectionIface *iface)
{
  iface->add = _collection_add;
  iface->remove = _collection_remove;
  iface->get_item = _collection_get_item;
}

/**
 * g_concurrent_queue_new:
 *
 * Create a new #GConcurrentQueue instance.
 */
GConcurrentQueue *
g_concurrent_queue_new (void)
{
  return g_object_new (G_TYPE_CONCURRENT_QUEUE, NULL);
}

/**
 * g_concurrent_queue_push:
 * @queue: a #GConcurrentQueue
 * @item: object to be queued
 *
 * Queues a new item on the given #GConcurrentQueue, that will be added to the
 * end of the queue. The @item will be referenced, so after calling this function
 * you should unref it if no longer needed.
 */
void
g_concurrent_queue_push (GConcurrentQueue *queue, GObject *item)
{
  g_return_if_fail (G_IS_CONCURRENT_QUEUE (queue));
  g_return_if_fail (G_IS_OBJECT (item));

  g_mutex_lock (&queue->priv->mutex);

  queue->priv->items = g_slist_append (queue->priv->items, g_object_ref (item));
  g_signal_emit_by_name (queue, "item_added", item);

  g_mutex_unlock (&queue->priv->mutex);
}

/**
 * g_concurrent_queue_pull:
 * @queue: a #GConcurrentQueue
 *
 * Dequeues an item (ie, returns the oldest one) from the given queue.
 *
 * Returns: the oldest item, which should be unrefed by the caller when
 * no longer needed, or NULL if there were no items in the queue.
 */
GObject *
g_concurrent_queue_pull (GConcurrentQueue *queue)
{
  GObject *result = NULL;

  g_return_val_if_fail (G_IS_CONCURRENT_QUEUE (queue), NULL);

  g_mutex_lock (&queue->priv->mutex);

  result = queue->priv->items->data;
  queue->priv->items = g_slist_remove (result);
  g_signal_emit_by_name (queue, "item_removed", item);

  g_mutex_unlock (&queue->priv->mutex);
}