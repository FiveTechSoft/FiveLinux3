/*------------------------------------------------------------------------------
  TKimi – unified client for Kimi (Moonshot) API
  Usage:
    oKimi := TKimi():New()
    ? oKimi:Send( "What is the capital of France?" )
    ? oKimi:Send( "photo.jpg", "Describe this image" )
    ? oKimi:Send( "https://my.cdn/img.jpg", "What do you see?" )
    oKimi:Send( "Tell me a joke", {|c| QQOut(c)} )
------------------------------------------------------------------------------*/

#include "FiveLinux.ch"
#include "hbcurl.ch"

CLASS TKimi
    DATA cKey      INIT ""
    DATA cModel    INIT "moonshot-v1-8k"
    DATA cPrompt
    DATA cResponse
    DATA cUrl
    DATA hCurl
    DATA nError    INIT 0
    DATA nHttpCode INIT 0
    DATA nTemperature INIT 0.5

    METHOD New( cKey, cModel )
    METHOD Send( xInput, uOptional )
    METHOD End()
    METHOD GetValue()

    /* Internal helpers */
    METHOD BuildTextPayload( cPrompt )
    METHOD BuildImagePayload( cType, cImage, cPrompt )
ENDCLASS

METHOD New( cKey, cModel ) CLASS TKimi
    IF Empty( cKey )
        ::cKey := GetEnv( "KIMI_API_KEY" )
        IF Empty( ::cKey )
            MsgAlert( "KIMI_API_KEY not found" )
        ENDIF
    ELSE
        ::cKey := cKey
    ENDIF

    IF ! Empty( cModel )
        ::cModel := cModel
    ENDIF

    ::cUrl := "https://api.moonshot.ai/v1/chat/completions"
    ::hCurl := curl_easy_init()
RETURN Self

METHOD End() CLASS TKimi
    IF ::hCurl != NIL
        curl_easy_cleanup( ::hCurl )
        ::hCurl := NIL
    ENDIF
RETURN NIL

METHOD GetValue() CLASS TKimi
    LOCAL hResponse, uValue
    hb_jsonDecode( ::cResponse, @hResponse )
    TRY
        uValue := hResponse[ "choices" ][ 1 ][ "message" ][ "content" ]
    CATCH
        uValue := hResponse[ "error" ][ "message" ]
    END
RETURN uValue

/*------------------------------------------------------------------------------
  Single method that covers:
    - Plain text
    - Local image file
    - Image URL
    - Streaming
------------------------------------------------------------------------------*/
METHOD Send( xInput, uOptional ) CLASS TKimi
    LOCAL cPrompt
    LOCAL bStream  := NIL
    LOCAL cBodyJson
    LOCAL aHeaders := { "Content-Type: application/json", ;
                        "Authorization: Bearer " + ::cKey }

    /* 1. Detect request type */
    IF ValType( xInput ) == "C"
        /* Streaming */
        IF ValType( uOptional ) == "B"
            bStream := uOptional
            cPrompt := xInput
            cBodyJson := hb_jsonEncode( { "model"      => ::cModel, ;
                                          "messages"   => { { "role"=>"user", "content"=>cPrompt } }, ;
                                          "stream"     => .T., ;
                                          "temperature"=> ::nTemperature } )
        /* Local image file */
        ELSEIF File( xInput )
            cPrompt := IIF( ValType( uOptional ) == "C", uOptional, ;
                            "What is in this image?" )
            cBodyJson := hb_jsonEncode( { "model"    => ::cModel, ;
                                          "messages" => { { "role"=>"user", ;
                                                            "content"=> { { "type"=>"text", "text"=>cPrompt }, ;
                                                                          { "type"=>"image_url", ;
                                                                            "image_url"=> { "url"=> "data:image/jpeg;base64," + ;
                                                                                             hb_base64Encode( MemoRead( xInput ) ) } } } } }, ;
                                          "temperature"=> ::nTemperature } )
        /* Image URL */
        ELSEIF Left( Lower( xInput ), 4 ) == "http"
            cPrompt := IIF( ValType( uOptional ) == "C", uOptional, ;
                            "What is in this image?" )
            cBodyJson := hb_jsonEncode( { "model"    => ::cModel, ;
                                          "messages" => { { "role"=>"user", ;
                                                            "content"=> { { "type"=>"text", "text"=>cPrompt }, ;
                                                                          { "type"=>"image_url", ;
                                                                            "image_url"=> { "url"=> xInput } } } } }, ;
                                          "temperature"=> ::nTemperature } )
        /* Plain text */
        ELSE
            cPrompt := xInput
            cBodyJson := hb_jsonEncode( { "model"      => ::cModel, ;
                                          "messages"   => { { "role"=>"user", "content"=>cPrompt } }, ;
                                          "temperature"=> ::nTemperature } )
        ENDIF
    ENDIF

    /* 2–4. (curl setup & perform) – unchanged */
    curl_easy_reset( ::hCurl )
    curl_easy_setopt( ::hCurl, HB_CURLOPT_URL,      ::cUrl )
    curl_easy_setopt( ::hCurl, HB_CURLOPT_POST,     .T. )
    curl_easy_setopt( ::hCurl, HB_CURLOPT_HTTPHEADER, aHeaders )
    curl_easy_setopt( ::hCurl, HB_CURLOPT_SSL_VERIFYPEER, .F. )
    curl_easy_setopt( ::hCurl, HB_CURLOPT_POSTFIELDS, cBodyJson )

    IF bStream != NIL
        curl_easy_setopt( ::hCurl, HB_CURLOPT_WRITEFUNCTION, bStream )
    ELSE
        curl_easy_setopt( ::hCurl, HB_CURLOPT_DL_BUFF_SETUP )
    ENDIF

    ::nError := curl_easy_perform( ::hCurl )
    curl_easy_getinfo( ::hCurl, HB_CURLINFO_RESPONSE_CODE, @::nHttpCode )

    IF ::nError == HB_CURLE_OK
        ::cResponse := IIF( bStream == NIL, ;
                            curl_easy_dl_buff_get( ::hCurl ), ;
                            "" )
    ELSE
        ::cResponse := "Error code " + Str( ::nError )
    ENDIF
RETURN ::cResponse

/*------------------------------------------------------------------------------
  Build JSON for plain-text requests
------------------------------------------------------------------------------*/
METHOD BuildTextPayload( cPrompt ) CLASS TKimi
RETURN hb_jsonEncode( { ;
        "model"      => ::cModel, ;
        "messages"   => { { "role"=>"user", "content"=>cPrompt } }, ;
        "temperature"=> ::nTemperature } )

/*------------------------------------------------------------------------------
  Build JSON for image requests
  cType = "file" | "url"
------------------------------------------------------------------------------*/
METHOD BuildImagePayload( cType, cImage, cPrompt ) CLASS TKimi
    LOCAL aContent := { { "type"=>"text", "text"=>cPrompt } }
    LOCAL cBase64

    IF cType == "file"
        cBase64 := hb_base64Encode( MemoRead( cImage ) )
        AAdd( aContent, { "type"=>"image_url", ;
                           "image_url"=> { "url"=>"data:image/jpeg;base64,"+cBase64 } } )
    ELSE
        AAdd( aContent, { "type"=>"image_url", ;
                           "image_url"=> { "url"=>cImage } } )
    ENDIF
RETURN hb_jsonEncode( { ;
        "model"    => ::cModel, ;
        "messages" => { { "role"=>"user", "content"=>aContent } }, ;
        "temperature"=> ::nTemperature } )
