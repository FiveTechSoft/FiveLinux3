#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

gboolean ClickEvent(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
gboolean LostFocusEvent(GtkWidget *widget, GdkEventFocus *event, gpointer user_data);
gboolean button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
gboolean motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data);

HB_FUNC(CREATECHECKBOX)
{
   GtkWidget *hWnd = gtk_check_button_new_with_label(hb_parc(1));

   g_signal_connect(G_OBJECT(hWnd), "clicked", G_CALLBACK(ClickEvent), NULL);
   g_signal_connect(G_OBJECT(hWnd), "focus-out-event", G_CALLBACK(LostFocusEvent), NULL);
   g_signal_connect(G_OBJECT(hWnd), "button-press-event", G_CALLBACK(button_press_event), NULL);
   // g_signal_connect(G_OBJECT(hWnd), "motion-notify-event", G_CALLBACK(motion_notify_event), NULL);

   gtk_widget_add_events(hWnd, GDK_EXPOSURE_MASK
                              | GDK_LEAVE_NOTIFY_MASK
                              | GDK_BUTTON_PRESS_MASK
                              | GDK_POINTER_MOTION_MASK
                              | GDK_POINTER_MOTION_HINT_MASK);

   hb_retptr(hWnd);
}

HB_FUNC(CBXSETCHECK)
{
   GtkWidget *hWnd = (GtkWidget *)hb_parptr(1);
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(hWnd), hb_parl(2));
}

HB_FUNC(CBXGETCHECK)
{
   GtkWidget *hWnd = (GtkWidget *)hb_parptr(1);
   hb_retl(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hWnd)));
}