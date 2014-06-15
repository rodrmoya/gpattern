/*
 * Copyright © 2014 Rodrigo Moya
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Rodrigo Moya <rodrigo@gnome-db.org>
 */

#ifndef __GTK_DOCUMENT_APPLICATION_H__
#define __GTK_DOCUMENT_APPLICATION_H__

#include <gtk/gtkapplication.h>

G_BEGIN_DECLS

#define GTK_TYPE_DOCUMENT_APPLICATION            (gtk_document_application_get_type ())
#define GTK_DOCUMENT_APPLICATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_DOCUMENT_APPLICATION, GtkDocumentApplication))
#define GTK_DOCUMENT_APPLICATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_DOCUMENT_APPLICATION, GtkDocumentApplicationClass))
#define GTK_IS_DOCUMENT_APPLICATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_DOCUMENT_APPLICATION))
#define GTK_IS_DOCUMENT_APPLICATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_DOCUMENT_APPLICATION))
#define GTK_DOCUMENT_APPLICATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_DOCUMENT_APPLICATION, GtkDocumentApplicationClass))

typedef struct _GtkDocumentApplication        GtkDocumentApplication;
typedef struct _GtkDocumentApplicationClass   GtkDocumentApplicationClass;
typedef struct _GtkDocumentApplicationPrivate GtkDocumentApplicationPrivate;

struct _GtkDocumentApplication
{
  GtkApplication parent;

  /*< private >*/
  GtkDocumentApplicationPrivate *priv;
};

struct _GtkDocumentApplicationClass
{
  GtkApplicationClass parent_class;

  /*< public >*/
};

GDK_AVAILABLE_IN_ALL
GType                   gtk_document_application_get_type (void) G_GNUC_CONST;

GDK_AVAILABLE_IN_ALL
GtkDocumentApplication *gtk_document_application_new      (const gchar *application_id,
                                                           GApplicationFlags flags,
                                                           GType window_type);

G_END_DECLS

#endif
