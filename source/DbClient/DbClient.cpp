#include <DbClient/DbClient.h>
#include <ConnectionPool/MysqlConnectionPool.h>
#include <DbClient/AbstractDbClient.h>
#include <DbClient/MysqlClient.h>

DbClient::DbClient(DbType dbType) : dbClient_(NULL), conn_(NULL)
{
    CreateClient(dbType);
}

DbClient::~DbClient()
{
    if (dbClient_)
    {
        delete dbClient_;
        dbClient_ = NULL;
    }
    if (conn_)
    {
        conn_ = NULL;
    }
}

void DbClient::CreateClient(DbType dbType)
{
    if (dbType == DbType::DB_MYSQL)
    {
        dbClient_ = new MysqlClient();
    }
}

bool DbClient::SelectDb(const char *db)
{
    if (dbClient_ && conn_)
    {
        return dbClient_->SelectDb(conn_, db);
    }
    return false;
}

bool DbClient::GetQuery(const char* sql, DB_QUERY_CBFUNC cb, void* param)
{
    if (dbClient_ && conn_)
    {
        return dbClient_->GetQuery(conn_, sql, cb, param);
    }
    return NULL;
}

bool DbClient::Query(const char* sql)
{
    if (dbClient_ && conn_)
    {
        return dbClient_->Query(conn_, sql);
    }
    return false;
}

void DbClient::ReleaseQuery(void* resultsPtr)
{
    if (dbClient_ && conn_)
    {
        return dbClient_->ReleaseQuery(resultsPtr);
    }
}

void DbClient::SetConnection(void* conn)
{
    conn_ = conn;
}


void* DbClient::ReturnConnection()
{
    return conn_;
}

void DbClient::CheckConnection()
{
    if (dbClient_ && conn_)
    {
        dbClient_->CheckConnection(conn_);
    }
}