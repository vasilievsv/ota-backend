#include "http_server.h"


void get_handler_codux_boot( const shared_ptr< Session > session )
{
    const auto request = session->get_request( );

    cout<<"get_handler_enter: "<<endl;

    // grab html-body from codux'a load
    const string body = R"( 
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <link rel="icon" href="/favicon.ico" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <meta name="theme-color" content="#000000" />
    <meta name="description" content="Web site created using create-react-app" />
    <link rel="apple-touch-icon" href="/logo192.png" />
    <link rel="manifest" href="/manifest.json" />
    <title>React App</title>
  <script defer src="/static/js/bundle.js"></script></head>
  <body>
    <noscript>You need to enable JavaScript to run this app.</noscript>
    <div id="root"></div>
  </body>
</html>)";
        
    const multimap< string, string > headers
    {
        { "Content-Type", "text/html" },
        { "Content-Length", ::to_string( body.length( ) ) }
    };
    
    session->close( OK, body, headers );
}