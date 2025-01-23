#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

#define GTK_TYPE_BROWSE            (gtk_browse_get_type ())
#define GTK_BROWSE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_BROWSE, GtkBrowse))
#define GTK_BROWSE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_BROWSE, GtkBrowseClass))
#define GTK_IS_BROWSE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_BROWSE))
#define GTK_IS_BROWSE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_BROWSE))
#define GTK_BROWSE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_BROWSE, GtkBrowseClass))

typedef struct _GtkBrowse        GtkBrowse;
typedef struct _GtkBrowseClass   GtkBrowseClass;

struct _GtkBrowse
{
   GtkWidget parent_instance;

   // new data
   PangoLayout *layout;
   GtkAdjustment *hadjustment;
   GtkAdjustment *vadjustment;
};

struct _GtkBrowseClass
{
  GtkWidgetClass parent_class;

  void (* set_scroll_adjustments) (GtkBrowse     *browse,
                                   GtkAdjustment *hadjustment,
                                   GtkAdjustment *vadjustment);
};

gboolean PaintEvent( GtkWidget * hWnd, cairo_t *cr );
gboolean button_press_event(GtkWidget *hWnd, GdkEventButton *event);
gboolean KeyPressEvent( GtkWidget * hWnd, GdkEventKey * event );
gboolean scroll_event( GtkWidget *widget, GdkEventScroll *event );

G_DEFINE_TYPE (GtkBrowse, gtk_browse, GTK_TYPE_WIDGET)

static void gtk_browse_set_scroll_adjustments (GtkBrowse *browse,
                                   GtkAdjustment *hadjustment,
                                   GtkAdjustment *vadjustment)
{
  // Implementation here
}

static void gtk_browse_realize(GtkWidget *widget)
{
    GdkWindowAttr attributes;
    gint attributes_mask;

    gtk_widget_set_realized(widget, TRUE);

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.x = allocation.x;
    attributes.y = allocation.y;
    attributes.width = allocation.width;
    attributes.height = allocation.height;
    attributes.wclass = GDK_INPUT_OUTPUT;
    attributes.visual = gtk_widget_get_visual(widget);
    attributes.event_mask = gtk_widget_get_events(widget) |
        GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK |
        GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK |
        GDK_POINTER_MOTION_HINT_MASK | GDK_KEY_PRESS_MASK;

    attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

    GdkWindow *window = gdk_window_new(gtk_widget_get_parent_window(widget),
                                       &attributes, attributes_mask);
    gtk_widget_set_window(widget, window);
    gdk_window_set_user_data(window, widget);

    GtkStyleContext *style_context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(style_context, GTK_STYLE_CLASS_BACKGROUND);
}

static void gtk_browse_class_init (GtkBrowseClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  widget_class->realize = gtk_browse_realize;
  widget_class->draw    = PaintEvent;         // paint
  widget_class->key_press_event = KeyPressEvent;      // pulsacion tecla
  widget_class->button_press_event = button_press_event; // click ratón
  widget_class->scroll_event = scroll_event; // mouse wheel
  klass->set_scroll_adjustments = gtk_browse_set_scroll_adjustments;

  g_signal_new ("set-scroll-adjustments",
                G_TYPE_FROM_CLASS (klass),
                G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                G_STRUCT_OFFSET (GtkBrowseClass, set_scroll_adjustments),
                NULL, NULL,
                NULL,
                G_TYPE_NONE, 2,
                GTK_TYPE_ADJUSTMENT,
                GTK_TYPE_ADJUSTMENT);
}

static void
gtk_browse_init (GtkBrowse *browse)
{
  browse->layout = gtk_widget_create_pango_layout (GTK_WIDGET (browse), "");
  browse->hadjustment = gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  browse->vadjustment = gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  gtk_widget_set_has_window (GTK_WIDGET (browse), TRUE);
}

GtkWidget * gtk_browse_new (void)
{
  return g_object_new (GTK_TYPE_BROWSE, NULL);
}

