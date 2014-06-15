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
#include "gobserver.h"

/**
 * SECTION:gobserver
 * @title: GObserver
 * @short_description: An object that can receive push notifications
 * @see_also: #GObservable
 *
 * #GObserver represents an object that can receive push notifications
 * from a #GObservable. Data is sent to the observer by using the
 * #g_observer_on_next() function, although it might happen, for some
 * #GObservable's, that no data is ever sent. Thus, observers are
 * guaranteed to get 0 or more calls to the on_next() virtual method,
 * followed optionally by a call to either on_error() or on_completed().
 */

/**
 * GObserverInterface:
 * @on_next: the virtual function pointer for g_observer_on_next()
 * on_error: the virtual function pointer for g_observer_on_error()
 * on_completed: the virtual function pointer for g_observer_on_completed()
 *
 * The virtual function table for #GObserver.
 *
 * Since: 2.39
 */

G_DEFINE_INTERFACE (GObserver, g_observer, G_TYPE_OBJECT)

static void
g_observer_default_init (GObserverInterface *iface)
{
  iface->on_next = NULL;
  iface->on_error = NULL;
  iface->on_completed = NULL;
}

/**
 * g_observer_on_next:
 * @observer: a #GObserver
 * @value: new value for the data being observed
 *
 * Push changes to the observer.
 *
 * Since: 2.39
 */
void
g_observer_on_next (GObserver *observer, GVariant *value)
{
  g_return_if_fail (G_IS_OBSERVER (observer));

  if (G_OBSERVER_GET_IFACE (observer)->on_next != NULL)
    {
      G_OBSERVER_GET_IFACE (observer)->on_next (observer, value);
    }
}

/**
 * g_observer_on_error:
 * @observer: a #GObserver
 * @error: error information
 *
 * Notifies the #GObserver of an error condition. After this call, the
 * observable will not send any further push notifications.
 *
 * Since: 2.39
 */
void
g_observer_on_error (GObserver *observer, GError *error)
{
  g_return_if_fail (G_IS_OBSERVER (observer));

  if (G_OBSERVER_GET_IFACE (observer)->on_error != NULL)
    {
      G_OBSERVER_GET_IFACE (observer)->on_error (observer, error);
    }
}

/**
 * g_observer_on_completed:
 * @observer: a #GObserver
 *
 * Notifies the #GObserver that all data has been sent. After
 * this call, the observable will not send any further
 * push notifications.
 *
 * Since: 2.39
 */
void
g_observer_on_completed (GObserver *observer)
{
  g_return_if_fail (G_IS_OBSERVER (observer));

  if (G_OBSERVER_GET_IFACE (observer)->on_completed != NULL)
    {
      G_OBSERVER_GET_IFACE (observer)->on_completed (observer);
    }
}
