#include "FiveLinux.ch"

function Main()

    local oKimi := TKimi():New()

    oKimi:Send( "cual es la capital de francia ?" )

    ? oKimi:GetValue()

return nil    