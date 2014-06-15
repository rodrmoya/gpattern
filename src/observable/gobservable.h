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

#ifndef __G_OBSERVABLE_H__
#define __G_OBSERVABLE_H__

#if !defined(__GPATTERN_GPATTERN_H_INSIDE__) && !defined(GPATTERN_COMPILATION)
#error "Only <gpattern/gpattern.h> can be included directly"
#endif

#include <gpattern/gobserver.h>

G_BEGIN_DECLS

#define G_TYPE_OBSERVABLE            (g_observable_get_type ())
#define G_OBSERVABLE(inst)           (G_TYPE_CHECK_INSTANCE_CAST ((inst), G_TYPE_OBSERVABLE, GObservable))
#define G_IS_OBSERVABLE(inst)        (G_TYPE_CHECK_INSTANCE_TYPE ((inst), G_TYPE_OBSERVABLE))
#define G_OBSERVABLE_GET_IFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE ((inst), G_TYPE_OBSERVABLE, GObservableInterface))

typedef struct _GObservableInterface GObservableInterface;

struct _GObservableInterface
{
  GTypeInterface g_iface;

  /* virtual functions */
  void (* subscribe)   (GObservable *observable, GObserver *observer);
  void (* unsubscribe) (GObservable *observable, GObserver *observer);

  /* signals */
};

GType g_observable_get_type   (void) G_GNUC_CONST;

void g_observable_subscribe   (GObservable *observable, GObserver *observer);
void g_observable_unsubscribe (GObservable *observable, GObserver *observer);


G_END_DECLS

#endif