// The rest of the functions (BRWDRAWHEADERS, BRWDRAWLINES, BRWDRAWCELL, etc.) 
// will need to be updated to use GTK 3.0 drawing functions. 
// Here's an example of how BRWDRAWHEADERS might be updated:

void brw_draw_headers (GtkWidget *widget, cairo_t *cr, gchar **headers, gint *col_sizes, gint col_pos)
{
  GtkBrowse *browse = GTK_BROWSE (widget);
  gint width = gtk_widget_get_allocated_width (widget);
  gint i, left = 0, right;
  gint cols = g_strv_length (headers);

  for (i = col_pos - 1; i < cols; i++)
  {
    if (left < width)
    {
      right = col_sizes[i];

      if (left + right > width)
        right = width - left;

      if (i + 1 == cols)
        right = width - left;

      gtk_render_background (gtk_widget_get_style_context (widget),
                             cr, left, 0, right, 21);

      pango_layout_set_text (browse->layout, headers[i], -1);

      gtk_render_layout (gtk_widget_get_style_context (widget),
                         cr, left + 6, 2, browse->layout);

      left += col_sizes[i] - 1;
    }
  }
}

// Similar updates would be needed for the other drawing functions.

gboolean motion_notify_event( GtkWidget * hWnd, GdkEventMotion * event );
gboolean ButtonPressEvent(GtkWidget *hWnd, GdkEventButton *event);

HB_FUNC( CREATEBROWSE )
{
   GtkBrowse * hWnd = ( GtkBrowse * ) gtk_browse_new();

   g_signal_connect( hWnd, "key-press-event",
                     G_CALLBACK (KeyPressEvent), NULL );

   g_signal_connect( hWnd, "button-press-event",
                     G_CALLBACK (ButtonPressEvent), NULL );

   g_signal_connect( hWnd, "motion-notify-event",
                     G_CALLBACK (motion_notify_event), NULL );

   gtk_widget_set_events( GTK_WIDGET(hWnd), GDK_EXPOSURE_MASK
             | GDK_LEAVE_NOTIFY_MASK
             | GDK_BUTTON_PRESS_MASK
             | GDK_POINTER_MOTION_MASK
             | GDK_POINTER_MOTION_HINT_MASK 
             | GDK_SCROLL_MASK 
             | GDK_KEY_PRESS_MASK );

   gtk_widget_set_can_focus(GTK_WIDGET(hWnd), TRUE);
   gtk_widget_grab_focus(GTK_WIDGET(hWnd));          

   hb_retptr( hWnd );
}

/*
HB_FUNC( BRWDRAWHEADERS ) // ( hWnd, pEvent, aHeaders, aColSizes, nColPos )
{
   GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );
   cairo_t *cr;
   int iCols = hb_parinfa( 3, 0 ), i, iLeft = 0, iRight;

   cr = gdk_cairo_create(gtk_widget_get_window(hWnd));

   for( i = hb_parnl( 5 ) - 1; i < iCols; i++ )
   {
      if( iLeft < gtk_widget_get_allocated_width (hWnd) )
      {
         iRight = hb_parvnl( 4, i + 1 );

         if( iLeft + iRight > gtk_widget_get_allocated_width (hWnd) )
            iRight = gtk_widget_get_allocated_width (hWnd) - iLeft;

         if( i + 1 == iCols )
            iRight = gtk_widget_get_allocated_width (hWnd) - iLeft;

         cairo_rectangle (cr, iLeft, 0, iRight, 21);
         cairo_set_source_rgb (cr, 1, 1, 1); // Blanco
         cairo_fill (cr);

         pango_layout_set_text( GTK_BROWSE( hWnd )->layout,
                                hb_parvc( 3, i + 1 ), -1 );

         cairo_set_source_rgb(cr, 0, 0, 0); // Negro
         cairo_move_to (cr, iLeft + 6, 2);
         pango_cairo_show_layout (cr, GTK_BROWSE( hWnd )->layout);

         iLeft += hb_parvnl( 4, i + 1 ) - 1;
      }
   }

   cairo_destroy (cr);
   gtk_widget_queue_draw(hWnd);
} */

