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
#include "gcollection.h"

/**
 * SECTION:GCollection
 * @short_description: A collection that notifies of changes.
 * @include: gio/gio.h
 *
 * The #GCollection interface represents a collection of items that emits notifications
 * of any change to the collection.
 */

typedef GCollectionIface GCollectionInterface;

static void
g_COLLECTION_default_init (GCollectionInterface *iface)
{
  /**
   * GCollection::item_added:
   *
   * Emitted when a new item is added to the collection.
   */
  g_signal_new (I_("item_added"),
                G_TYPE_COLLECTION,
                G_SIGNAL_RUN_LAST,
                G_STRUCT_OFFSET (GCollectionIface, item_added),
                NULL, NULL, NULL,
                G_TYPE_NONE, 1,
                G_TYPE_OBJECT);
  
  /**
   * GCollection::item_removed:
   *
   * Emitted when an item is removed from the collection.
   */
  g_signal_new (I_("item_removed"),
                G_TYPE_COLLECTION,
                G_SIGNAL_RUN_LAST,
                G_STRUCT_OFFSET (GCollectionIface, item_removed),
                NULL, NULL, NULL,
                G_TYPE_NONE, 1,
                G_TYPE_OBJECT);
}

/**
 * g_collection_add:
 * @collection: a #GCollection
 * @item: item to be added
 *
 * Adds an item to the collection.
 */
void
g_collection_add (GCollection *collection, GObject *item)
{
  GCollectionIface *iface;

  g_return_if_fail (G_IS_COLLECTION (collection));

  iface = G_COLLECTION_GET_IFACE (collection);

  return (* iface->add) (collection, item);
}

/**
 * g_collection_remove:
 * @collection: a #GCollection
 * @item: item to be removed
 *
 * Removes an item from the collection.
 */
void
g_collection_remove (GCollection *collection, GObject *item)
{
  GCollectionIface *iface;

  g_return_if_fail (G_IS_COLLECTION (collection));

  iface = G_COLLECTION_GET_IFACE (collection);

  return (* iface->remove) (collection, item);
}

/**
 * g_collection_get_item:
 * @collection: a #GCollection
 * @index: index of the item to retrieve
 *
 * Retrieve an item from the collection. To do so, an @index is used, which, depending
 * on the #GCollection implementation used, would be an integer (for array-based
 * collections), a string (dictionaries), ...
 *
 * Returns: the item associated with the given @index if found, NULL otherwise. The returned
 * object belongs to the collection, so #g_object_ref it if you need to keep it around.
 */
GObject *
g_collection_get_item (GCollection *collection, gpointer index)
{
  GCollectionIface *iface;

  g_return_val_if_fail (G_IS_COLLECTION (collection), NULL);
  g_return_val_if_fail (index != NULL, NULL);

  iface = G_COLLECTION_GET_IFACE (collection);

  return (* iface->get_item) (collection, index);
}