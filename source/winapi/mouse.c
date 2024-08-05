#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

HB_FUNC( ISLBTNPRESSED )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 ); 
   int x, y;
   GdkModifierType state;
   GdkWindow *window = gtk_widget_get_window(hWnd);
   GdkDisplay *display = gdk_window_get_display(window);
   GdkSeat *seat = gdk_display_get_default_seat(display);
   GdkDevice *pointer = gdk_seat_get_pointer(seat);
   
   gdk_window_get_device_position(window, pointer, &x, &y, &state);
    
   hb_retl( state & GDK_BUTTON1_MASK );
}

HB_FUNC( MOUSEGETROW )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 ); 
   int x, y;
   GdkModifierType state;
   GdkWindow *window = gtk_widget_get_window(hWnd);
   GdkDisplay *display = gdk_window_get_display(window);
   GdkSeat *seat = gdk_display_get_default_seat(display);
   GdkDevice *pointer = gdk_seat_get_pointer(seat);
   
   gdk_window_get_device_position(window, pointer, &x, &y, &state);
    
   hb_retnl( y );
}

HB_FUNC( MOUSEGETCOL )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 ); 
   int x, y;
   GdkModifierType state;
   GdkWindow *window = gtk_widget_get_window(hWnd);
   GdkDisplay *display = gdk_window_get_display(window);
   GdkSeat *seat = gdk_display_get_default_seat(display);
   GdkDevice *pointer = gdk_seat_get_pointer(seat);
   
   gdk_window_get_device_position(window, pointer, &x, &y, &state);
    
   hb_retnl( x );
}