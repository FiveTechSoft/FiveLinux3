// Using ListBoxes to perform simple selections

#include "FiveLinux.ch"

//----------------------------------------------------------------//

function Main()

   MsgInfo( MsgSelect( { "one", "two", "three" }, "two" ) )

return nil

//----------------------------------------------------------------//

function MsgSelect( aItems, cValue, cTitle )

   local oDlg

   DEFAULT cTitle := "Please, select"

   DEFINE DIALOG oDlg TITLE cTitle SIZE 280, 265

   @  1, 2 LISTBOX cValue ITEMS aItems SIZE 181, 110 OF oDlg

   @ 13, 2 BUTTON "_OK" OF oDlg SIZE 80, 25 ;
     ACTION oDlg:End()

   @ 13, 12 BUTTON "_Cancel" OF oDlg SIZE 80, 25 ;
     ACTION ( cValue := nil, oDlg:End() )

   ACTIVATE DIALOG oDlg CENTERED

return cValue

//----------------------------------------------------------------//
