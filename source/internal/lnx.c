#include <msgs.h>
#include <hbapi.h>
#include <hbvm.h>
#include <gtk/gtk.h>

int MsgInfo( char * szMsg );

PHB_SYMB pFLH = 0;

static GtkWidget * hWndMain = 0;

static HB_BOOL bInit = FALSE;
// static HB_BOOL bRunning = FALSE;

static GtkBuilder * hResources = 0;

void CheckGtkInit( void )
{
   if( ! bInit )
   {
      int argc = 0;
      char **argv = NULL;
      gtk_init(&argc, &argv);
      bInit = TRUE;
   }
}

HB_FUNC( __GTKINIT )
{
   CheckGtkInit();

   if( pFLH == 0 )
       pFLH = hb_dynsymSymbol( hb_dynsymFindName( "_FLH" ) );
}

void SetWndMain( GtkWidget * hWnd )
{
   if( ! hWndMain )
      hWndMain = hWnd;
}

HB_FUNC( WINRUN )
{
   // if( ! bRunning )
   // {
   //    bRunning = TRUE;
      gtk_main();
   // }
}

gint ConfigureEvent( GtkWidget * hWnd, GdkEventConfigure * event )
{
   /*  if (!gc) {

                 *  This code is executed on the first configure event only,
                 *  i.e. when the window for the drawing area is created.
                 *  We create a new GC and allocate the colors we will use.

                gc = gdk_gc_new(widget->window);
                init_colors(widget);  } */

   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_SIZE );                    // nMsg
   hb_vmPushLong( ( HB_ULONG ) event->width );     // nWParam
   hb_vmPushLong( ( HB_ULONG ) event->height );    // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( hWnd ), "WP" ) );
   hb_vmFunction( 4 );

   // FALSE means = do the default action. TRUE = don't do anything
   return FALSE;  // keep this here or it does not properly resize menu and statusbar
}

gint DeleteEvent( GtkWidget * hWnd, gpointer data )
{
   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_CLOSE );          // nMsg
   hb_vmPushLong( ( HB_ULONG ) data );    // nWParam
   hb_vmPushLong( ( HB_ULONG ) NULL );    // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( hWnd ), "WP" ) );
   hb_vmFunction( 4 );

   if( hWnd == hWndMain )
   {
      gtk_main_quit();
   }

   return TRUE;
}

gboolean motion_notify_event( GtkWidget * hWnd, GdkEventMotion * event )
{
   int x = event->x, y = event->y;
   GdkModifierType state;

   if( gtk_widget_get_parent( hWnd ) )
   {
      GdkWindow *parent_window = gtk_widget_get_window(gtk_widget_get_parent(hWnd));
      gdk_window_get_pointer(parent_window, &x, &y, &state);
   }

   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_MOUSEMOVE );    // nMsg
   hb_vmPushLong( ( HB_ULONG ) y );  // nWParam
   hb_vmPushLong( ( HB_ULONG ) x );  // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( hWnd ), "WP" ) );
   hb_vmFunction( 4 );

   return TRUE;
}

void RadioButtonClick( GtkToggleButton * hWnd, gpointer data )
{
   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_CLICK );          // nMsg
   hb_vmPushLong( ( HB_ULONG ) NULL );    // nWParam
   hb_vmPushLong( ( HB_ULONG ) NULL );    // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( hWnd ), "WP" ) );
   hb_vmFunction( 4 );
}

void ScrollBarChanged( GtkAdjustment * adjustment, gpointer user_data )
{
   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_SCROLL );            // nMsg
   hb_vmPushLong( ( HB_ULONG ) NULL );       // nWParam
   hb_vmPushLong( ( HB_ULONG ) NULL );       // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( user_data ), "WP" ) );
   hb_vmFunction( 4 );
}

static glong timer = 0;

static gboolean TimerEvent( gpointer data )
{
   static PHB_SYMB symTimerEvent = NULL;

   if( symTimerEvent == NULL )
      symTimerEvent = hb_dynsymSymbol( hb_dynsymFindName( "TIMEREVENT" ) );

   hb_vmPushSymbol( symTimerEvent );
   hb_vmPushNil();
   hb_vmPushLong( timer );
   hb_vmFunction( 1 );
   return TRUE;
}

HB_FUNC( SETTIMER )
{
   timer = ( glong ) g_timeout_add( hb_parnl( 1 ), TimerEvent, NULL );
   hb_retnll( timer );
}

