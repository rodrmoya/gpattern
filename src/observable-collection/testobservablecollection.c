#include <gio/gio.h>
#include "gobservablecollection.h"

GMainLoop *main_loop;

static void
item_added_cb (GObservableCollection *collection, gpointer item, gint position)
{
  g_print ("item %p added at position %d\n", item, position);
}

static void
item_removed_cb (GObservableCollection *collection, gpointer item)
{
  g_print ("item %p removed\n", item);
}

static gpointer
update_thread_func (gpointer user_data)
{
  static gint counter = 0;
  GObservableCollection *collection = G_OBSERVABLE_COLLECTION (user_data);

  while (counter < 100) {
    g_observable_collection_append (collection, g_strdup_printf ("Item %d", counter++));
  }

  return NULL;
}

static gpointer
consume_thread_func (gpointer user_data)
{
  GObservableCollection *collection = G_OBSERVABLE_COLLECTION (user_data);
  gpointer item;

  while ((item = g_observable_collection_item_at (collection, 0))) {
    g_observable_collection_remove (collection, item);
  }

  g_main_loop_quit (main_loop);

  return NULL;
}

int
main (int argc, char *argv[])
{
  GObservableCollection *collection = g_observable_collection_new ();
  g_signal_connect (collection, "item_added", G_CALLBACK (item_added_cb), NULL);
  g_signal_connect (collection, "item_removed", G_CALLBACK (item_removed_cb), NULL);

  main_loop = g_main_loop_new (NULL, TRUE);

  // Start tasks
  GThread *updater_thread = g_thread_new ("updater", (GThreadFunc) update_thread_func, collection);
  GThread *consumer_thread = g_thread_new ("consumer", (GThreadFunc) consume_thread_func, collection);

  g_main_loop_run (main_loop);

  return 0;
}
