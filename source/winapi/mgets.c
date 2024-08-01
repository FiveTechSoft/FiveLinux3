#include <hbapi.h>
#include <hbvm.h>
#include <gtk/gtk.h>

gint LostFocusEvent( GtkWidget * hWnd, GdkEventFocus * event,
                     gpointer user_data );

HB_FUNC( CREATETEXT )
{
   GtkWidget * hViewPort = gtk_viewport_new( NULL, NULL );
   GtkWidget * hScrollBars = gtk_scrolled_window_new( NULL, NULL );
   GtkWidget * hWnd = gtk_text_view_new();

   gtk_text_view_set_cursor_visible( GTK_TEXT_VIEW( hWnd ), TRUE );

   gtk_container_add( GTK_CONTAINER( hScrollBars ), hWnd ); // ViewPort );
   // gtk_container_add( GTK_CONTAINER( hViewPort ), hWnd );

   g_object_set_data( G_OBJECT( hScrollBars ), "hWnd", ( gpointer ) hWnd );
   g_object_set_data( G_OBJECT( hWnd ), "hScrolls",
                        ( gpointer ) hScrollBars );
   g_object_set_data( G_OBJECT( hScrollBars ), "hViewPort", ( gpointer ) hViewPort );

   gtk_widget_set_can_focus( hScrollBars, TRUE );
   gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( hScrollBars ),
                                   GTK_POLICY_NEVER, GTK_POLICY_ALWAYS );

   g_signal_connect( hWnd, "focus-out-event",
                       G_CALLBACK( LostFocusEvent ), NULL );

   hb_retptr( hScrollBars );
}

HB_FUNC( TXTSETTEXT )
{
   GtkWidget * hWnd = g_object_get_data( G_OBJECT( hb_parptr( 1 ) ), "hWnd" );
   GtkTextBuffer * buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW( hWnd ) );

   gtk_text_buffer_set_text( buffer, hb_parc( 2 ), -1 );
}

HB_FUNC( TXTGETTEXT )
{
   GtkWidget * hWnd = g_object_get_data( G_OBJECT( hb_parptr( 1 ) ), "hWnd" );
   GtkTextBuffer * buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW( hWnd ) );
   GtkTextIter start, end;

   gtk_text_buffer_get_bounds( buffer, &start, &end );

   hb_retc( gtk_text_buffer_get_text( buffer, &start, &end, TRUE ) );
}