#include <hbapi.h>
#include <gtk/gtk.h>

void ScrollBarChanged(GtkAdjustment * adjustment, gpointer user_data);

HB_FUNC(CREATESCROLL) // lVertical
{
   GtkWidget * hWnd;

   if (hb_parl(1))
      hWnd = gtk_scrollbar_new(GTK_ORIENTATION_VERTICAL, NULL);
   else
      hWnd = gtk_scrollbar_new(GTK_ORIENTATION_HORIZONTAL, NULL);

   // GTK_UPDATE_DISCONTINUOUS is deprecated, using GTK_SCROLL_JUMP instead
   gtk_range_set_slider_size_fixed(GTK_RANGE(hWnd), TRUE);

   hb_retptr(hWnd);
}

HB_FUNC(SCRLSETRANGE)
{
   GtkAdjustment * adj = gtk_adjustment_new(1, hb_parnl(2), hb_parnl(3), 1, hb_parnl(4), 
                                            hb_parnl(4)); // same value for page down and up

   gtk_range_set_adjustment(GTK_RANGE(hb_parptr(1)),
                            GTK_ADJUSTMENT(adj));

   g_signal_connect(adj, "value-changed", G_CALLBACK(ScrollBarChanged),
                    (gpointer)hb_parptr(1));
}

HB_FUNC(SCRLGETRANGE)
{
   GtkAdjustment * adjust = gtk_range_get_adjustment(GTK_RANGE(hb_parptr(1)));

   hb_reta(2);
   hb_storvnl(gtk_adjustment_get_lower(adjust), -1, 1);
   hb_storvnl(gtk_adjustment_get_upper(adjust), -1, 2); 
}

HB_FUNC(SCRLSETVALUE)
{
   GtkAdjustment * adj = gtk_range_get_adjustment(GTK_RANGE(hb_parptr(1)));

   gtk_adjustment_set_value(adj, hb_parnl(2));
}

HB_FUNC(SCRLGETVALUE)
{
   GtkAdjustment * adj = gtk_range_get_adjustment(GTK_RANGE(hb_parptr(1)));

   hb_retnl(gtk_adjustment_get_value(adj));
}