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

gdouble get_paper_width(GtkPrintOperation *operation) {
    gdouble width = 0.0;
    GtkPageSetup *page_setup = gtk_print_operation_get_default_page_setup(operation);

    // Método 1: Usar la configuración de página predeterminada si está disponible
    if (page_setup != NULL) {
        width = gtk_page_setup_get_paper_width(page_setup, GTK_UNIT_MM);
        return width;
    }

    // Método 2: Crear una nueva configuración de página
    page_setup = gtk_page_setup_new();
    if (page_setup != NULL) {
        width = gtk_page_setup_get_paper_width(page_setup, GTK_UNIT_MM);
        g_object_unref(page_setup);
        if (width > 0) {
            return width;
        }
    }

    // Método 3: Usar un tamaño de papel predeterminado (A4)
    GtkPaperSize *paper_size = gtk_paper_size_new(GTK_PAPER_NAME_A4);
    if (paper_size != NULL) {
        width = gtk_paper_size_get_width(paper_size, GTK_UNIT_MM);
        gtk_paper_size_free(paper_size);
        if (width > 0) {
            return width;
        }
    }

    // Método 4: Intentar obtener el ancho de la configuración de impresión
    GtkPrintSettings *settings = gtk_print_operation_get_print_settings(operation);
    if (settings != NULL) {
        paper_size = gtk_print_settings_get_paper_size(settings);
        if (paper_size != NULL) {
            width = gtk_paper_size_get_width(paper_size, GTK_UNIT_MM);
            gtk_paper_size_free(paper_size);
            if (width > 0) {
                return width;
            }
        }
    }

    // Si todos los métodos fallan, devolver un ancho predeterminado (ancho A4 en mm)
    return 210.0;
}

HB_FUNC( PRNGETWIDTH )
{
    GtkPrintOperation *operation = (GtkPrintOperation *)hb_parptr(1);
    
    if (operation == NULL) {
        hb_retni(-1);  // Devolver -1 para indicar un error
        return;
    }

    gdouble width = get_paper_width(operation);
    hb_retnd(width);
}

gdouble get_paper_height(GtkPrintOperation *operation) {
    gdouble height = 0.0;
    GtkPageSetup *page_setup = gtk_print_operation_get_default_page_setup(operation);

    // Method 1: Use the default page setup if available
    if (page_setup != NULL) {
        height = gtk_page_setup_get_paper_height(page_setup, GTK_UNIT_MM);
        return height;
    }

    // Method 2: Create a new page setup
    page_setup = gtk_page_setup_new();
    if (page_setup != NULL) {
        height = gtk_page_setup_get_paper_height(page_setup, GTK_UNIT_MM);
        g_object_unref(page_setup);
        if (height > 0) {
            return height;
        }
    }

    // Method 3: Use a default paper size (A4)
    GtkPaperSize *paper_size = gtk_paper_size_new(GTK_PAPER_NAME_A4);
    if (paper_size != NULL) {
        height = gtk_paper_size_get_height(paper_size, GTK_UNIT_MM);
        gtk_paper_size_free(paper_size);
        if (height > 0) {
            return height;
        }
    }

    // Method 4: Try to get height from print settings
    GtkPrintSettings *settings = gtk_print_operation_get_print_settings(operation);
    if (settings != NULL) {
        paper_size = gtk_print_settings_get_paper_size(settings);
        if (paper_size != NULL) {
            height = gtk_paper_size_get_height(paper_size, GTK_UNIT_MM);
            gtk_paper_size_free(paper_size);
            if (height > 0) {
                return height;
            }
        }
    }

    // If all methods fail, return a default height (A4 height in mm)
    return 297.0;
}

HB_FUNC( PRNGETHEIGHT )
{
    GtkPrintOperation *operation = (GtkPrintOperation *)hb_parptr(1);
    
    if (operation == NULL) {
        hb_retni(-1);  // Return -1 to indicate an error
        return;
    }

    gdouble height = get_paper_height(operation);
    hb_retnd(height);
}