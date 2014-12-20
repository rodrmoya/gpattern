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
#include "gconcurrentdictionary.h"

/**
 * SECTION:gconcurrentdictionary
 * @short_description: Concurrent dictionary implementation.
 * @include: gio/gio.h
 *
 * The #GConcurrentDictionary class represents a dictionary that can be safely
 * accessed from different threads.
 */

struct _GConcurrentDictionaryPrivate
{
  GMutex mutex;
  GHashTable *items;
};

static void g_concurrent_dictionary_collection_interface_init (GCollectionIface *iface);

G_DEFINE_TYPE_WITH_CODE (GConcurrentDictionary, g_concurrent_dictionary, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_COLLECTION, g_concurrent_dictionary_collection_interface_init))

static void
g_concurrent_dictionary_finalize (GObject *object)
{
  GConcurrentDictionary *dictionary = G_CONCURRENT_DICTIONARY (object);

  g_mutex_lock (&dictionary->priv->mutex);
  g_hash_table_destroy (dictionary->priv->items);
  g_mutex_unlock (&dictionary->priv->mutex);

  g_free (dictionary->priv);

  G_OBJECT_CLASS (g_concurrent_dictionary_parent_class)->finalize (object);
}

static void
g_concurrent_dictionary_class_init (GConcurrentDictionaryClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_concurrent_dictionary_finalize;
}

static void
g_concurrent_dictionary_init (GConcurrentDictionary *dictionary)
{
  dictionary->priv = g_new0 (GConcurrentDictionaryPrivate, 1);
  dictionary->priv->items = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_object_unref);
}

static gboolean
_collection_add (GCollection *collection, GObject *item)
{
  gchar *key = g_strdup_printf ("%p", item);
  gboolean result = g_concurrent_dictionary_add (G_CONCURRENT_DICTIONARY (collection), key, item);
  g_free (key);

  return result;
}

static gboolean
_collection_remove (GCollection *collection, GObject *item)
{
}

static GObject *
_collection_get_item (GCollection *collection, gpointer index)
{
  GConcurrentDictionary *dictionary = G_CONCURRENT_DICTIONARY (collection);

  g_return_val_if_fail (G_IS_CONCURRENT_DICTIONARY (dictionary), NULL);
  g_return_val_if_fail (index != NULL, NULL);

  return g_concurrent_dictionary_remove (dictionary, (const gchar *) index);
}

static void
g_concurrent_dictionary_collection_interface_init (GCollectionIface *iface)
{
  iface->add = _collection_add;
  iface->remove = _collection_remove;
  iface->get_item = _collection_get_item;
}

/**
 * g_concurrent_dictionary_new:
 *
 * Create a new #GConcurrentDictionary instance.
 */
GConcurrentDictionary *
g_concurrent_dictionary_new (void)
{
  return g_object_new (G_TYPE_CONCURRENT_DICTIONARY, NULL);
}

/**
 * g_concurrent_dictionary_add:
 * @dictionary: a #GConcurrentDictionary
 * @key: key for the object to be inserted
 * @item: object to add. The #GConcurrentDictionary will keep a ref
 * to this object, so after adding it to the dictionary, make sure to
 * unref it if no longer needed.
 *
 * Add a new item to the given concurrent dictionary.
 *
 * Returns: TRUE if adding the item succeeded, FALSE otherwise.
 */
gboolean
g_concurrent_dictionary_add (GConcurrentDictionary *dictionary, const gchar *key, GObject *item)
{
  g_return_val_if_fail (G_IS_CONCURRENT_DICTIONARY (dictionary), FALSE);
  g_return_val_if_fail (key != NULL, FALSE);
  g_return_val_if_fail (G_IS_OBJECT (item), FALSE);

  g_mutex_lock (&dictionary->priv->mutex);
  g_hash_table_insert (queue->priv->items, (gpointer) g_strdup (key), (gpointer) g_object_ref (item));
  g_mutex_unlock (&dictionary->priv->mutex);

  return TRUE;
}

/**
 * g_concurrent_dictionary_remove:
 * @dictionary: a #GConcurrentDictionary
 * @key: key for the object to remove
 *
 * Remove an item from the given dictionary.
 *
 * Returns: TRUE if removing the item succeeded, FALSE otherwise.
 */
gboolean
g_concurrent_dictionary_remove (GConcurrentDictionary *dictionary, const gchar *key)
{
  gboolean result;

  g_return_val_if_fail (G_IS_CONCURRENT_DICTIONARY (dictionary), FALSE);
  g_return_val_if_fail (key != NULL, FALSE);

  g_mutex_lock (&dictionary->priv->mutex);
  result = g_hash_table_remove (dictionary->priv->items, (gconstpointer) key);
  if (result)
    {
    }
  g_mutex_unlock (&dictionary->priv->mutex);

  return result;
}