HB_FUNC( KILLTIMER )
{
   hb_retl( g_source_remove( ( guint ) hb_parnll( 1 ) ) );
}

HB_FUNC( SYSQUIT )
{
   GtkWidget *widget = (GtkWidget *) hb_parptr(1);

   if (widget!= hWndMain) {
      gtk_widget_destroy(widget);
   }

   gtk_main_quit();
}

gint ClickEvent(GtkWidget *hWnd, GdkEventButton *event);
gboolean LostFocusEvent( GtkWidget *hWnd, GdkEventFocus *event, gpointer user_data );

HB_FUNC( LOADBUTTON )
{
   GtkWidget *hWnd = gtk_builder_get_object( hResources, hb_parc( 1 ) );

   g_signal_connect( hWnd, "clicked", G_CALLBACK( ClickEvent ), NULL );

   g_signal_connect( hWnd, "focus-out-event", G_CALLBACK( LostFocusEvent ), NULL );

   hb_retptr( hWnd );
}

void CbxChangeEvent( GtkWidget * hWnd, GtkWidget * hItem, gpointer user_data )
{
   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_CHANGE );         // nMsg
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( hItem ), "index" ) );    // nWParam
   hb_vmPushLong( ( HB_ULONG ) NULL );    // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( user_data ), "WP" ) );
   hb_vmFunction( 4 );
}

gint GotFocusEvent( GtkWidget * hWnd, GdkEventFocus * event, gpointer user_data )
{
   GtkWidget * hScrolls = g_object_get_data( G_OBJECT( hWnd ), "hScrolls" );

   if( hScrolls != NULL )
      hWnd = hScrolls;

   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_GOTFOCUS );      // nMsg
   hb_vmPushLong( ( HB_ULONG ) event );   // nWParam
   hb_vmPushLong( ( HB_ULONG ) NULL );    // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( hWnd ), "WP" ) );
   hb_vmFunction( 4 );

   return FALSE;
}

gboolean KeyPressEvent( GtkWidget * hWnd, GdkEventKey * event )
{
   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_KEYDOWN );        // nMsg
   hb_vmPushLong( ( HB_ULONG ) event->keyval );   // nWParam
   hb_vmPushLong( ( HB_ULONG ) NULL );    // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( hWnd ), "WP" ) );
   hb_vmFunction( 4 );

   if( event->keyval == 65289 || ( event->keyval == 65289 && ( event->state & GDK_SHIFT_MASK ) ) ) // K_TAB
      return FALSE;
   else
      return TRUE;
}

gint LostFocusEvent(GtkWidget *hWnd, GdkEventFocus *event, gpointer user_data)
{
    GtkWidget *hScrolls = g_object_get_data(G_OBJECT(hWnd), "hScrolls");

    if (hScrolls!= NULL)
        hWnd = hScrolls;

    hb_vmPushSymbol(pFLH);
    hb_vmPushNil();
    hb_vmPushLong(WM_LOSTFOCUS);      // nMsg
    hb_vmPushLong((HB_ULONG)event);   // nWParam
    hb_vmPushLong((HB_ULONG)NULL);    // nLParam
    hb_vmPushLong((HB_ULONG)g_object_get_data(G_OBJECT(hWnd), "WP"));
    hb_vmFunction(4);

    return FALSE;
}

gboolean ButtonPressEvent(GtkWidget *hWnd, GdkEventButton *event)
{
   hb_vmPushSymbol(pFLH);
   hb_vmPushNil();

   if (event->button == 1)
   {
      if (event->type == GDK_2BUTTON_PRESS)
         hb_vmPushLong(WM_LDBLCLICK);
      else
         hb_vmPushLong(WM_LBUTTONDOWN); // nMsg
   }
   else // 3
   {
      if (event->type == GDK_2BUTTON_PRESS)
         hb_vmPushLong(WM_RDBLCLICK);
      else
         hb_vmPushLong(WM_RBUTTONDOWN); // nMsg
   }

   hb_vmPushLong((HB_ULONG)event->y); // nWParam
   hb_vmPushLong((HB_ULONG)event->x); // nLParam
   hb_vmPushLong((HB_ULONG)g_object_get_data(G_OBJECT(hWnd), "WP"));
   hb_vmFunction(4);

   return FALSE;
}

