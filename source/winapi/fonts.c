#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

HB_FUNC( CREATEFONT )
{
   PangoFontDescription * hFont = pango_font_description_from_string( hb_parc( 1 ) );

   hb_retptr( hFont );
}

HB_FUNC( DESTROYFONT )
{
   pango_font_description_free( ( PangoFontDescription * ) hb_parptr( 1 ) );
}