#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

gint ClickEvent(GtkWidget *hWnd, GdkEventButton *event);
gboolean LostFocusEvent(GtkWidget *widget, GdkEventFocus *event, gpointer user_data);
gboolean button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
gboolean motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data);

HB_FUNC(CREATEBUTTON)
{
    GtkWidget *hWnd;

    if (HB_ISCHAR(2))
        hWnd = gtk_button_new_from_icon_name(hb_parc(2), GTK_ICON_SIZE_BUTTON);
    else
        hWnd = gtk_button_new_with_mnemonic(hb_parc(1));

    g_signal_connect(G_OBJECT(hWnd), "clicked",
                     G_CALLBACK(ClickEvent), NULL);

    g_signal_connect(G_OBJECT(hWnd), "focus-out-event",
                     G_CALLBACK(LostFocusEvent), NULL);

    g_signal_connect(G_OBJECT(hWnd), "button-press-event",
                     G_CALLBACK(button_press_event), NULL);

    g_signal_connect(G_OBJECT(hWnd), "motion-notify-event",
                     G_CALLBACK(motion_notify_event), NULL);

    gtk_widget_add_events(hWnd, GDK_EXPOSURE_MASK
                               | GDK_LEAVE_NOTIFY_MASK
                               | GDK_BUTTON_PRESS_MASK
                               | GDK_POINTER_MOTION_MASK
                               | GDK_POINTER_MOTION_HINT_MASK);

    hb_retptr(hWnd);
}

void MsgInfo(char *msg);

#include <hbapi.h>
#include <gtk/gtk.h>

HB_FUNC(CREATEBTN)
{
    GtkWidget *hToolBar = GTK_WIDGET(g_object_get_data(G_OBJECT(hb_parptr(1)), "hToolBar"));
    GtkToolItem *toolItem = NULL;
    GtkWidget *button = NULL;

    // Agregar separador si se solicita
    if (hb_parl(4))
    {
        toolItem = gtk_separator_tool_item_new();
        if (toolItem) {
            gtk_toolbar_insert(GTK_TOOLBAR(hToolBar), toolItem, -1);
        }
    }

    button = gtk_button_new_with_label(hb_parc(2));

    // Añadir imagen si se proporciona
    if (HB_ISCHAR(5) && g_file_test(hb_parc(5), G_FILE_TEST_EXISTS))
    {
        GtkWidget *image = gtk_image_new_from_file(hb_parc(5));
        if (image) {
            gtk_button_set_image(GTK_BUTTON(button), image);
            gtk_widget_set_tooltip_text(button, "Private");
        }
    }
    else if (HB_ISCHAR(3))
    {
        GtkWidget *image = gtk_image_new_from_icon_name(hb_parc(3), GTK_ICON_SIZE_BUTTON);
        if (image) {
            gtk_button_set_image(GTK_BUTTON(button), image);
        }
    }

    // Crear GtkToolItem y añadir el botón
    toolItem = gtk_tool_item_new();
    gtk_container_add(GTK_CONTAINER(toolItem), button);

    // Añadir el GtkToolItem a la barra de herramientas
    gtk_toolbar_insert(GTK_TOOLBAR(hToolBar), toolItem, -1);

    // Conectar la señal "clicked"
    if (GTK_IS_BUTTON(button)) {
        g_signal_connect(G_OBJECT(button), "clicked",
                         G_CALLBACK(ClickEvent), NULL);
    }

    hb_retptr(button);
}

HB_FUNC(BTNSETTEXT)
{
    gtk_button_set_label(GTK_BUTTON(hb_parptr(1)), hb_parc(2));
}

HB_FUNC(BTNGETTEXT)
{
    hb_retc((char *)gtk_button_get_label(GTK_BUTTON(hb_parptr(1))));
}