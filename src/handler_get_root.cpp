#include "http_server.h"


void get_handler_root( const shared_ptr< Session > session )
{
    const auto request = session->get_request();
    const auto header  = request->get_headers();  
    const auto params = request->get_path_parameters();

    string var_root_file     = request->get_path();
    
    for (const auto it = header.begin(); it != header.end(); ++it)
    {
        cout << it->first << " : " ;
        cout << it->second << endl ;
    }

    for (const auto it = params.begin(); it != params.end(); ++it)
    {
        cout << it->first << " : " ;
        cout << it->second << endl ;
    }

    var_root_file=var_root_file.substr(1,var_root_file.size()-1);

    cout <<"sending file: "<<var_root_file<<endl;
    
    sendFile("./"+var_root_file,"text/plain",session);
}