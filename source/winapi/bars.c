#include <hbapi.h>
#include <gtk/gtk.h>

HB_FUNC( CREATEBAR )
{
   GtkWidget * hWnd = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
   GtkWidget * hVBox = GTK_WIDGET(g_object_get_data(G_OBJECT(
                       (GtkWidget *)hb_parptr(1)), "vbox"));
   GtkWidget * hToolBar = gtk_toolbar_new();

   g_object_set_data(G_OBJECT(hWnd), "hToolBar",
                     (gpointer)hToolBar);

   // Empaquetar hWnd en la segunda posición (índice 1) de hVBox
   gtk_box_pack_start(GTK_BOX(hVBox), hWnd, FALSE, TRUE, 0);
   gtk_container_child_set(GTK_CONTAINER(hVBox), hWnd, "position", 1, NULL);

   gtk_widget_set_size_request(hWnd, -1, hb_parnl(3));

   gtk_container_add(GTK_CONTAINER(hWnd), hToolBar);
   
   // Cambio en GTK 3.0: gtk_toolbar_set_style() está obsoleto
   // En su lugar, usamos CSS para estilizar la barra de herramientas
   GtkCssProvider *provider = gtk_css_provider_new();
   gtk_css_provider_load_from_data(provider,
       "toolbar { -gtk-icon-size: large-toolbar; }", -1, NULL);
   gtk_style_context_add_provider(
       gtk_widget_get_style_context(hToolBar),
       GTK_STYLE_PROVIDER(provider),
       GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
   g_object_unref(provider);

   hb_retptr( hWnd );
}