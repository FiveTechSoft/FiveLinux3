#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

gboolean button_press_event( GtkWidget * hWnd, GdkEventButton * event );
gboolean motion_notify_event( GtkWidget * hWnd, GdkEventMotion * event );

HB_FUNC( CREATEIMAGE )
{
   GtkWidget * hWnd = gtk_image_new();
   GtkWidget * event_box = gtk_event_box_new();

   g_signal_connect( G_OBJECT( event_box ), "button-press-event",
                     G_CALLBACK( button_press_event ), NULL );

   g_signal_connect( G_OBJECT( event_box ), "motion-notify-event",
                     G_CALLBACK( motion_notify_event ), NULL );

   gtk_widget_set_events( event_box, GDK_EXPOSURE_MASK
                         | GDK_LEAVE_NOTIFY_MASK
                         | GDK_BUTTON_PRESS_MASK
                         | GDK_POINTER_MOTION_MASK
                         | GDK_POINTER_MOTION_HINT_MASK );

   gtk_container_add( GTK_CONTAINER( event_box ), hWnd );
   gtk_widget_show( hWnd );

   hb_retptr( event_box );
}

HB_FUNC( IMGLOADFILE ) // ( cFileName )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );
   GList * children = gtk_container_get_children( GTK_CONTAINER( hWnd ) );

   gtk_image_set_from_file( GTK_IMAGE( g_list_nth_data( children, 0 ) ), hb_parc( 2 ) );
}

HB_FUNC( IMGGETWIDTH ) // ( hWnd )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );
   GList * children = gtk_container_get_children( GTK_CONTAINER( hWnd ) );

   hb_retnl( gdk_pixbuf_get_width( gtk_image_get_pixbuf( GTK_IMAGE( g_list_nth_data( children, 0 ) ) ) ) );
}

HB_FUNC( IMGGETHEIGHT ) // ( hWnd )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );
   GList * children = gtk_container_get_children( GTK_CONTAINER( hWnd ) );

   hb_retnl( gdk_pixbuf_get_height( gtk_image_get_pixbuf( GTK_IMAGE( g_list_nth_data( children, 0 ) ) ) ) );
}
