#include <hbapi.h>
#include <gtk/gtk.h>

gboolean button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
gboolean motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data);

HB_FUNC(CREATEFOLDER)
{
   GtkWidget *hWnd = gtk_notebook_new();

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

HB_FUNC(FLDSETPROMPTS) // (aPrompts) --> aPagesHandles
{
   GtkWidget *hWnd = (GtkWidget *)hb_parptr(1);
   int iPages = hb_parinfa(2, 0), i;

   hb_reta(iPages);

   for(i = 0; i < iPages; i++)
   {
      GtkWidget *label = gtk_label_new_with_mnemonic(hb_parvc(2, i + 1));
      GtkWidget *fixed;

      gtk_notebook_append_page(GTK_NOTEBOOK(hWnd),
                               fixed = gtk_fixed_new(), label);
      gtk_widget_show(fixed);

      hb_storvptr(fixed, -1, i + 1);
   }
}

HB_FUNC(FLDGOPREV)
{
   GtkNotebook *hWnd = (GtkNotebook *)hb_parptr(1);

   gtk_notebook_prev_page(hWnd);
}

HB_FUNC(FLDGONEXT)
{
   GtkNotebook *hWnd = (GtkNotebook *)hb_parptr(1);

   gtk_notebook_next_page(hWnd);
}

HB_FUNC(FLDSHOWTABS)
{
   GtkNotebook *hWnd = (GtkNotebook *)hb_parptr(1);

   gtk_notebook_set_show_tabs(hWnd, hb_parl(2));
}

HB_FUNC(FLDSHOWBORDER)
{
   GtkNotebook *hWnd = (GtkNotebook *)hb_parptr(1);

   gtk_notebook_set_show_border(hWnd, hb_parl(2));
}

HB_FUNC(FLDSETTABSPOS)
{
   GtkNotebook *hWnd = (GtkNotebook *)hb_parptr(1);

   gtk_notebook_set_tab_pos(hWnd, hb_parnl(2));
}

HB_FUNC(FLDGETPAGE)
{
   GtkNotebook *hWnd = (GtkNotebook *)hb_parptr(1);

   hb_retptr(gtk_notebook_get_current_page(hWnd) + 1);
}