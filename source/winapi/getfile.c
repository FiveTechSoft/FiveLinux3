#include <hbapi.h>
#include <gtk/gtk.h>

void CheckGtkInit( void );

static void ButtonOk( GtkWidget *hWnd, gpointer data )
{
   GtkFileChooser *chooser = GTK_FILE_CHOOSER(data);
   hb_retc( gtk_file_chooser_get_filename( chooser ) );

   gtk_widget_destroy( hWnd );
   gtk_main_quit();
}

static void ButtonCancel( GtkWidget *hWnd, gpointer data )
{
   gtk_widget_destroy( hWnd );
   gtk_main_quit();
}

static gboolean DeleteEvent( GtkWidget *hWnd, GdkEvent *event, gpointer data )
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

   gtk_file_chooser_set_filename( GTK_FILE_CHOOSER( hWnd ), hb_parc( 1 ) );

   g_signal_connect( hWnd, "response", G_CALLBACK( ButtonOk ), hWnd );
   g_signal_connect( hWnd, "delete_event", G_CALLBACK( DeleteEvent ), NULL );

   gtk_window_set_modal( GTK_WINDOW( hWnd ), TRUE );

   gtk_widget_show_all( hWnd );

   gtk_main();
}
