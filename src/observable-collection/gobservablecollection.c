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

#include "gobservablecollection.h"

/**
 * SECTION:gobservablecollection
 * @title: GObservableCollection
 * @short_description: Represents a collection of items that emits notifications
 * of any change to the collection.
 */

struct _GObservableCollectionPrivate
{
  GSList *items;
};

G_DEFINE_TYPE (GObservableCollection, g_observable_collection, G_TYPE_REACTIVE_SUBJECT)

static void
g_observable_collection_finalize (GObject *object)
{
  GObservableCollection *collection = G_OBSERVABLE_COLLECTION (object);

  if (collection-priv)
    {
      g_list_free (collection->priv->items);
    }

  G_OBJECT_CLASS (g_observable_collection_parent_class)->finalize (object);
}

static void
g_observable_collection_class_init (GObservableCollectionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_observable_collection_finalize;
}

static void
g_observable_collection_init (GObservableCollection *collection)
{
  collection->priv = g_observable_collection_get_instance_private (collection);
  collection->priv->items = NULL;
}

/**
 * g_observable_collection_new:
 */
GObservableCollection
g_observable_collection_new (void)
{
  return g_object_new (G_TYPE_OBSERVABLE_COLLECTION, NULL);
}

/**
 * g_observable_collection_append:
 */
void
g_observable_collection_append (GObservableCollection *collection, gpointer item)
{
  g_return_if_fail (G_IS_OBSERVABLE_COLLECTION (collection));

  collection->priv->items = g_slist_append (collection->priv->items, item);
}

/**
 * g_observable_collection_append:
 */
void
g_observable_collection_prepend (GObservableCollection *collection, gpointer item)
{
  g_return_if_fail (G_IS_OBSERVABLE_COLLECTION (collection));

  collection->priv->items = g_slist_prepend (collection->priv->items, item);
}

/**
 * g_observable_collection_insert:
 */
void
g_observable_collection_insert (GObservableCollection *collection, gpointer item, gint position)
{
  g_return_if_fail (G_IS_OBSERVABLE_COLLECTION (collection));

  collection->priv->items = g_slist_insert (collection->priv->items, item, position);
}

/**
 * g_observable_collection_insert_sorted:
*/
void
g_observable_collection_insert_sorted (GObservableCollection *collection, gpointer item, GCompareFunc compare_func)
{
  g_return_if_fail (G_IS_OBSERVABLE_COLLECTION (collection));

  collection->priv->items = g_slist_insert_sorted (collection->priv->items, item, compare_func);
}

/**
 * g_observable_collection_remove:
 */
void
g_observable_collection_remove (GObservableCollection *collection, gpointer item)
{
  g_return_if_fail (G_IS_OBSERVABLE_COLLECTION (collection));

  collection->priv->items = g_slist_remove (collection->priv->items, item);
}

/**
 * g_observable_collection_reverse:
 */
void
g_observable_collection_reverse (GObservableCollection *collection)
{
  g_return_if_fail (G_IS_OBSERVABLE_COLLECTION (collection));

  collection->priv->items = g_slist_reverse (collection->priv->items);
}
