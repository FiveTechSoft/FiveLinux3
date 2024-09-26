#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

void MenuItemSelect( GtkMenuItem * hMenuItem, gpointer user_data );

HB_FUNC( CREATEMENU )
{
   hb_retptr( gtk_menu_bar_new() );
}

HB_FUNC( CREATEPOPUP )
{
   hb_retptr( gtk_menu_new() );
}

HB_FUNC( MENUSHOWPOPUP )
{
   gtk_menu_popup_at_pointer( ( GtkMenu * ) hb_parptr( 1 ), NULL );
}

HB_FUNC( MENUSHOWITEM )
{
   gtk_widget_show( ( GtkWidget * ) hb_parptr( 1 ) );
}

HB_FUNC( SETMENU )
{
   GtkWidget * hVBox = ( GtkWidget * ) g_object_get_data( G_OBJECT( hb_parptr( 1 ) ), "vbox" );
   GtkWidget * hMenu = ( GtkWidget * ) hb_parptr( 2 );

   gtk_box_pack_start( GTK_BOX( hVBox ), hMenu, FALSE, FALSE, 0 );
   gtk_box_reorder_child( GTK_BOX( hVBox ), hMenu, 0 );
}

static void show_menu_accelerators_permanently( void )
{
    GtkSettings * settings = gtk_settings_get_default();
    g_object_set( G_OBJECT( settings ), "gtk-auto-mnemonics", FALSE, NULL );
}

HB_FUNC( APPENDMENU )
{
   GtkWidget * hMenu = ( GtkWidget * ) hb_parptr( 1 );
   GtkWidget * hItem;

   show_menu_accelerators_permanently();

   if( HB_ISCHAR( 2 ) && strcmp(hb_parc( 2 ), "-") == 0 )
   {
      // Create a separator instead of a menu item
      hItem = gtk_separator_menu_item_new();
   }
   else if( HB_ISCHAR( 3 ) )
   {
      hItem = gtk_menu_item_new();
      GtkWidget * box = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 6 );
      GtkWidget * image = gtk_image_new_from_icon_name( hb_parc( 3 ), GTK_ICON_SIZE_MENU );
      GtkWidget * label = gtk_label_new_with_mnemonic( hb_parc( 2 ) );
      // GtkWidget * label = gtk_label_new( hb_parc( 2 ) );
      
      gtk_container_add( GTK_CONTAINER( box ), image );
      gtk_container_add( GTK_CONTAINER( box ), label );
      gtk_container_add( GTK_CONTAINER( hItem ), box );
      gtk_widget_show_all( box );
   }
   else
   {
      if( hb_parclen( 2 ) > 0 )
         hItem = gtk_menu_item_new_with_mnemonic( hb_parc( 2 ) );
      else
      {
         hItem = gtk_menu_item_new();
         gtk_widget_set_sensitive( hItem, FALSE );
      }
   }   

   gtk_container_add( GTK_CONTAINER( hMenu ), hItem );

   // Don't connect the signal for separators
   if( strcmp(hb_parc( 2 ), "-") != 0 )
   {
      g_signal_connect( G_OBJECT( hItem ), "activate",
                        G_CALLBACK( MenuItemSelect ), NULL );
   }

   hb_retptr( hItem );
}

HB_FUNC( ADDPOPUP )
{
   gtk_menu_item_set_submenu( GTK_MENU_ITEM( hb_parptr( 1 ) ),
                              ( GtkWidget * ) hb_parptr( 2 ) );
}

HB_FUNC( MENUSELITEM )
{
   gtk_menu_shell_select_item( ( GtkMenuShell * ) hb_parptr( 1 ), ( GtkWidget * ) hb_parptr( 2 ) );
}