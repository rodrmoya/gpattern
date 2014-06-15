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

#ifndef __GPATTERN_TYPES_H__
#define __GPATTERN_TYPES_H__

#if !defined(__GPATTERN_GPATTERN_H_INSIDE__) && !defined(GPATTERN_COMPILATION)
#error "Only <gpattern/gpattern.h> can be included directly"
#endif

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _GObserver        GObserver;
typedef struct _GObservable      GObservable;
typedef struct _GReactiveSubject GReactiveSubject;

G_END_DECLS

#endif /* __GPATTERN_TYPES_H__ */
