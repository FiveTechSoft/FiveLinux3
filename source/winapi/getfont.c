#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

void CheckGtkInit(void);

HB_FUNC(CHOOSEFONT) // cTitle, cFontName
{
   GtkWidget *hWndParent = NULL;
   GtkWidget *hWnd;
   gint result;
   char *selected_font = NULL;

   CheckGtkInit();

   GList *toplevels = gtk_window_list_toplevels();
   if (toplevels && g_list_last(toplevels))
      hWndParent = GTK_WIDGET(g_list_last(toplevels)->data);

   hWnd = gtk_font_chooser_dialog_new(
      HB_ISCHAR(1) ? hb_parc(1) : "Select a font",
      GTK_WINDOW(hWndParent));

   if (!hWnd) {
      hb_retc(NULL);
      return;
   }

   if (hb_pcount() > 1 && HB_ISCHAR(2))
      gtk_font_chooser_set_font(GTK_FONT_CHOOSER(hWnd), hb_parc(2));

   gtk_window_set_modal(GTK_WINDOW(hWnd), TRUE);

   result = gtk_dialog_run(GTK_DIALOG(hWnd));

   if (result == GTK_RESPONSE_OK) {
      selected_font = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(hWnd));
      if (selected_font) {
         hb_retc(selected_font);
         g_free(selected_font);
      } else {
         hb_retc(NULL);
      }
   } else {
      hb_retc(NULL);
   }

   gtk_widget_destroy(hWnd);
}