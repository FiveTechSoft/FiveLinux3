#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

gint LostFocusEvent( GtkWidget * hWnd, GdkEventFocus * event, gpointer user_data );
void CbxChangeEvent( GtkWidget * hWnd, GtkWidget * hItem, gpointer user_data );
gboolean button_press_event( GtkWidget * hWnd, GdkEventButton * event );
gboolean motion_notify_event( GtkWidget * hWnd, GdkEventMotion * event );

HB_FUNC( CREATECOMBOBOX )
{
   GtkWidget *hWnd = gtk_combo_box_text_new();

   gtk_widget_set_can_focus(hWnd, TRUE);

   g_signal_connect( G_OBJECT( hWnd ), "focus_out_event",
                       G_CALLBACK( LostFocusEvent ), NULL );

   g_signal_connect( G_OBJECT(hWnd), "changed",
                     G_CALLBACK(CbxChangeEvent), (gpointer)hWnd);

   g_signal_connect( G_OBJECT( hWnd ), "button_press_event",
                     G_CALLBACK( button_press_event ), NULL );

   g_signal_connect( G_OBJECT( hWnd ), "motion_notify_event",
                     G_CALLBACK( motion_notify_event ), NULL );

   gtk_widget_set_events( hWnd, GDK_EXPOSURE_MASK
			 | GDK_LEAVE_NOTIFY_MASK
			 | GDK_BUTTON_PRESS_MASK
			 | GDK_POINTER_MOTION_MASK
			 | GDK_POINTER_MOTION_HINT_MASK );

   hb_retptr( hWnd );
}

HB_FUNC( CBXSETTEXT ) // ( hWnd, cText )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   gtk_combo_box_text_insert_text( ( GtkComboBoxText * ) hWnd, 0, hb_parc( 2 ) );
}

HB_FUNC( CBXGETTEXT ) // ( hWnd ) --> cText
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   hb_retc( gtk_combo_box_text_get_active_text( ( GtkComboBoxText * ) hWnd ) );
}

HB_FUNC( CBXSETITEMS ) // ( hWnd, aItems )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );
   int iLen = hb_parinfa( 2, 0 ), i;
   GList * glist = NULL;

   for( i = 0; i < iLen; i++ )
      glist = g_list_append( glist, ( char * ) hb_parvc( 2, i + 1 ) );

   while( glist ) 
   {
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(hWnd), (const gchar*)glist->data);
      glist = glist->next;
   }

   if( glist != NULL )
      g_list_free( glist );
}