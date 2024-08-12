#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

void CheckGtkInit( void );

static gchar *g_selected_filename = NULL;

static void ButtonOk( GtkWidget *widget, gint response, gpointer user_data )
{
   GtkFileChooser *chooser = GTK_FILE_CHOOSER(user_data);
   if (response == GTK_RESPONSE_ACCEPT) {
      g_free(g_selected_filename);
      g_selected_filename = gtk_file_chooser_get_filename(chooser);
   }
   gtk_main_quit();
}

static gboolean DeleteEvent( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
   gtk_main_quit();
   return FALSE;
}

HB_FUNC( CGETFILE ) // cTitle, cFileName
{
   GtkWidget *hWndParent = NULL;
   GtkWidget *hWnd;

   CheckGtkInit();

   if( gtk_window_list_toplevels() )
      hWndParent = g_list_last( gtk_window_list_toplevels() )->data;

   hWnd = gtk_file_chooser_dialog_new(
      HB_ISCHAR( 2 ) ? hb_parc( 2 ) : "Select a file",
      GTK_WINDOW(hWndParent),
      GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancel", GTK_RESPONSE_CANCEL,
      "_Open", GTK_RESPONSE_ACCEPT,
      NULL);

   if (HB_ISCHAR( 1 ))
      gtk_file_chooser_set_filename( GTK_FILE_CHOOSER( hWnd ), hb_parc( 1 ) );

   g_signal_connect( hWnd, "response", G_CALLBACK( ButtonOk ), hWnd );
   g_signal_connect( hWnd, "delete_event", G_CALLBACK( DeleteEvent ), NULL );

   gtk_window_set_modal( GTK_WINDOW( hWnd ), TRUE );

   gtk_widget_show_all( hWnd );

   gtk_main();

   if (g_selected_filename) {
      hb_retc( g_selected_filename );
      g_free(g_selected_filename);
      g_selected_filename = NULL;
   } else {
      hb_ret();
   }

   gtk_widget_destroy( hWnd );
}