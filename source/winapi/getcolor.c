#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

void CheckGtkInit(void);

HB_FUNC(CHOOSECOLOR) // cTitle, nColor
{
    GtkWidget *hWndParent = NULL;
    GtkWidget *hWnd;
    GdkRGBA rgba = {0};

    CheckGtkInit();

    GList *toplevels = gtk_window_list_toplevels();
    if (toplevels && g_list_last(toplevels))
        hWndParent = GTK_WIDGET(g_list_last(toplevels)->data);

    hWnd = gtk_color_chooser_dialog_new(HB_ISCHAR(1) ? hb_parc(1) : "Select a color", GTK_WINDOW(hWndParent));
    if (!hWnd) {
        hb_retnd(-1);  // Indicar error
        return;
    }

    if (HB_ISNUM(2))
    {
       gdouble color_double = hb_parnd(2);
       guint32 color = (guint32)color_double;
       rgba.red = ((color & 0xFF) / 255.0);
       rgba.green = (((color >> 8) & 0xFF) / 255.0);
       rgba.blue = (((color >> 16) & 0xFF) / 255.0);
       rgba.alpha = 1.0;

       gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(hWnd), &rgba);
    }

    if (hWndParent)
        gtk_window_set_transient_for(GTK_WINDOW(hWnd), GTK_WINDOW(hWndParent));

    gtk_window_set_modal(GTK_WINDOW(hWnd), TRUE);

    gint result = gtk_dialog_run(GTK_DIALOG(hWnd));
    if (result == GTK_RESPONSE_OK) {
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(hWnd), &rgba);
        hb_retnd(rgba.red * 65535 + rgba.green * 65535 * 256 + rgba.blue * 65535 * 65536);
    } else {
        hb_retnd(-1);  // Indicar que no se seleccionó color
    }

    gtk_widget_destroy(hWnd);
}