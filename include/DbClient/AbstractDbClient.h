#ifndef _DB_CLIENT_INTERFACE_
#define _DB_CLIENT_INTERFACE_

#include <DbClient/Config.h>

class AbstractDbClient {
public:
    virtual bool SelectDb(void*, const char*) = 0;
    virtual bool GetQuery(void*, const char*, DB_QUERY_CBFUNC, void*) = 0;
    virtual bool Query(void*, const char*) = 0;
    virtual void ReleaseQuery(void*) = 0;
    virtual void CheckConnection(void*) = 0;
};

#endif