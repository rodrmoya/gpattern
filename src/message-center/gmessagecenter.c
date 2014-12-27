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

#include "gmessagecenter.h"

/**
 * SECTION:gmessagecenter
 * @title: GMessageCenter
 * @short_description: Application-wide message passing interface
 */

typedef struct
{
  GObject *sender;
  gchar *message_name;
  GMessageCenterFilterFunc callback;
  gpointer user_data;
} RegisteredCallback;

struct _GMessageCenterPrivate
{
  GSList *registered_callbacks;
};

enum {
  MESSAGE_RECEIVED_SIGNAL,
  LAST_SIGNAL
};
static guint message_center_signals [LAST_SIGNAL] ;

G_DEFINE_TYPE (GMessageCenter, g_message_center, G_TYPE_OBJECT)

static void
g_message_center_finalize (GObject *object)
{
  GMessageCenter *message_center = G_MESSAGE_CENTER (object);

  if (message_center->priv != NULL) {
    while (message_center->priv->registered_callbacks != NULL)
      {
        RegisteredCallback *rc = message_center->priv->registered_callbacks->data;
        message_center->priv->registered_callbacks = g_slist_remove (message_center->priv->registered_callbacks, rc);

        if (rc->sender != NULL)
          {
            g_object_unref (rc->sender);
          }
        g_free (rc->message_name);
        g_free (rc);
      }
  }

  G_OBJECT_CLASS (g_message_center_parent_class)->finalize (object);
}

static void
g_message_center_class_init (GMessageCenterClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = g_message_center_finalize;

  message_center_signals[MESSAGE_RECEIVED_SIGNAL] =
		g_signal_new ("message-received",
                  G_OBJECT_CLASS_TYPE (object_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (GMessageCenterClass, message_received),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__OBJECT,
                  G_TYPE_NONE, 1,
                  G_TYPE_OBJECT);
}

static void
g_message_center_init (GMessageCenter *message_center)
{
  message_center->priv = g_message_center_get_instance_private (message_center);
}

/**
 * g_message_center_get:
 */
GMessageCenter *
g_message_center_get (void)
{
  static GMessageCenter *message_center = NULL;

  if (message_center == NULL) {
    message_center = g_object_new (G_TYPE_MESSAGE_CENTER, NULL);
  }

  return message_center;
}

/**
 * g_message_center_send:
 */
void
g_message_center_send (GMessageCenter *message_center, GMessage *message)
{
  GSList *l;

  g_return_if_fail (G_IS_MESSAGE_CENTER (message_center));
  g_return_if_fail (G_IS_MESSAGE (message));

  /* 1st, notify all listeners */
  g_signal_emit_by_name (message_center, "message-received", message);

  /* 2nd, notify registered filters */
  for (l = message_center->priv->registered_callbacks; l != NULL; l = l->next)
    {
      RegisteredCallback *rc = l->data;

      if ((rc->sender == NULL || rc->sender == g_message_get_sender (message)) &&
          !g_strcmp0 (rc->message_name, g_message_get_name (message)))
        {
          rc->callback (message_center, message, rc->user_data);
        }
    }
}

typedef struct
{
  GMessageCenter *message_center;
  GMessage *message;
} DelayedMessage;

static gboolean
delayed_send_message_cb (gpointer user_data)
{
  DelayedMessage *dm = (DelayedMessage *) user_data;

  g_message_center_send (dm->message_center, dm->message);

  g_object_unref (dm->message_center);
  if (dm->message != NULL)
    {
      g_object_unref (dm->message);
    }
  g_free (dm);

  return FALSE;
}

/**
 * g_message_center_send_with_delay:
 */
void
g_message_center_send_with_delay (GMessageCenter *message_center, GMessage *message, guint delay)
{
  g_return_if_fail (G_IS_MESSAGE_CENTER (message_center));
  g_return_if_fail (G_IS_MESSAGE (message));

  if (delay == 0)
    {
      g_message_center_send (message_center, message);
    }
  else
    {
      DelayedMessage *dm = g_new0 (DelayedMessage, 1);
      dm->message_center = g_object_ref (message_center);
      dm->message = g_object_ref (message);

      g_timeout_add (delay, delayed_send_message_cb, dm);
    }
}

/**
 * g_message_center_send_full:
 */
void
g_message_center_send_full (GMessageCenter *message_center, GObject *sender, const gchar *message_name, gpointer user_data)
{
  GMessage *msg;

  g_return_if_fail (G_IS_MESSAGE_CENTER (message_center));

  msg = g_message_new (sender, message_name, user_data);
  if (msg != NULL)
    {
      g_message_center_send (message_center, msg);
      g_object_unref (msg);
    }
}

/**
 * g_message_center_send_full_with_delay:
 */
void
g_message_center_send_full_with_delay (GMessageCenter *message_center,
                                       GObject *sender,
                                       const gchar *message_name,
                                       gpointer user_data,
                                       guint delay)
{
  GMessage *msg;

  g_return_if_fail (G_IS_MESSAGE_CENTER (message_center));

  msg = g_message_new (sender, message_name, user_data);
  if (msg != NULL)
    {
      g_message_center_send_with_delay (message_center, msg, delay);
      g_object_unref (msg);
    }
}

/**
 * g_message_center_filter:
 */
void
g_message_center_filter (GMessageCenter *message_center,
                         GObject *sender,
                         const gchar *message_name,
                         GMessageCenterFilterFunc callback,
                         gpointer user_data)
{
  RegisteredCallback *rc;

  g_return_if_fail (G_IS_MESSAGE_CENTER (message_center));
  g_return_if_fail (G_IS_OBJECT (sender) || message_name != NULL);
  g_return_if_fail (callback != NULL);

  rc = g_new0 (RegisteredCallback, 1);
  rc->sender = G_IS_OBJECT (sender) ? g_object_ref (sender) : NULL;
  rc->message_name = g_strdup (message_name);
  rc->callback = callback;
  rc->user_data = user_data;

  message_center->priv->registered_callbacks = g_slist_append (message_center->priv->registered_callbacks, rc);
}
