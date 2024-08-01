#include <hbapi.h>
#include <gtk/gtk.h>

HB_FUNC( CREATEMSGBAR )
{
   GtkWidget *hWnd = gtk_statusbar_new();
   GtkWidget *hVBox = (GtkWidget *) g_object_get_data(G_OBJECT(hb_parptr(1)), "vbox");

   gtk_box_pack_start(GTK_BOX(hVBox), hWnd, FALSE, FALSE, 0);

   hb_retptr(hWnd);
}

HB_FUNC( SETMSGBARTEXT )
{
   gtk_statusbar_push(GTK_STATUSBAR(hb_parptr(1)), hb_parnl(2), (const gchar *) hb_parc(3));
}