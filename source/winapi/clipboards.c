#include <hbapi.h>
#include <gtk/gtk.h>

HB_FUNC( CREATECLIPBOARD )
{
   GtkClipboard * hClipboard = gtk_clipboard_get_default(gdk_display_get_default());

   hb_retptr( hClipboard );
}

HB_FUNC( CLPSETTEXT )
{
   GtkClipboard * hClipboard = ( GtkClipboard * ) hb_parptr( 1 );

   gtk_clipboard_set_text( hClipboard, hb_parc( 2 ), -1 );
}

HB_FUNC( CLPGETTEXT )
{
   GtkClipboard * hClipboard = ( GtkClipboard * ) hb_parptr( 1 );

   gchar *text = gtk_clipboard_wait_for_text( hClipboard );
   if (text != NULL) {
      hb_retc( text );
      g_free(text);
   } else {
      hb_retc( "" );
   }
}

HB_FUNC( CLPCLEAR )
{
   GtkClipboard * hClipboard = ( GtkClipboard * ) hb_parptr( 1 );

   gtk_clipboard_clear( hClipboard );
   gtk_clipboard_set_text( hClipboard, "", -1 );
}