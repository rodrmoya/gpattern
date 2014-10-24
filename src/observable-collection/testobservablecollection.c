#include "gobservablecollection.h"

static void
item_added_cb (GObservableCollection *collection, gpointer item, gint position)
{
}

static void
item_removed_cb (GObservableCollection *collection, gpointer item)
{
}

int
main (int argc, char *argv[])
{
  GObservableCollection *collection = g_observable_collection_new ();
  g_signal_connect (collection, "item_added", G_CALLBACK (item_added_cb), NULL);
  g_signal_connect (collection, "item_removed", G_CALLBACK (item_removed_cb), NULL);

  GMainLoop *main_loop = g_main_loop_new (NULL, TRUE);
  g_main_loop_run (main_loop);

  return 0;
}
