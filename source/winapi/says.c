#include <hbapi.h>
#include <gtk/gtk.h>

static gboolean button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static gboolean motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data);

HB_FUNC(CREATESAY)
{
   GtkWidget *label = gtk_label_new(hb_parc(1));
   GtkWidget *event_box = gtk_event_box_new();

   g_signal_connect(G_OBJECT(event_box), "button-press-event",
                    G_CALLBACK(button_press_event), NULL);

   g_signal_connect(G_OBJECT(event_box), "motion-notify-event",
                    G_CALLBACK(motion_notify_event), NULL);

   gtk_widget_add_events(event_box, GDK_EXPOSURE_MASK
                        | GDK_LEAVE_NOTIFY_MASK
                        | GDK_BUTTON_PRESS_MASK
                        | GDK_POINTER_MOTION_MASK
                        | GDK_POINTER_MOTION_HINT_MASK);

   gtk_container_add(GTK_CONTAINER(event_box), label);
   gtk_widget_show(label);

   hb_retptr(event_box);
}

HB_FUNC(SAYSETTEXT)
{
   GtkWidget *event_box = (GtkWidget *)hb_parptr(1);
   GList *children = gtk_container_get_children(GTK_CONTAINER(event_box));
   GtkWidget *label = GTK_WIDGET(children->data);

   gtk_label_set_text(GTK_LABEL(label), hb_parc(2));
   g_list_free(children);
}

HB_FUNC(SAYGETTEXT)
{
   GtkWidget *event_box = (GtkWidget *)hb_parptr(1);
   GList *children = gtk_container_get_children(GTK_CONTAINER(event_box));
   GtkWidget *label = GTK_WIDGET(children->data);

   hb_retc(gtk_label_get_text(GTK_LABEL(label)));
   g_list_free(children);
}

HB_FUNC(SAYSETJUSTIFY)
{
   GtkWidget *event_box = (GtkWidget *)hb_parptr(1);
   GList *children = gtk_container_get_children(GTK_CONTAINER(event_box));
   GtkWidget *label = GTK_WIDGET(children->data);

   gtk_label_set_justify(GTK_LABEL(label), (GtkJustification)hb_parnl(2));
   g_list_free(children);
}

HB_FUNC(SAYSETANGLE)
{
   GtkWidget *event_box = (GtkWidget *)hb_parptr(1);
   GList *children = gtk_container_get_children(GTK_CONTAINER(event_box));
   GtkWidget *label = GTK_WIDGET(children->data);

   gtk_label_set_angle(GTK_LABEL(label), hb_parnl(2));
   g_list_free(children);
}

static gboolean button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
   // Implement your button press event handling here
   return FALSE;
}

static gboolean motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data)
{
   // Implement your motion notify event handling here
   return FALSE;
}