HB_FUNC( BRWDRAWHEADERS ) // ( hWnd, pEvent, aHeaders, aColSizes, nColPos )
{
    GtkWidget * hWnd = ( GtkWidget * ) hb_parptr( 1 );
    cairo_t *cr;
    int iCols = hb_parinfa( 3, 0 ), i, iLeft = 0, iRight;

    GdkWindow *window = gtk_widget_get_window(hWnd);
    GdkDrawingContext *context;
    cairo_region_t *region;

    region = cairo_region_create();
    context = gdk_window_begin_draw_frame(window, region);
    cr = gdk_drawing_context_get_cairo_context(context);

    for( i = hb_parnl( 5 ) - 1; i < iCols; i++ )
    {
      if( iLeft < gtk_widget_get_allocated_width (hWnd) )
      {
         iRight = hb_parvnl( 4, i + 1 );

         if( iLeft + iRight > gtk_widget_get_allocated_width (hWnd) )
            iRight = gtk_widget_get_allocated_width (hWnd) - iLeft;

         if( i + 1 == iCols )
            iRight = gtk_widget_get_allocated_width (hWnd) - iLeft;

         cairo_rectangle (cr, iLeft, 0, iRight, 21);
         cairo_set_source_rgb (cr, 1, 1, 1); // Blanco
         cairo_fill (cr);

         pango_layout_set_text( GTK_BROWSE( hWnd )->layout,
                                hb_parvc( 3, i + 1 ), -1 );

         cairo_set_source_rgb(cr, 0, 0, 0); // Negro
         cairo_move_to (cr, iLeft + 6, 2);
         pango_cairo_show_layout (cr, GTK_BROWSE( hWnd )->layout);

         iLeft += hb_parvnl( 4, i + 1 ) - 1;
      }
    }

    gdk_window_end_draw_frame(window, context);
    cairo_region_destroy(region);

    gtk_widget_queue_draw(hWnd);
}

HB_FUNC( BRWROWCOUNT ) // ( hWnd )
{
   GtkWidget *widget = (GtkWidget *)hb_parptr(1);
   GtkAllocation allocation;
   gtk_widget_get_allocation(widget, &allocation);
   hb_retnl(allocation.height / 20);
}

