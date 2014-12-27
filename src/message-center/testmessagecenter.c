#include "gmessagecenter.h"

GMainLoop *main_loop;

static void
filter_cb (GMessageCenter *msg_center, GMessage *msg, gpointer user_data)
{
  g_main_loop_quit (main_loop);
}

static gboolean
timeout_cb (gpointer user_data)
{
  GMessageCenter *msg_center = G_MESSAGE_CENTER (user_data);
  static int count = 1;

  g_message_center_send_full (msg_center, G_OBJECT (msg_center), "test.message.count", NULL);
  if (count++ >= 10)
    {
      g_print ("Done sending messages, sending QUIT message\n");
      g_message_center_send_with_delay (msg_center, g_message_new (G_OBJECT (msg_center), "test.message.quit", NULL), 2000);
      return FALSE;
    }

  return TRUE;
}

static void
monitor_cb (GMessageCenter *msg_center, GMessage *msg, gpointer user_data)
{
  g_print ("Got message %s\n", g_message_get_name (msg));
}

int
main (int argc, char *argv[])
{
  GMessageCenter *msg_center = g_message_center_get ();
  g_signal_connect (G_OBJECT (msg_center), "message-received", G_CALLBACK (monitor_cb), NULL);
  g_message_center_filter (msg_center, NULL, "test.message.quit", filter_cb, NULL);

  main_loop = g_main_loop_new (NULL, TRUE);
  g_timeout_add (1000, timeout_cb, msg_center);
  g_main_loop_run (main_loop);

  return 0;
}
