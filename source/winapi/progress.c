#include <msgs.h>
#include <hbapi.h>
#include <hbvm.h>
#include <gtk/gtk.h>

HB_FUNC( CREATEPROGRESS )
{
   GtkWidget * hWnd = gtk_progress_bar_new();

   hb_retptr( hWnd );
}

HB_FUNC( PROSET )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR( hWnd ), ( gdouble ) hb_parnd( 2 ) );
}

HB_FUNC( PROSETTEXT )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   gtk_progress_bar_set_text( GTK_PROGRESS_BAR( hWnd ), hb_parc( 2 ) );
}

HB_FUNC( PROSETVERTICAL )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );

   gtk_progress_bar_set_orientation( GTK_PROGRESS_BAR( hWnd ), GTK_ORIENTATION_VERTICAL );
   gtk_progress_bar_set_inverted( GTK_PROGRESS_BAR( hWnd ), TRUE );
}