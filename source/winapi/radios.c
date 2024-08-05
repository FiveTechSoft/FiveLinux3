#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

void RadioButtonClick(GtkToggleButton *hWnd, gpointer data);

HB_FUNC(CREATERADIO)
{
   GSList *radios_group = (GSList *)hb_parptr(2);
   GtkWidget *hWnd = gtk_radio_button_new_with_label(radios_group, "");
   gtk_button_set_label(GTK_BUTTON(hWnd), hb_parc(1));
   
   guint radio_key = gtk_label_get_mnemonic_keyval(GTK_LABEL(gtk_bin_get_child(GTK_BIN(hWnd))));
   GtkAccelGroup *accel_group = gtk_accel_group_new();

   gtk_widget_add_accelerator(hWnd, "clicked", accel_group, radio_key,
                              GDK_MOD1_MASK, (GtkAccelFlags)0);
   radios_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(hWnd));

   hb_storptr(radios_group, 2);

   g_signal_connect(G_OBJECT(hWnd), "toggled",
                    G_CALLBACK(RadioButtonClick), NULL);

   hb_retptr(hWnd);
}

HB_FUNC(RADCHECKED)
{
   hb_retl(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(hb_parptr(1))));
}

HB_FUNC(RADSETCHECK)
{
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(hb_parptr(1)), hb_parl(2));
}