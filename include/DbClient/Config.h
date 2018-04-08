#ifndef _DB_CLIENT_CONFIG_
#define _DB_CLIENT_CONFIG_

#include <string>
#include <functional>  

typedef std::function<void(char** row, void *param)> DB_QUERY_CBFUNC;

struct DbClientConfig
{
    std::string host;
    unsigned int port;
    std::string user;
    std::string pwd;
    std::string db;
    unsigned int poolSize;
    std::string db_path;  //For sqlite use
};

enum DbType
{
    DB_MYSQL = 1,
    DB_SQLITE = 2
};

#endif