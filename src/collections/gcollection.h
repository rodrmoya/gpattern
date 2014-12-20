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

#ifndef __G_COLLECTION_H__
#define __G_COLLECTION_H__

#if !defined (__GIO_GIO_H_INSIDE__) && !defined (GIO_COMPILATION)
#error "Only <gio/gio.h> can be included directly."
#endif

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_COLLECTION            (g_collection_get_type ())
#define G_COLLECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_COLLECTION, GCollection))
#define G_IS_COLLECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_COLLECTION))
#define G_COLLECTION_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), G_TYPE_COLLECTION, GCollectionIface))

/**
 * GCollectionIface:
 * @g_iface: The parent interface.
 * @item_added: signal that is emitted when an item is added to the collection.
 * @item_removed: signal that is emitted when an item is removed from the collection.
 * @add: method to add items to the collection.
 * @remove: method to remove items from the collection
 * @get_item: method to retrieve items by index
 */
typedef struct _GCollectionIface GCollectionIface;

struct _GCollectionIface
{
  GTypeInterface g_iface;

  /* signals */
  void (* item_added)    (GCollection *collection, GObject *item);
  void (* item_removed)  (GCollection *collection, GObject *item);

  /* virtual table */
  gboolean (* add)       (GCollection *collection, GObject *item);
  gboolean (* remove)    (GCollection *collection, GObject *item);
  GObject * (* get_item) (GCollection *collection, gpointer index);
};

GLIB_AVAILABLE_IN_ALL
GType    g_collection_get_type (void) G_GNUC_CONST;

GLIB_AVAILABLE_IN_ALL
gboolean g_collection_add      (GCollection *collection, GObject *item);

GLIB_AVAILABLE_IN_ALL
gboolean g_collection_remove   (GCollection *collection, GObject *item);

GLIB_AVAILABLE_IN_ALL
GObject *g_collection_get_item (GCollection *collection, gpointer index);

G_END_DECLS

#endif /* __G_COLLECTION_H__ */
