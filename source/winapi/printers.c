#include <hbapi.h>
#include <gtk/gtk.h>
#include <gtk/gtkprint.h>
// #include <gtk/gtkprintoperation.h>
// #include <gtk/gtkprintcontext.h>

void CheckGtkInit( void );

static HB_BOOL bInit = FALSE;

HB_FUNC( CREATEPRINTER )
{
   if(! bInit )
   {
      bInit = TRUE;
      gtk_init(NULL, NULL);
   }

   hb_retptr( gtk_print_operation_new() );
}

HB_FUNC( PRNGETGPC )
{
   hb_retptr( gtk_print_operation_get_print_context( ( GtkPrintOperation * )
             hb_parptr( 1 ) ) );
}

HB_FUNC( PRNSTARTPAGE )
{
   gtk_print_context_begin_page( ( GtkPrintContext * ) hb_parptr( 1 ),
                                 ( const gchar * ) hb_parc( 2 ) );
}

HB_FUNC( PRNENDPAGE )
{
   gtk_print_context_end_page( ( GtkPrintContext * ) hb_parptr( 1 ) );
}

HB_FUNC( PRNMOVETO )
{
   gtk_print_context_move_to( ( GtkPrintContext * ) hb_parptr( 1 ),
                              hb_parnl( 2 ), hb_parnl( 3 ) );
}

HB_FUNC( PRNSAY )
{
   gtk_print_context_show_text( ( GtkPrintContext * ) hb_parptr( 1 ), ( const gchar * ) hb_parc( 2 ) );
}

HB_FUNC( PRNLINE )
{
   gtk_print_context_line_to( ( GtkPrintContext * ) hb_parptr( 1 ), hb_parnl( 3 ),
                              hb_parnl( 2 ) );
   gtk_print_context_stroke( ( GtkPrintContext * ) hb_parptr( 1 ) );
}

HB_FUNC( PRNEND )
{
   GtkPrintOperation * operation = ( GtkPrintOperation * ) hb_parptr( 1 );

   gtk_print_operation_finish( operation );
   gtk_print_operation_print( operation );

   g_object_unref( G_OBJECT( hb_parptr( 2 ) ) );
   g_object_unref( G_OBJECT( operation ) );
}

HB_FUNC( PRNDIALOG )
{
   GtkWidget * dialog;
   gint response;

   CheckGtkInit();

   dialog = gtk_print_unix_dialog_new( ( GtkPrintOperation * ) hb_parptr( 1 ),
                                       ( const gchar * ) hb_parc( 2 ) );
   gtk_window_set_position( GTK_WINDOW( dialog ), GTK_WIN_POS_CENTER );				    
   gtk_widget_show( dialog );
   response = gtk_dialog_run( GTK_DIALOG( dialog ) );

   hb_retl( response!= GTK_RESPONSE_CANCEL );
}

HB_FUNC( PRNGETWIDTH )
{
   GtkPrintSettings * settings = gtk_print_operation_get_print_settings(
                                 ( GtkPrintOperation * ) hb_parptr( 1 ) );
   gdouble width;

   gtk_print_settings_get_paper_width( settings, GTK_UNIT_MM, &width );
   hb_retnl( width );
}

HB_FUNC( PRNGETHEIGHT )
{
   GtkPrintSettings * settings = gtk_print_operation_get_print_settings(
                                 ( GtkPrintOperation * ) hb_parptr( 1 ) );
   gdouble height;

   gtk_print_settings_get_paper_height( settings, GTK_UNIT_MM, &height );
   hb_retnl( height );
}