HB_FUNC( BRWDRAWCELL ) // ( hWnd, nRow, nCol, cText, nWidth, lSelected, nRGBColorBackGround )
{
   GtkWidget * hWnd = (GtkWidget *) hb_parptr(1);
   GdkRectangle cell_rect = { hb_parnl(3), hb_parnl(2),
                              hb_parnl(5) - 1, 18 }; // 19
   PangoAttrList * attrs;
   GtkStyleContext *context = gtk_widget_get_style_context(hWnd);
   
   GdkWindow *window = gtk_widget_get_window(hWnd);
   GdkDrawingContext *draw_context;
   cairo_region_t *region;
   cairo_t *cr;

   GtkAllocation allocation;
   gtk_widget_get_allocation(hWnd, &allocation);

   // Usar el área completa del widget para la región
   GdkRectangle widget_rect = { 0, 0, allocation.width, allocation.height };
   
   // Crear el contexto de dibujo usando el área completa del widget
   region = cairo_region_create_rectangle(&widget_rect);
   draw_context = gdk_window_begin_draw_frame(window, region);
   cr = gdk_drawing_context_get_cairo_context(draw_context);

   // Recortar al área de la celda
   cairo_rectangle(cr, cell_rect.x, cell_rect.y, cell_rect.width, cell_rect.height);
   cairo_clip(cr);

   // Dibujar el fondo
   guint32 nColorFondo = -1;
   if (hb_parl(6)) {
      nColorFondo = 3381759;
   } else if (hb_pcount() > 6 && !HB_ISNIL(7)) {
      nColorFondo = (guint32) hb_parnl(7);
   }

   if (nColorFondo >= 0)
   {
      GdkRGBA color;
      guint32 rgb_value = nColorFondo;
      color.red  = ((rgb_value >> 16) & 0xFF) / 255.0;
      color.green = ((rgb_value >> 8) & 0xFF) / 255.0;
      color.blue   = (rgb_value & 0xFF) / 255.0;
      color.alpha = 1.0;
      gdk_cairo_set_source_rgba(cr, &color);
      cairo_paint(cr);
   }
   else
   {
      gtk_style_context_save(context);
      if (hb_parl(6))
      {
         gtk_style_context_set_state(context, gtk_style_context_get_state(context) | GTK_STATE_FLAG_SELECTED);
      }
      else
      {
         gtk_style_context_set_state(context, gtk_style_context_get_state(context) & ~GTK_STATE_FLAG_SELECTED);
      }
      gtk_render_background(context, cr, cell_rect.x, cell_rect.y, cell_rect.width, cell_rect.height);
      gtk_style_context_restore(context);
   }

   // Dibujar el texto
   PangoLayout *layout = ((GtkBrowse *)hWnd)->layout;
   pango_layout_set_text(layout, hb_parc(4), -1);

   if (hb_pcount() > 7 && !HB_ISNIL(8))
    {
      guint32 color = (guint32) hb_parnl(8);
      GdkRGBA rgba;
      rgba.red   = ((color >> 16) & 0xFF) / 255.0;
      rgba.green = ((color >> 8) & 0xFF) / 255.0;
      rgba.blue  = (color & 0xFF) / 255.0;
      rgba.alpha = 1.0;

      attrs = pango_attr_list_new();
      pango_attr_list_insert(attrs, pango_attr_foreground_new(rgba.red * 65535, rgba.green * 65535, rgba.blue * 65535));
      pango_layout_set_attributes(layout, attrs);
   }
   else
      gdk_cairo_set_source_rgba(cr, &(GdkRGBA){0, 0, 0, 1}); // Color de texto negro por defecto
   
   gtk_render_layout(context, cr, cell_rect.x, cell_rect.y, layout);

   if (hb_pcount() > 7 && !HB_ISNIL(8))
   {
      pango_attr_list_unref(attrs);
      pango_layout_set_attributes(layout, NULL);
   }

   // Finalizar el contexto de dibujo
   gdk_window_end_draw_frame(window, draw_context);
   cairo_region_destroy(region);
}

HB_FUNC( BRWDRAWLINES ) // ( hWnd, aColSizes, nColPos )
{
   GtkWidget * hWnd = (GtkWidget *) hb_parptr(1);
   int iColSizes = hb_parinfa(2, 0), i, iLeft = 0;
   GtkAllocation allocation;
   GtkStyleContext *context;
   cairo_t *cr;
   GdkWindow *window;
   GdkDrawingContext *draw_context;
   cairo_region_t *region;

   gtk_widget_get_allocation(hWnd, &allocation);
   context = gtk_widget_get_style_context(hWnd);
   window = gtk_widget_get_window(hWnd);

   // Crear la región para todo el widget
   region = cairo_region_create_rectangle(&(cairo_rectangle_int_t){0, 0, allocation.width, allocation.height});
   
   // Comenzar el frame de dibujo
   draw_context = gdk_window_begin_draw_frame(window, region);
   cr = gdk_drawing_context_get_cairo_context(draw_context);

   // Set line color
   GdkRGBA color;
   gtk_style_context_get_color(context, gtk_style_context_get_state(context), &color);
   gdk_cairo_set_source_rgba(cr, &color);

   // Draw horizontal lines
   for (i = 2; i < allocation.height / 20; i++)
   {
      cairo_move_to(cr, 0, i * 20);
      cairo_line_to(cr, allocation.width, i * 20);
      cairo_stroke(cr);
   }

   // Draw bottom line
   cairo_move_to(cr, 0, allocation.height - 1);
   cairo_line_to(cr, allocation.width, allocation.height - 1);
   cairo_stroke(cr);

   // Draw left line
   cairo_move_to(cr, 0, 20);
   cairo_line_to(cr, 0, allocation.height);
   cairo_stroke(cr);

   // Draw vertical lines for columns
   for (i = hb_parnl(3); i < iColSizes && iLeft < allocation.width; i++)
   {
      iLeft += hb_parvnl(2, i) - 1;
      cairo_move_to(cr, iLeft, 20);
      cairo_line_to(cr, iLeft, allocation.height);
      cairo_stroke(cr);
   }

   // Draw right line
   cairo_move_to(cr, allocation.width - 1, 20);
   cairo_line_to(cr, allocation.width - 1, allocation.height);
   cairo_stroke(cr);

   // Finalizar el frame de dibujo
   gdk_window_end_draw_frame(window, draw_context);

   // Liberar la región
   cairo_region_destroy(region);
}

