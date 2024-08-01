#include <hbapi.h>
#include <gtk/gtk.h>

HB_FUNC( CREATECURSOR )
{
   GdkCursor * hCursor = gdk_cursor_new_for_display(gdk_display_get_default(), (GdkCursorType) hb_parnl(1));

   GdkWindow * window = gtk_widget_get_window(GTK_WIDGET(hb_parnl(2)));
   gdk_window_set_cursor(window, hCursor);
 
   hb_retptr(hCursor);
}

HB_FUNC( CURSORSIZE )
{
   GdkCursor * hCursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_SIZING);

   GdkWindow * window = gtk_widget_get_window(GTK_WIDGET(hb_parptr(1)));
   gdk_window_set_cursor(window, hCursor);
 
   hb_retptr(hCursor);
}

HB_FUNC( CURSORARROW )
{
   GdkCursor * hCursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_ARROW);

   GdkWindow * window = gtk_widget_get_window(GTK_WIDGET(hb_parptr(1)));
   gdk_window_set_cursor(window, hCursor);
 
   hb_retptr(hCursor);
}

HB_FUNC( CURSOREND )
{
   g_object_unref(G_OBJECT((GdkCursor *) hb_parptr(1)));
}