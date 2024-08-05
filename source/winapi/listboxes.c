#include <hbapi.h>
#include <hbvm.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

void LbxChangeEvent(GtkListBox *hWnd, GtkListBoxRow *hItem, gpointer user_data);
gboolean LostFocusEvent( GtkWidget * hWnd, GdkEventFocus * event, gpointer user_data );
gboolean button_press_event( GtkWidget * hWnd, GdkEventButton * event );
gboolean motion_notify_event( GtkWidget * hWnd, GdkEventMotion * event );

HB_FUNC( CREATELISTBOX )
{
   GtkWidget * hScrollBars = gtk_scrolled_window_new( NULL, NULL );
   GtkWidget * hWnd = gtk_list_box_new();

   gtk_container_add( GTK_CONTAINER( hScrollBars ), hWnd );

   g_object_set_data( G_OBJECT( hScrollBars ), "hWnd", ( gpointer ) hWnd );

   gtk_widget_set_can_focus( hWnd, TRUE );
   gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( hScrollBars ),
                                   GTK_POLICY_NEVER, GTK_POLICY_ALWAYS );

   // g_signal_connect( G_OBJECT( hWnd ), "focus-out-event",
   //                   G_CALLBACK( LostFocusEvent ), NULL );

   // Set selection mode for GtkListBox
   gtk_list_box_set_selection_mode( GTK_LIST_BOX( hWnd ), GTK_SELECTION_SINGLE );

   g_signal_connect( G_OBJECT( hWnd ), "row-selected",
                     G_CALLBACK( LbxChangeEvent ), ( gpointer ) hScrollBars );

   g_signal_connect( G_OBJECT( hWnd ), "button-press-event",
                     G_CALLBACK( button_press_event ), NULL );

   g_signal_connect( G_OBJECT( hWnd ), "motion-notify-event",
                     G_CALLBACK( motion_notify_event ), NULL );

   gtk_widget_add_events( hWnd, GDK_EXPOSURE_MASK
                         | GDK_LEAVE_NOTIFY_MASK
                         | GDK_BUTTON_PRESS_MASK
                         | GDK_POINTER_MOTION_MASK
                         | GDK_POINTER_MOTION_HINT_MASK );

   gtk_widget_show_all( hScrollBars );

   hb_retptr( hScrollBars );
}

HB_FUNC( LBXSETITEMS ) // ( hWnd, aItems )
{
    GtkWidget * hScrollBars = GTK_WIDGET( hb_parptr( 1 ) );
    GtkWidget * hWnd = g_object_get_data( G_OBJECT( hScrollBars ), "hWnd" );
    int iLen = hb_parinfa( 2, 0 ), i;
    GtkListBox *listbox = GTK_LIST_BOX( hWnd );

    // Primero, eliminamos todos los elementos existentes
    GList *children, *iter;
    children = gtk_container_get_children( GTK_CONTAINER( listbox ) );
    for( iter = children; iter != NULL; iter = g_list_next( iter ) ) {
        gtk_widget_destroy( GTK_WIDGET( iter->data ) );
    }
    g_list_free( children );

    // Ahora añadimos los nuevos elementos
    for( i = 0; i < iLen; i++ ) 
    {
        GtkWidget * label = gtk_label_new( hb_parvc( 2, i + 1 ) );
        gtk_widget_set_halign( label, GTK_ALIGN_START );
        gtk_list_box_insert( listbox, label, -1 );
        GtkListBoxRow *row = gtk_list_box_get_row_at_index( listbox, i );
        g_object_set_data( G_OBJECT( row ), "index", GINT_TO_POINTER( i + 1 ) );
    }
    gtk_widget_show_all( hWnd );
}

HB_FUNC( LBXSELITEM ) // ( hWnd, nItem )
{
   GtkWidget * hScrollBars = GTK_WIDGET( hb_parptr( 1 ) );
   GtkWidget * hWnd = g_object_get_data( G_OBJECT( hScrollBars ), "hWnd" );

   gtk_list_box_select_row( GTK_LIST_BOX( hWnd ), gtk_list_box_get_row_at_index( GTK_LIST_BOX( hWnd ), hb_parni( 2 ) - 1 ) );
}