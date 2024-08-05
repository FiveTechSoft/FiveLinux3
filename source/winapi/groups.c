#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

HB_FUNC( CREATEGROUP )
{
   GtkWidget * hWnd = gtk_frame_new( hb_parc( 1 ) );

   hb_retptr( hWnd );
}

HB_FUNC( GRPSETTEXT )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   gtk_frame_set_label( GTK_FRAME( hWnd ), hb_parc( 2 ) );
}

HB_FUNC( GRPGETTEXT )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   hb_retc( gtk_frame_get_label( GTK_FRAME( hWnd ) ) );
}
