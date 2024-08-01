#include <hbapi.h>
#include <gtk/gtk.h>

void SetWndMain(GtkWidget *hWnd);
gboolean DeleteEvent(GtkWidget *hWnd, GdkEvent *event, gpointer data);
gboolean ButtonPressEvent(GtkWidget *hWnd, GdkEventButton *event, gpointer data);

static GtkWidget *hWndParent = NULL;

HB_FUNC(CREATEDIALOG)
{
    GtkWidget *hWnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *hVBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *hFixed = gtk_fixed_new();

    SetWndMain(hWnd);

    gtk_container_add(GTK_CONTAINER(hWnd), hVBox);
    gtk_box_pack_start(GTK_BOX(hVBox), hFixed, TRUE, TRUE, 0);

    g_object_set_data(G_OBJECT(hWnd), "vbox", (gpointer)hVBox);
    g_object_set_data(G_OBJECT(hWnd), "fixed", (gpointer)hFixed);

    gtk_widget_realize(hWnd);
    gtk_window_set_decorated(GTK_WINDOW(hWnd), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(hWnd), 100, 100);

    g_signal_connect(G_OBJECT(hWnd), "delete-event",
                     G_CALLBACK(DeleteEvent), NULL);

    g_signal_connect(G_OBJECT(hWnd), "button-press-event",
                     G_CALLBACK(ButtonPressEvent), NULL);

    gtk_widget_add_events(hWnd, GDK_CONFIGURE |
                                GDK_EXPOSURE_MASK |
                                GDK_LEAVE_NOTIFY_MASK |
                                GDK_BUTTON_PRESS_MASK |
                                GDK_POINTER_MOTION_MASK |
                                GDK_POINTER_MOTION_HINT_MASK);

    hb_retptr(hWnd);
}

HB_FUNC(SETMODAL)
{
    if (hb_pcount() == 2)
        hWndParent = (GtkWidget *)hb_parptr(2);

    if (hWndParent)
        gtk_window_set_transient_for(GTK_WINDOW(hb_parptr(1)),
                                     GTK_WINDOW(hWndParent));

    gtk_window_set_modal(GTK_WINDOW(hb_parptr(1)), TRUE);
}