HB_FUNC( BRWSCROLLUP )
{
   GtkWidget * hWnd = (GtkWidget *) hb_parptr(1);
   GtkAllocation allocation;
   cairo_t *cr;
   cairo_surface_t *surface;
   GdkWindow *window;
   GdkDrawingContext *draw_context;
   cairo_region_t *region;

   gtk_widget_get_allocation(hWnd, &allocation);
   window = gtk_widget_get_window(hWnd);

   // Crear la región para todo el widget
   region = cairo_region_create_rectangle(&(cairo_rectangle_int_t){0, 0, allocation.width, allocation.height});
   
   // Comenzar el frame de dibujo
   draw_context = gdk_window_begin_draw_frame(window, region);
   cr = gdk_drawing_context_get_cairo_context(draw_context);

   // Create a surface from the window
   surface = cairo_surface_create_similar(cairo_get_target(cr),
                                          CAIRO_CONTENT_COLOR,
                                          allocation.width,
                                          allocation.height);

   // Copy the window contents to the surface
   cairo_t *cr_surface = cairo_create(surface);
   cairo_set_source_surface(cr_surface, cairo_get_target(cr), 0, 0);
   cairo_paint(cr_surface);
   cairo_destroy(cr_surface);

   // Draw the surface on the window with offset
   cairo_set_source_surface(cr, surface, 0, -20);
   cairo_rectangle(cr, 0, 20, allocation.width, allocation.height - 41);
   cairo_fill(cr);

   cairo_surface_destroy(surface);

   // Finalizar el frame de dibujo
   gdk_window_end_draw_frame(window, draw_context);

   // Liberar la región
   cairo_region_destroy(region);
}

HB_FUNC( BRWSCROLLDOWN )
{
   GtkWidget * hWnd = (GtkWidget *) hb_parptr(1);
   GtkAllocation allocation;
   cairo_t *cr;
   cairo_surface_t *surface;
   GdkWindow *window;
   GdkDrawingContext *draw_context;
   cairo_region_t *region;

   gtk_widget_get_allocation(hWnd, &allocation);
   window = gtk_widget_get_window(hWnd);

   // Crear la región para todo el widget
   region = cairo_region_create_rectangle(&(cairo_rectangle_int_t){0, 0, allocation.width, allocation.height});
   
   // Comenzar el frame de dibujo
   draw_context = gdk_window_begin_draw_frame(window, region);
   cr = gdk_drawing_context_get_cairo_context(draw_context);

   // Create a surface from the window
   surface = cairo_surface_create_similar(cairo_get_target(cr),
                                          CAIRO_CONTENT_COLOR,
                                          allocation.width,
                                          allocation.height);

   // Copy the window contents to the surface
   cairo_t *cr_surface = cairo_create(surface);
   cairo_set_source_surface(cr_surface, cairo_get_target(cr), 0, 0);
   cairo_paint(cr_surface);
   cairo_destroy(cr_surface);

   // Draw the surface on the window with offset
   cairo_set_source_surface(cr, surface, 0, 20);
   cairo_rectangle(cr, 0, 40, allocation.width, allocation.height - 41);
   cairo_fill(cr);

   cairo_surface_destroy(surface);

   // Finalizar el frame de dibujo
   gdk_window_end_draw_frame(window, draw_context);

   // Liberar la región
   cairo_region_destroy(region);
}
