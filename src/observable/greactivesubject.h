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

#ifndef __G_REACTIVE_SUBJECT_H__
#define __G_REACTIVE_SUBJECT_H__

#if !defined(__GPATTERN_GPATTERN_H_INSIDE__) && !defined(GPATTERN_COMPILATION)
#error "Only <gpattern/gpattern.h> can be included directly"
#endif

#include <gpattern/gpatterntypes.h>

G_BEGIN_DECLS

#define G_TYPE_REACTIVE_SUBJECT         (g_reactive_subject_get_type ())
#define G_REACTIVE_SUBJECT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_REACTIVE_SUBJECT, GReactiveSubject))
#define G_REACTIVE_SUBJECT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_REACTIVE_SUBJECT, GReactiveSubjectClass))
#define G_IS_REACTIVE_SUBJECT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_REACTIVE_SUBJECT))
#define G_IS_REACTIVE_SUBJECT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_REACTIVE_SUBJECT))
#define G_REACTIVE_SUBJECT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_REACTIVE_SUBJECT, GReactiveSubjectClass))

typedef struct _GReactiveSubjectClass   GReactiveSubjectClass;
typedef struct _GReactiveSubjectPrivate GReactiveSubjectPrivate;

/**
 * GReactiveSubject:
 *
 * Represents an object that is both an observable sequence as well as an observer.
 **/
struct _GReactiveSubject
{
  GObject parent_instance;

  /*< private >*/
  GReactiveSubjectPrivate *priv;
};

struct _GReactiveSubjectClass
{
  GObjectClass parent_class;
};

GType             g_reactive_subject_get_type (void) G_GNUC_CONST;

GReactiveSubject *g_reactive_subject_new      (void);
GReactiveSubject *g_reactive_subject_new_full (gboolean replay,
                                               gboolean async,
                                               guint cache_size);

/* GReactiveSubject factory */
GReactiveSubject *g_reactive_subject_new_for_property (GObject object,
                                                       const gchar *property_name);

G_END_DECLS

#endif
