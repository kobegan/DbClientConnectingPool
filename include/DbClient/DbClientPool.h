#ifndef _DB_CLIENT_POOL_
#define _DB_CLIENT_POOL_

#include <string>
#include <list>
#include <memory>
#include <mysql/mysql.h>
#include <boost/thread.hpp>
#include <DbClient/Config.h>

class DbClient;

class DbClientPool
{
public:
    static DbClientPool* getInstance()
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

    bool PoolInit(DbType dbType, DbClientConfig* config);

    DbClient* GetClient();

    void ReleaseClient(DbClient* client);

    size_t GetPoolSize();

    ~DbClientPool();

private:

    DbClientPool();

    void DestoryPool();

private:
    std::list<DbClient*> clientList_;     //存放所有连接
    static DbClientPool *instance_;//连接池对象
    boost::mutex mutex_;
    boost::condition_variable  cond_get_;
    bool poolEmpty_;   //条件
};

#endif
