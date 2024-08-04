#include <hbapi.h>
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

HB_FUNC( APPENDMENU )
{
   GtkWidget * hMenu = ( GtkWidget * ) hb_parptr( 1 );
   GtkWidget * hItem;

   if( HB_ISCHAR( 3 ) )
   {
      hItem = gtk_menu_item_new();
      GtkWidget * box = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 6 );
      GtkWidget * image = gtk_image_new_from_icon_name( hb_parc( 3 ), GTK_ICON_SIZE_MENU );
      GtkWidget * label = gtk_label_new( hb_parc( 2 ) );
      
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

   g_signal_connect( G_OBJECT( hItem ), "activate",
                  G_CALLBACK( MenuItemSelect ), NULL );

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