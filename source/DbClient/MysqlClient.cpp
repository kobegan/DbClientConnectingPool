#include <DbClient/MysqlClient.h>
#include <ConnectionPool/MysqlConnectionPool.h>

MysqlClient::MysqlClient()
{
}
MysqlClient::~MysqlClient()
{

}

bool MysqlClient::SelectDb(void *conn, const char *db)
{
    return mysql_select_db((MYSQL*)conn, db) ? false : true;    //if success mysql_select_db return 0  
}

bool MysqlClient::GetQuery(void* conn, const char* sql, DB_QUERY_CBFUNC cb, void* param)
{
    //printf("\nMysqlClient::GetQuery: %s\n", sql);
    MYSQL_RES *results;
    MYSQL_ROW row;
    if (mysql_real_query((MYSQL*)conn, sql, strlen(sql)) == 0){    //if success mysql_query return 0.  
        do {
            if (results = mysql_store_result((MYSQL*)conn))
            {
                while (row = mysql_fetch_row(results))
                {
                    cb(row, param);
                }
                mysql_free_result(results);
            }
        } while (!mysql_next_result((MYSQL*)conn));
        return true;
    }
    else{
        std::cout << "mysql query error:" << mysql_error((MYSQL*)conn) << std::endl;
        return false;
    }
}
bool MysqlClient::Query(void* conn, const char* sql)
{
    //printf("\nMysqlClient::Query: %s\n", sql);
    return mysql_real_query((MYSQL*)conn, sql, strlen(sql)) ? false : true;
}
void MysqlClient::ReleaseQuery(void* conn)
{
    if (conn)
    {
        MYSQL_RES *results;
        do {
            if (results = mysql_store_result((MYSQL*)conn))
            {
                mysql_free_result(results);
            }
        } while (!mysql_next_result((MYSQL*)conn));
    }
}

void MysqlClient::CheckConnection(void* conn)
{
    if (conn)
    {
        mysql_ping((MYSQL*)conn);
    }
}