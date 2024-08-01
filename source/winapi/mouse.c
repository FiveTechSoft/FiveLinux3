#include <hbapi.h>
#include <gtk/gtk.h>

HB_FUNC( ISLBTNPRESSED )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 ); 
   int x, y;
   GdkModifierType state;
   GdkWindow *window = gtk_widget_get_window(hWnd);
   
   gdk_window_get_device_position(window, gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_window_get_display(window))), &x, &y, &state);
    
   hb_retl( state & GDK_BUTTON1_MASK );
}

HB_FUNC( MOUSEGETROW )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 ); 
   int x, y;
   GdkModifierType state;
   GdkWindow *window = gtk_widget_get_window(hWnd);
   
   gdk_window_get_device_position(window, gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_window_get_display(window))), &x, &y, &state);
    
   hb_retnl( y );
}

HB_FUNC( MOUSEGETCOL )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 ); 
   int x, y;
   GdkModifierType state;
   GdkWindow *window = gtk_widget_get_window(hWnd);
   
   gdk_window_get_device_position(window, gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_window_get_display(window))), &x, &y, &state);
    
   hb_retnl( x );
}