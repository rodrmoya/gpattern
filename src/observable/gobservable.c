/* GPattern - GLib software patterns implementation library
 *
 * Copyright (C) 2013 Rodrigo Moya
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
#include "gobservable.h"

/**
 * SECTION:gobservable
 * @title: GObservable
 * @short_description: An object that can be observed for changes
 * @see_also: #GObserver
 *
 * #GObservable represents an object that other objects can attach to
 * to receive notification of changes on the observed object.
 */

/**
 * GObservableInterface:
 * @subscribe: the virtual function pointer for g_observable_subscribe()
 *
 * The virtual function table for #GObservable.
 *
 * Since: 2.39
 */

G_DEFINE_INTERFACE (GObservable, g_observable, G_TYPE_OBJECT)

static void
g_observable_default_init (GObservableInterface *iface)
{
  iface->subscribe = NULL;
  iface->unsubscribe = NULL;
}

/**
 * g_observable_subscribe:
 * @observable: a #GObservable
 * @observer: a #GObserver
 *
 * Subscribes the given #GObserver so that it gets notified of changes
 * in @observable.
 *
 * Since: 2.39
 */
void
g_observable_subscribe (GObservable *observable, GObserver *observer)
{
  g_return_if_fail (G_IS_OBSERVABLE (observable));

  if (G_OBSERVABLE_GET_IFACE (observable)->subscribe != NULL)
    {
      G_OBSERVABLE_GET_IFACE (observable)->subscribe (observable, observer);
    }
}

/**
 * g_observable_unsubscribe:
 * @observable: a #GObservable
 * @observer: a #GObserver
 *
 * Unsubscribes the given #GObserver so that it stops getting notification
 * of changes from  @observable.
 *
 * Since: 2.39
 */
void
g_observable_unsubscribe (GObservable *observable, GObserver *observer)
{
  g_return_if_fail (G_IS_OBSERVABLE (observable));

  if (G_OBSERVABLE_GET_IFACE (observable)->unsubscribe != NULL)
    {
      G_OBSERVABLE_GET_IFACE (observable)->unsubscribe (observable, observer);
    }
}
