#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

void CheckGtkInit(void);

static void ButtonOk(GtkWidget *widget, gpointer data)
{
    GtkColorChooser *colorchooser = GTK_COLOR_CHOOSER(data);
    GdkRGBA color;

    gtk_color_chooser_get_rgba(colorchooser, &color);
    hb_retnd(color.red * 65535 + color.green * 65535 * 256 + color.blue * 65535 * 65536);

    gtk_widget_destroy(GTK_WIDGET(data));
    gtk_main_quit();
}

static void ButtonCancel(GtkWidget *widget, gpointer data)
{
    gtk_widget_destroy(GTK_WIDGET(data));
    gtk_main_quit();
}

static gboolean DeleteEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_widget_destroy(GTK_WIDGET(data));
    gtk_main_quit();
    return TRUE;
}

HB_FUNC(CHOOSECOLOR) // cTitle, nColor
{
    GtkWidget *hWndParent = NULL;
    GtkWidget *hWnd;

    CheckGtkInit();

    GList *toplevels = gtk_window_list_toplevels();
    if (toplevels)
        hWndParent = g_list_last(toplevels)->data;

    hWnd = gtk_color_chooser_dialog_new(HB_ISCHAR(1) ? hb_parc(1) : "Select a color", GTK_WINDOW(hWndParent));

    if (HB_ISNUM(2))
    {
       gdouble color_double = hb_parnd(2);
       guint32 color = (guint32)color_double;
       GdkRGBA rgba;
       rgba.red = ((color & 0xFF) / 255.0);
       rgba.green = (((color >> 8) & 0xFF) / 255.0);
       rgba.blue = (((color >> 16) & 0xFF) / 255.0);
       rgba.alpha = 1.0;

        gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(hWnd), &rgba);
    }

    g_signal_connect(hWnd, "response", G_CALLBACK(ButtonOk), hWnd);
    g_signal_connect(hWnd, "delete-event", G_CALLBACK(DeleteEvent), hWnd);

    if (hWndParent)
        gtk_window_set_transient_for(GTK_WINDOW(hWnd), GTK_WINDOW(hWndParent));

    gtk_window_set_modal(GTK_WINDOW(hWnd), TRUE);

    gtk_widget_show(hWnd);

    gtk_main();
}