//
//  Over The Air by Backend
//
#include <csignal>
#include <chrono>
#include <unistd.h>
#include "backend/http_server.h"

#include "my_utils/safe_json.h"
#include "my_utils/safe_delete.h"

void ws_get_method_handler( const shared_ptr< Session > session );
void ws_ping_handler( void );
void ws_send_all(string);

bool termination_flag = false;
void signalHandler(int signum)
{
    termination_flag = true;
    exit(signum);
}


int main( const int, const char** )
{
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGILL , signalHandler);

    auto settings  = make_shared< Settings >( );

    auto resource1 = make_shared< Resource >( );
    auto resource2 = make_shared< Resource >( );
    auto resource3 = make_shared< Resource >( );
    auto resource4 = make_shared< Resource >( );

    auto resource_ws = make_shared< Resource >( );

// ОТЛАДКА: CORS DISABLE
    // settings->set_default_header( "Connection", "close" );
    // settings->set_default_header( "Access-Control-Allow-Origin", "*");
// Разрешения запроса из приложения к внешним ip (нужен CORS)
    settings->set_default_headers( { 
        { "Access-Control-Allow-Origin", "* "},
        { "Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept"} 
    });


// React App
    resource1->set_path( "/.*" );
    resource2->set_path( "/app/boot" );
    resource3->set_path ( "/static/.*/.*" );

    resource1->set_method_handler ( "GET" , get_handler_root );
    resource2->set_method_handler ( "GET" , get_handler_codux_boot );
    resource3->set_method_handler ( "GET" , get_handler_codux_static );

// REST-API
    resource4->set_path ( "/api/post" );
    resource4->set_method_handler ( "POST" , post_method_handler );
    resource4->set_method_handler ( "OPTIONS" , post_method_handler );

// WebSocket
    resource_ws->set_path( "/socket/bus" );
    resource_ws->set_method_handler( "GET", ws_get_method_handler );
    
//
// Publish 
//
    std::thread  _backend_thread([&]{
        Service service;

        service.publish( resource1 );
        service.publish( resource2 );
        service.publish( resource3 );
        service.publish( resource4 );
        service.publish( resource_ws );
        
        settings->set_port( 8080 );
        service.schedule( ws_ping_handler, chrono::milliseconds( 3000 ) );
        service.start( settings );
    });

//
// MAIN
//
    printf("*\n");
    printf("* ota-backend [start]\n");
    printf("*\n");
    run:while(termination_flag != true)
    {
        sleep(3);
    }
    _backend_thread.join();

    return EXIT_SUCCESS;
}