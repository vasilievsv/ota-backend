#include "http_server.h"


void get_handler_codux_static( const shared_ptr< Session > session )
{
    const auto& request = session->get_request( );
    
    string path=request->get_path();

    cout<<"get_handler_static: "<<path<<endl;
 
    char * writable = new char[path.size() + 1];
    std::copy(path.begin(), path.end(), writable);
    writable[path.size()] = '\0';
    splitstring uri(writable);
    vector<string> detail= uri.split('/') ;
    delete[] writable;  

    string type=detail[1];
    const string filename = detail[2];


    if (type.compare("views")==0)
    { 
        sendFile("./static/views/"+filename,"text/html",session);
    } 
    else if (type.compare("css")==0)
    {
        sendFile("./static/css/"+filename,"text/css",session);
    }
    else if  (type.compare("js")==0)
    {
        sendFile("./static/js/"+filename,"text/js",session);
    } 
    else if  (type.compare("media")==0)
    {      

        char * p=strchr((char*)path.c_str(), '.');
        string ext(p);

        sendFile("./static/media/"+filename,"image/"+ext,session);
    }
    else
        session->close( NOT_FOUND );
}