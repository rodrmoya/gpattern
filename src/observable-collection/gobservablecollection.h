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

#ifndef __G_OBSERVABLE_COLLECTION_H__
#define __G_OBSERVABLE_COLLECTION_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define G_TYPE_OBSERVABLE_COLLECTION         (g_observable_collection_get_type ())
#define G_OBSERVABLE_COLLECTION(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_OBSERVABLE_COLLECTION, GObservableCollection))
#define G_OBSERVABLE_COLLECTION_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_OBSERVABLE_COLLECTION, GObservableCollectionClass))
#define G_IS_OBSERVABLE_COLLECTION(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_OBSERVABLE_COLLECTION))
#define G_IS_OBSERVABLE_COLLECTION_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_OBSERVABLE_COLLECTION))
#define G_OBSERVABLE_COLLECTION_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_OBSERVABLE_COLLECTION, GObservableCollectionClass))

typedef struct _GObservableCollection        GObservableCollection;
typedef struct _GObservableCollectionClass   GObservableCollectionClass;
typedef struct _GObservableCollectionPrivate GObservableCollectionPrivate;

/**
 * GObservableCollection:
 *
 * Represents a collection of items that emits notifications of any change to the collection.
 */
struct _GObservableCollection
{
  GObject parent_instance;

  /*< private >*/
  GObservableCollectionPrivate *priv;
};

struct _GObservableCollectionClass
{
  GObjectClass parent_class;

  void (* item_added)   (GObservableCollection *collection, gpointer item, gint position);
  void (* item_removed) (GObservableCollection *collection, gpointer item);
};

GType                  g_observable_collection_get_type      (void) G_GNUC_CONST;

GObservableCollection *g_observable_collection_new           (void);

void                   g_observable_collection_append        (GObservableCollection *collection, gpointer item);
void                   g_observable_collection_prepend       (GObservableCollection *collection, gpointer item);
void                   g_observable_collection_insert        (GObservableCollection *collection, gpointer item, gint position);
void                   g_observable_collection_insert_sorted (GObservableCollection *collection, gpointer item, GCompareFunc compare_func);
void                   g_observable_collection_remove        (GObservableCollection *collection, gpointer item);
void                   g_observable_collection_reverse       (GObservableCollection *collection);

gpointer               g_observable_collection_item_at       (GObservableCollection *collection, gint position);
gint                   g_observable_collection_index         (GObservableCollection *collection, gpointer item);

G_END_DECLS

#endif
