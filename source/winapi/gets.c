#include <hbapi.h>
#include <hbvm.h>
#include <gtk/gtk.h>

gboolean LostFocusEvent( GtkWidget * hWnd, GdkEventFocus * event, gpointer user_data );
gboolean GotFocusEvent( GtkWidget * hWnd, GdkEventFocus * event, gpointer user_data );
gboolean KeyPressEvent( GtkWidget * hWnd, GdkEventKey * event, gpointer user_data );
gboolean ButtonPressEvent( GtkWidget * hWnd, GdkEventButton * event );
gboolean motion_notify_event( GtkWidget * hWnd, GdkEventMotion * event );

HB_FUNC( CREATEGET )
{
   GtkWidget * hWnd = gtk_entry_new();

   g_object_set( gtk_widget_get_settings( hWnd ), "gtk-entry-select-on-focus", FALSE, NULL );

   g_signal_connect( G_OBJECT( hWnd ), "focus_out_event", G_CALLBACK( LostFocusEvent ), NULL );
   g_signal_connect_after( G_OBJECT( hWnd ), "focus_in_event", G_CALLBACK( GotFocusEvent ), NULL );
   g_signal_connect( G_OBJECT( hWnd ), "key_press_event", G_CALLBACK( KeyPressEvent ), NULL );
   g_signal_connect( G_OBJECT( hWnd ), "button_press_event", G_CALLBACK( ButtonPressEvent ), NULL );
   g_signal_connect( G_OBJECT( hWnd ), "motion_notify_event", G_CALLBACK( motion_notify_event ), NULL );

   gtk_widget_set_events( hWnd, GDK_EXPOSURE_MASK 
                          | GDK_LEAVE_NOTIFY_MASK
                          | GDK_BUTTON_PRESS_MASK
                          | GDK_POINTER_MOTION_MASK
                          | GDK_POINTER_MOTION_HINT_MASK );

   hb_retptr( hWnd );
}

HB_FUNC( GETGETCURPOS )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   hb_retni( gtk_editable_get_position( GTK_EDITABLE( hWnd ) ) );
}

HB_FUNC( GETSETCURPOS )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   gtk_editable_set_position( GTK_EDITABLE( hWnd ), hb_parni( 2 ) );
}

HB_FUNC( GETSETTEXT )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   gtk_entry_set_text( GTK_ENTRY( hWnd ), hb_parc( 2 ) );
}

HB_FUNC( GETGETTEXT )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   hb_retc( ( char * ) gtk_entry_get_text( GTK_ENTRY( hWnd ) ) );
}

HB_FUNC( GETSETSEL )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   gtk_editable_select_region( ( GtkEditable * ) hWnd, ( gint ) hb_parni( 2 ), ( gint ) hb_parni( 3 ) );
}
