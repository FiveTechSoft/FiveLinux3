#include <hbapi.h>
#include <sys/types.h>
#include <sys/stat.h>

HB_FUNC( SETEXECUTABLE )
{
   chmod( hb_parc( 1 ), 0744 );
}

HB_FUNC( ISAPP64 ) // Check if our app is 64 bits
{
   hb_retl( ( sizeof( void * ) == 8 ) );
}