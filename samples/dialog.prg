#include "FiveLinux.ch"

function Main()

   local oDlg, cTest := PadR( "Hello world", 20 ), oCbx, cValue := "Two", lValue := .T.

   DEFINE DIALOG oDlg TITLE "Dialog"

   @ 1, 1 SAY "Say" OF oDlg

   @ 3, 1 GET cTest OF oDlg

   @ 3, 20 BUTTON "Value" OF oDlg ACTION MsgInfo( cTest )

   @ 7, 1 COMBOBOX cValue ITEMS { "One", "Two", "Three" } OF oDlg

   @ 7, 20 BUTTON "Value" OF oDlg ACTION MsgInfo( cValue )

   @ 11, 1 CHECKBOX lValue OF oDlg 

   @ 11, 20 BUTTON "Value" OF oDlg ACTION MsgInfo( lValue )

   ACTIVATE DIALOG oDlg CENTERED

return nil   