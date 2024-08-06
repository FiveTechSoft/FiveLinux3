#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

static HB_BOOL bInit = FALSE;

HB_FUNC( CREATEPRINTER )
{
   if (!bInit)
   {
      bInit = TRUE;
      gtk_init(NULL, NULL);
   }

   hb_retptr( gtk_print_operation_new() );
}

HB_FUNC( PRNGETGPC )
{
   // Esta función ya no es necesaria con GtkPrint
   hb_retptr( NULL );
}

static int current_page = 0;
static const char* current_page_name = NULL;

HB_FUNC( PRNSTARTPAGE )
{
   GtkPrintOperation* operation = (GtkPrintOperation*)hb_parptr(1);
   const char* page_name = hb_parc(2);

   current_page++;
   current_page_name = page_name;

   // Establecer el número de páginas si aún no se ha hecho
   if (gtk_print_operation_get_n_pages_to_print(operation) < current_page) {
      gtk_print_operation_set_n_pages(operation, current_page);
   }
}

HB_FUNC( PRNENDPAGE )
{
   // No necesitamos hacer nada aquí en GtkPrint
   // La página termina automáticamente cuando se llama a draw_page para la siguiente página
}

// Modifica la función draw_page para usar estas variables
static void draw_page(GtkPrintOperation *operation,
                      GtkPrintContext *context,
                      gint page_nr,
                      gpointer user_data)
{
   cairo_t *cr = gtk_print_context_get_cairo_context(context);

   if (page_nr < current_page) {
      // Dibujar el contenido de la página
      // Aquí es donde implementarías la lógica para dibujar cada página
      // Puedes usar current_page_name si necesitas el nombre de la página

      // Ejemplo:
      cairo_move_to(cr, 72, 72);
      cairo_show_text(cr, current_page_name ? current_page_name : "Unnamed Page");
   }
}

HB_FUNC( PRNSTARTPRINT )
{
   GtkPrintOperation *operation = (GtkPrintOperation *)hb_parptr(1);
   g_signal_connect(operation, "draw-page", G_CALLBACK(draw_page), NULL);
}

HB_FUNC( PRNRUN )
{
   GtkPrintOperation *operation = (GtkPrintOperation *)hb_parptr(1);
   GtkPrintOperationResult result;

   result = gtk_print_operation_run(operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG,
                                    NULL, NULL);

   hb_retni(result);
}

HB_FUNC( PRNMOVETO )
{
   // Esta función necesitaría ser implementada dentro de draw_page
   // usando cairo_move_to
}

HB_FUNC( PRNSAY )
{
   // Esta función necesitaría ser implementada dentro de draw_page
   // usando cairo_show_text
}

HB_FUNC( PRNLINE )
{
   // Esta función necesitaría ser implementada dentro de draw_page
   // usando cairo_line_to y cairo_stroke
}

HB_FUNC( PRNEND )
{
   GtkPrintOperation *operation = (GtkPrintOperation *)hb_parptr(1);
   g_object_unref(G_OBJECT(operation));
}

HB_FUNC( PRNDIALOG )
{
   // Esta función ya no es necesaria, ya que gtk_print_operation_run
   // muestra el diálogo de impresión automáticamente
   hb_retl(TRUE);
}

HB_FUNC( PRNGETWIDTH )
{
   GtkPrintOperation *operation = (GtkPrintOperation *)hb_parptr(1);
   GtkPageSetup *page_setup = gtk_print_operation_get_default_page_setup(operation);
   gdouble width = gtk_page_setup_get_paper_width(page_setup, GTK_UNIT_MM);
   hb_retnl(width);
}

HB_FUNC( PRNGETHEIGHT )
{
   GtkPrintOperation *operation = (GtkPrintOperation *)hb_parptr(1);
   GtkPageSetup *page_setup = gtk_print_operation_get_default_page_setup(operation);
   gdouble height = gtk_page_setup_get_paper_height(page_setup, GTK_UNIT_MM);
   hb_retnd(height);  // Usa hb_retnd para retornar un double en lugar de un long
}