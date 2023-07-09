//
//
// post default
//
//
// curl -i -H "Accept: application/json" -H "Content-type: application/json" -X POST -d '{"test":{"field":"xxx"}}' http://localhost:8080/func1

#include "backend/http_server.h"
#include "my_utils/safe_json.h"


void post_method_handler( const shared_ptr< Session > session )
{
    // CORS
    if (session->get_request()->get_method() == "OPTIONS") 
    {
        //cout<<"CORS ENABLE"<<endl;
        session->close(OK);
        return;
    }

    const auto request      = session->get_request( );
    auto path               = request->get_path();
    size_t content_length   = request->get_header( "Content-Length", 0 );

    session->fetch( content_length, [ request ]( const shared_ptr< Session > session, const Bytes & raw )
    {
        cJSON *json = cJSON_Parse( (char*)raw.data() );
        char *str   = cJSON_Print(json);
        
        printf("SEND-TO-COMMAND-BUS:\n%s\n", str);
    
        session->close(OK);

//
// LEGACY: обработка пост-запроса
//
//        // генерация json'a
//        system("./wrapper_apt2json.sh");
//        ifstream t("./gen_package_list.json");
//        
//        string body;
//        body = string((istreambuf_iterator<char>(t)),
//                    istreambuf_iterator<char>());
//        
//        const multimap< string, string > headers
//        {
//            { "Content-Type", "application/x-www-form-urlencoded"},
//            { "Content-Length", to_string( body.length() ) }
//        };
//
//        cJSON_free(json);
//        session->close( OK, body, headers );

    } );
}