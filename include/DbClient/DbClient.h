#ifndef _DB_CLIENT_
#define _DB_CLIENT_

#include <list>
#include <DbClient/Config.h>

class AbstractDbClient;

class DbClient {
public:
    DbClient(DbType dbType);
    ~DbClient();

    bool SelectDb(const char *db);
    bool GetQuery(const char* sql, DB_QUERY_CBFUNC cb, void* param);
    bool Query(const char* sql);
    void ReleaseQuery(void* resultsPtr);
    void SetConnection(void* conn);
    void* ReturnConnection();
    void CheckConnection();


private:
    void CreateClient(DbType dbType);

private:
    AbstractDbClient* dbClient_;
    void* conn_;    //database connection
};

#endif