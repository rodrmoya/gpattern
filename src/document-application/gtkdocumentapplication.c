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

enum {
  PROP_ZERO,
  PROP_WINDOW_TYPE
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
  GType window_type;
};

G_DEFINE_TYPE_WITH_PRIVATE (GtkDocumentApplication, gtk_document_application, GTK_TYPE_APPLICATION)

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
    PROP_WINDOW_TYPE:
      g_value_set_type (value, application->priv->window_type);
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
    case PROP_WINDOW_TYPE:
      application->priv->window_type = g_value_get_type (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_document_application_finalize (GObject object)
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

  g_object_class_install_property (object_class, PROP_WINDOW_TYPE,
                                   g_param_spec_type ("window-type",
                                                      P_("Window type"),
                                                      P_("GType to use for instantiating windows"),
                                                      GTK_DOCUMENT_WINDOW, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

/**
 * gtk_document_application_new:
 */
GtkDocumentApplication *
gtk_document_application_new (const gchar *application_id,
                              GApplicationFlags flags,
                              GType window_type)
{
  return g_object_new (GTK_TYPE_DOCUMENT_APPLICATION,
                       "application-id", application_id,
                       "flags", flags,
                       "register-session", true,
                       "window-type", window_type,
                       NULL);
}
