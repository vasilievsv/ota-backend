#include "http_server.h"

bool sendFile(string path,string type,const shared_ptr< Session > session )
{
    ifstream stream( path, ifstream::in );
    
    if ( stream.is_open() )
    {
        const string body = string( istreambuf_iterator< char >( stream ), istreambuf_iterator< char >( ) );
        
        const multimap< string, string > headers
        {
            { "Content-Type", type },
            { "Content-Length", to_string( body.length( ) ) },
            { "Connection", "keep-alive" } 
        };
        
        session->yield( OK,body, headers,[&](const std::shared_ptr< Session > session) { 

          //   session->yield( OK, {{ "Connection", "keep-alive" } });

        });

       
    }
    else
    {
        printf("send_file: not_found\n");
        session->close( NOT_FOUND );
    };
    return true;
}