
#include "safe_json.h"
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

void jsonSaveFile(const char *filename, cJSON* json)
{
	FILE *fp = NULL;

	fp = fopen(filename,"wb");
	if(fp == NULL)
	{
		fprintf(stderr,"open file failed\n");
		exit(-1);
	}

    auto out = cJSON_PrintUnformatted(json);
    //ESP_LOGI(JSON_TAG,"%s",out);
	fprintf(fp,"%s",out);

	if(fp != NULL)
		fclose(fp);

    cJSON_free(out);
}

cJSON * jsonLoadFile(const char * path_to_file)
{
    printf("LOAD_FILE: %s\n", path_to_file);
    FILE*   fd   = NULL;
    cJSON*  json = NULL;
    char*   buff = (char*)calloc(1*1024,1);

    if(buff == NULL)
    {
        printf("ERROR_ALLOCATION_BUFFER\n");
        return NULL;
    }

    fd = fopen(path_to_file,"rb");
    if(fd == NULL)
    {
        printf("ERROR_OPENNING_FILE\n");
        free(buff);
        return NULL;
    }

    int count = fread(buff,1,4*1024,fd);
    if(count <= 0)
    {
        printf("ERROR_READ_FILE\n");
        free(buff);
        fclose(fd);
        return NULL;
    }

    //ESP_LOGI(JSON_TAG," %d bytes read",count);
    
    json = cJSON_Parse(buff);
    if(json == NULL)
    {
        const char * error_ptr = cJSON_GetErrorPtr();
        printf("ERROR_BEFORE=%s\n",error_ptr);
    }

    free(buff);
    fclose(fd);

    return json;
}

cJSON * jsonGetObject( cJSON* root, const char* name)
{
    auto T = cJSON_GetObjectItem( root, name );
    if( T )
    {
        return T;
    }
    
    return cJSON_AddObjectToObject(root,name);
}

char * jsonGetString(cJSON* root, const char* name,  char* _default="")
{
    auto T = cJSON_GetObjectItemCaseSensitive(root,name);
    if( T )
    {
        return T->valuestring;
    } 

    return _default;
}

vector<string> jsonGetStringA(cJSON* root, const char* name, char c)
{
    //auto str = jsonGetString( root, name, "" );
    //auto ret = tokenize( string(str), c );
    //
    //return ret;

    return vector<string>();
}

bool jsonGetBool(cJSON* root, const char* name, bool _default = false)
{
    auto T = cJSON_GetObjectItemCaseSensitive(root, name);
    
    if(T == NULL)
    {
        return _default;
    }

    return (bool)cJSON_IsTrue(T);
}

int64_t jsonGetInt64(cJSON* root, const char* name, const int64_t _default)
{
    auto T = cJSON_GetObjectItemCaseSensitive(root,name);
    if( T )
    {
        return (int64_t)T->valueint;
    }

    return _default;
}

int jsonGetInt(cJSON* root, const char* name, const int _default)
{
    auto T = cJSON_GetObjectItemCaseSensitive(root,name);
    if( T )
    {
        return T->valueint;
    }

    return _default;
}

cJSON* jsonGetArray(cJSON* root, const char* name)
{
    return cJSON_GetObjectItemCaseSensitive(root,name);
}