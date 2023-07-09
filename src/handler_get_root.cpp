#include "http_server.h"


void get_handler_root( const shared_ptr< Session > session )
{
    const auto request = session->get_request( );
    
    string var_root_file     = request->get_path();
    
    cout<<"get_handler_root: "<<var_root_file<<endl;

    var_root_file=var_root_file.substr(1,var_root_file.size()-1);

    ifstream stream( var_root_file, ifstream::in );
    
    if ( stream.is_open( ) )
    {
        const string body = string( istreambuf_iterator< char >( stream ), istreambuf_iterator< char >( ) );
        
        const multimap< string, string > headers
        {
            { "Content-Type", "text/html" },
            { "Content-Length", ::to_string( body.length( ) ) }
        };
        
        session->close( OK, body, headers );
    }
    else
    {
        session->close( NOT_FOUND );
    }
}