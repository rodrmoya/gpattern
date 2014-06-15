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

#include "config.h"
#include "gtkdocumentapplication.h"
#include <glib/gi18n.h>

enum {
  PROP_ZERO,
  PROP_VIEW_TYPE,
  PROP_USE_TABS
};

/**
 * SECTION:gtkdocumentapplication
 * @title: GtkDocumentApplication
 * @short_description: Document-based application class
 *
 * #GtkDocumentApplication provides a convenient way to write document-based
 * applications, automating common things like creating, opening and saving
 * new documents.
 */

struct _GtkDocumentApplicationPrivate
{
  GType view_type;
  gboolean use_tabs;
};

G_DEFINE_TYPE_WITH_PRIVATE (GtkDocumentApplication, gtk_document_application, GTK_TYPE_APPLICATION);

static void
gtk_document_application_init (GtkDocumentApplication *application)
{
  application->priv = gtk_document_application_get_instance_private (application);
}

static void
gtk_document_application_get_property (GObject    *object,
                                       guint       prop_id,
                                       GValue     *value,
                                       GParamSpec *pspec)
{
  GtkDocumentApplication *application = GTK_DOCUMENT_APPLICATION (object);

  switch (prop_id)
    {
    case PROP_VIEW_TYPE:
      g_value_set_gtype (value, application->priv->view_type);
      break;
    case PROP_USE_TABS:
      g_value_set_bool (value, application->priv->use_tabs);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_document_application_set_property (GObject      *object,
                                       guint         prop_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
  GtkDocumentApplication *application = GTK_DOCUMENT_APPLICATION (object);

  switch (prop_id)
    {
    case PROP_VIEW_TYPE:
      application->priv->view_type = g_value_get_gtype (value);
      break;
    case PROP_USE_TABS:
      application->priv->use_tabs = g_value_get_bool (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_document_application_finalize (GObject *object)
{
  G_OBJECT_CLASS (gtk_document_application_parent_class)->finalize (object);
}

static void
gtk_document_application_class_init (GtkDocumentApplicationClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = gtk_document_application_get_property;
  object_class->set_property = gtk_document_application_set_property;
  object_class->finalize = gtk_document_application_finalize;

  g_object_class_install_property (object_class, PROP_VIEW_TYPE,
                                   g_param_spec_gtype ("view-type",
                                                       "View type",
                                                       "GType to use for instantiating new documents",
                                                       GTK_TYPE_WIDGET,
                                                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_USE_TABS,
                                   g_param_spec_boolean ("use-tabs",
                                                         "Use tabs",
                                                         "Whether to use tabs or separate windows for each document",
                                                         FALSE,
                                                         G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

/**
 * gtk_document_application_new:
 */
GtkDocumentApplication *
gtk_document_application_new (const gchar *application_id,
                              GApplicationFlags flags,
                              GType view_type)
{
  return g_object_new (GTK_TYPE_DOCUMENT_APPLICATION,
                       "application-id", application_id,
                       "flags", flags,
                       "register-session", TRUE,
                       "view-type", view_type,
                       "use-tabs", FALSE,
                       NULL);
}
