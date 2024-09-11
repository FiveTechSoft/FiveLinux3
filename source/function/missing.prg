#include "FiveLinux.ch"

function XBrowse()

   MsgInfo( "Xbrowse not implemented yet for FiveLinux" )

return nil

function TXBrows()

   MsgInfo( "TXBrows() not implemented yet for FiveLinux" )

return nil

function ExcelObj()

   MsgInfo( "ExcelObj() not implemented yet for FiveLinux" )

return nil

function StringToLiteral( u )

return cValToChar( u )

function RGB( nRed, nGreen, nBlue )

   nRed   := Min( 255, Max( 0, Int( nRed   ) ) )
   nGreen := Min( 255, Max( 0, Int( nGreen ) ) )
   nBlue  := Min( 255, Max( 0, Int( nBlue  ) ) )

return ( nRed * 65536 ) + ( nGreen * 256 ) + nBlue