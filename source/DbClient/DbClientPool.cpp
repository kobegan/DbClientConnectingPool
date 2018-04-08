#include <DbClient/DbClientPool.h>
#include <DbClient/DbClient.h>
#include <ConnectionPool/MysqlConnectionPool.h>

DbClientPool* DbClientPool::instance_ = new DbClientPool();

DbClientPool::DbClientPool() : poolEmpty_(true)
{
    clientList_.clear();
}


DbClientPool::~DbClientPool()
{
    DestoryPool();
}

bool DbClientPool::PoolInit(DbType dbType, DbClientConfig* config)
{
    if (dbType == DbType::DB_MYSQL)
    {

        if (ConnectionPool::getInstance()->PoolInit(config->host.c_str(), config->port,
            config->user.c_str(), config->pwd.c_str(), config->db.c_str(), config->poolSize))
        {
            for (size_t i = 0; i < config->poolSize; ++i){
                DbClient* client = new DbClient(dbType);
                assert(client != NULL);
                client->SetConnection(ConnectionPool::getInstance()->GetConnection());
                clientList_.push_back(client);
            }
            poolEmpty_ = false;
            return true;
        }
    }
    return false;
}

void DbClientPool::DestoryPool()
{
    while (clientList_.size()){    //if the mysql connection free list not empty, release them  
        DbClient *client = clientList_.front();
        ConnectionPool::getInstance()->ReleaseConnection(client->ReturnConnection());
        delete client;
        client = NULL;
        clientList_.pop_front();
    }
    ConnectionPool::DeletePool();
}

DbClient* DbClientPool::GetClient()
{
    boost::mutex::scoped_lock lock(mutex_);
    if (clientList_.size() <= 1)
    {
        poolEmpty_ = true;
        while (poolEmpty_)
        {
            cond_get_.wait(lock);
        }
    }
    //printf("\n%s 1 client size: %d\n", __FUNCTION__, clientList_.size());

    DbClient *client = clientList_.front();
    clientList_.pop_front();
    client->CheckConnection();
    return client;
}

void DbClientPool::ReleaseClient(DbClient* client)
{
    boost::mutex::scoped_lock lock(mutex_);
    clientList_.push_back(client);    //push back  
    if (clientList_.size() == 1)
    {
        poolEmpty_ = false;
        cond_get_.notify_one();
    }
}

size_t DbClientPool::GetPoolSize()
{
    boost::mutex::scoped_lock lock(mutex_);
    return clientList_.size();
}