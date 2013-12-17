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

#ifndef __G_OBSERVER_H__
#define __G_OBSERVER_H__

#if !defined(__GPATTERN_GPATTERN_H_INSIDE__) && !defined(GPATTERN_COMPILATION)
#error "Only <gpattern/gpattern.h> can be included directly"
#endif

#include <gobject/gobject.h>

G_BEGIN_DECLS

#define G_TYPE_OBSERVER            (g_observer_get_type ())
#define G_OBSERVER(inst)           (G_TYPE_CHECK_INSTANCE_CAST ((inst), G_TYPE_OBSERVER, GObserver))
#define G_IS_OBSERVER(inst)        (G_TYPE_CHECK_INSTANCE_TYPE ((inst), G_TYPE_OBSERVER))
#define G_OBSERVER_GET_IFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE ((inst), G_TYPE_OBSERVER, GObserverInterface))

typedef struct _GObserverInterface GObserverInterface;

struct _GObserverInterface
{
  GTypeInterface g_iface;

  /* virtual functions */
  void on_next      (GObserver *observer, GVariant *value);
  void on_error     (GObserver *observer, GError *error);
  void on_completed (GObserver *observer);

  /* signals */
};

GLIB_AVAILABLE_IN_2_39
GType g_observer_get_type (void) G_GNUC_CONST;

GLIB_AVAILABLE_IN_2_39
void g_observer_on_next (GObserver *observer, GVariant *value);

GLIB_AVAILABLE_IN_2_39
void g_observer_on_error (GObserver *observer, GError *error);

GLIB_AVAILABLE_IN_2_39
void g_observer_on_completed (GObserver *observer);

G_END_DECLS

#endif
