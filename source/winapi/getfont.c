#include <hbapi.h>
#include <gtk/gtk.h>

void CheckGtkInit( void );

static void ButtonOk( GtkWidget *hWnd, gpointer data )
{
   GtkFontChooser *chooser = GTK_FONT_CHOOSER(data);
   hb_retc( gtk_font_chooser_get_font( chooser ) );

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
   gtk_widget_destroy( hWnd );
   gtk_main_quit();
   return FALSE;
}

HB_FUNC( CHOOSEFONT ) // cTitle, cFontName
{
   GtkWidget *hWndParent = NULL;
   GtkWidget *hWnd;

   CheckGtkInit();

   if( gtk_window_list_toplevels() )
      hWndParent = g_list_last( gtk_window_list_toplevels() )->data;

   hWnd = gtk_font_chooser_dialog_new(
      HB_ISCHAR( 1 ) ? hb_parc( 1 ) : "Select a font",
      GTK_WINDOW(hWndParent));

   if( hb_pcount() > 1 )
      gtk_font_chooser_set_font( GTK_FONT_CHOOSER( hWnd ), hb_parc( 2 ) );

   g_signal_connect( hWnd, "response", G_CALLBACK( ButtonOk ), hWnd );
   g_signal_connect( hWnd, "delete_event", G_CALLBACK( DeleteEvent ), NULL );

   gtk_window_set_modal( GTK_WINDOW( hWnd ), TRUE );

   gtk_widget_show_all( hWnd );

   gtk_main();
}
