#ifndef _DB_CLIENT_MYSQL_
#define _DB_CLIENT_MYSQL_

#include <DbClient/AbstractDbClient.h>


class MysqlClient : public AbstractDbClient {
public:
    MysqlClient();
    virtual ~MysqlClient();

    virtual bool SelectDb(void* conn, const char *db);

    virtual bool GetQuery(void* conn, const char* sql, DB_QUERY_CBFUNC cb, void* param);

    virtual bool Query(void* conn, const char* sql);

    virtual void ReleaseQuery(void* conn);

    virtual void CheckConnection(void* conn);
};

#endif