gint ClickEvent(GtkWidget *hWnd, GdkEventButton *event)
{
    hb_vmPushSymbol(pFLH);
    hb_vmPushNil();
    hb_vmPushLong(WM_CLICK);          // nMsg
    hb_vmPushLong((HB_ULONG)event);   // nWParam
    hb_vmPushLong((HB_ULONG)NULL);    // nLParam
    hb_vmPushLong((HB_ULONG)g_object_get_data(G_OBJECT(hWnd), "WP"));
    hb_vmFunction(4);

    return TRUE;
}

gboolean button_press_event(GtkWidget *hWnd, GdkEventButton *event)
{
   if (event->button == 1) 
   {   
      hb_vmPushSymbol(pFLH);
      hb_vmPushNil();
      hb_vmPushLong(WM_LBUTTONDOWN);        // nMsg
      hb_vmPushLong((HB_ULONG) event->y); // nWParam
      hb_vmPushLong((HB_ULONG) event->x); // nLParam
      hb_vmPushLong((HB_ULONG) g_object_get_data(G_OBJECT(hWnd), "WP"));
      hb_vmFunction(4);
   }
   
   return!HB_ISNIL(-1); // FALSE invokes default behavior
}

void LbxChangeEvent(GtkListBox *hWnd, GtkListBoxRow *hItem, gpointer user_data)
{
   hb_vmPushSymbol(pFLH);
   hb_vmPushNil();
   hb_vmPushLong(WM_CHANGE);         // nMsg
   hb_vmPushLong((HB_ULONG)g_object_get_data(G_OBJECT(hItem), "index"));    // nWParam
   hb_vmPushLong((HB_ULONG)NULL);    // nLParam
   hb_vmPushLong((HB_ULONG)g_object_get_data(G_OBJECT(user_data), "WP"));
   hb_vmFunction(4);
}

void MenuItemSelect(GtkMenuItem *hMenuItem, gpointer user_data)
{
    hb_vmPushSymbol(pFLH);
    hb_vmPushNil();
    hb_vmPushLong( WM_MENUCMD );    // nMsg
    hb_vmPushPointer( hMenuItem );  // nWParam
    hb_vmPushLong(0);               // nLParam
    hb_vmPushLong((HB_ULONG)g_object_get_data(G_OBJECT(hMenuItem), "WP"));
    hb_vmFunction(4);
}

gboolean PaintEvent( GtkWidget * hWnd, GdkEventExpose * event )
{
   hb_vmPushSymbol( pFLH );
   hb_vmPushNil();
   hb_vmPushLong( WM_PAINT );             // nMsg
   hb_vmPushLong( ( HB_ULONG ) event );   // nWParam
   hb_vmPushLong( ( HB_ULONG ) NULL );    // nLParam
   hb_vmPushLong( ( HB_ULONG ) g_object_get_data( G_OBJECT( hWnd ), "WP" ) );
   hb_vmFunction( 4 );

   return FALSE;
}


HB_FUNC( LOADDIALOG )
{
    // Inicializa GtkBuilder y carga el archivo de interfaz
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, hb_parc( 1), NULL);

    // Obtén el widget del archivo cargado
    GtkWidget *hWnd = GTK_WIDGET(gtk_builder_get_object(builder, hb_parc(1)));

    // Asegúrate de que el widget se ha cargado correctamente
    if (hWnd == NULL) {
        g_warning("Widget %s no encontrado en el archivo de interfaz.", hb_parc(1));
        hb_retptr(NULL);
        g_object_unref(builder);
        return;
    }

    // Usa el widget como antes
    SetWndMain(hWnd);

    g_signal_connect(hWnd, "delete-event",
                     G_CALLBACK(DeleteEvent), NULL);

    g_signal_connect(hWnd, "button-press-event",
                     G_CALLBACK(ButtonPressEvent), NULL);

    gtk_widget_add_events(hWnd, GDK_CONFIGURE |
                               GDK_EXPOSURE_MASK |
                               GDK_LEAVE_NOTIFY_MASK |
                               GDK_BUTTON_PRESS_MASK |
                               GDK_POINTER_MOTION_MASK |
                               GDK_POINTER_MOTION_HINT_MASK);

    hb_retptr(hWnd);

    // Libera la memoria utilizada por GtkBuilder
    g_object_unref(builder);
}