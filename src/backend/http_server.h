#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>
#include <fstream>
#include "restbed"
#include <chrono>
#include <streambuf>
#include <sstream>
#include <iostream>
using namespace std;
using namespace restbed;

extern void get_handler_codux_static( const shared_ptr< Session > session );
extern void get_handler_root( const shared_ptr< Session > session );
extern bool sendFile(string path,string type,const shared_ptr< Session > session );
extern void get_handler_codux_boot( const shared_ptr< Session > session );
extern void post_method_handler( const shared_ptr< Session > session );

class splitstring : public string 
{
    vector<string> flds;
public:
    splitstring(char *s) : string(s) { };
    vector<string>& split(char delim, int rep=0);
};