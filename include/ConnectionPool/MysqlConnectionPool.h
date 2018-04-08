#ifndef _MYSQL_CONNECTION_POOL_
#define _MYSQL_CONNECTION_POOL_

#include <string>
#include <list>
#include <memory>
#include <mysql/mysql.h>
#include <boost/thread.hpp>

class ConnectionPool
{
public:
    //获取数据库连接池对象 static单例模式
    static ConnectionPool* getInstance()
    {
        return instance_;
    }

    static void DeletePool()
    {
        if (instance_ != NULL)
        {
            delete instance_;
            instance_ = NULL;
        }
    }

    bool PoolInit(const char* host, unsigned int port, const char* name, const char* passwd,
        const char* db, const int num);

    MYSQL* GetConnection();

    void ReleaseConnection(void* conn);

    void CheckConnection(void* conn); 

    ~ConnectionPool();

private:
    ConnectionPool();

    void DestoryPool();
private:
    std::list<MYSQL*> connList_;     //存放所有连接
    static bool Initialized;
    static ConnectionPool *instance_;//连接池对象
    boost::mutex mutex_;
    boost::condition_variable  cond_get_;
    bool poolEmpty_;   //条件
};

#endif