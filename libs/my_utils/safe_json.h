#pragma once

#ifndef _UTILS_JSON_H_
#define _UTILS_JSON_H_

#include <inttypes.h>
#include <stdio.h>
#include <cJSON.h>


#define JSON_TAG "JSON"

extern void     jsonSaveFile  ( const char *filename, cJSON* json);
extern cJSON *  jsonLoadFile  ( const char * path_to_file );
extern bool     jsonGetBool   ( cJSON* root, const char* name, bool          _default );
extern int      jsonGetInt    ( cJSON* root, const char* name, const int     _default );
extern int64_t  jsonGetInt64  ( cJSON* root, const char* name, const int64_t _default );
extern char *   jsonGetString ( cJSON* root, const char* name, const char*   _default );
extern cJSON *  jsonGetObject ( cJSON* root, const char* name );

#endif