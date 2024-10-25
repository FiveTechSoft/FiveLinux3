#include "FiveLinux.ch"

static oFontDefault

CLASS TFont

   DATA   hFont  // An internal handle of the font

   METHOD New( cFontName ) INLINE ::hFont := CreateFont( cFontName ), Self

   METHOD End() INLINE DestroyFont( ::hFont ), hFont := nil

ENDCLASS

function GetFontDefault()

return oFontDefault

function SetFontDefault( oFont )

   local oOldFontDefault := oFontDefault

   oFontDefault = oFont

return oOldFontDefault
