#include <ConnectionPool/MysqlConnectionPool.h>


ConnectionPool* ConnectionPool::instance_ = new ConnectionPool();
bool ConnectionPool::Initialized = false;

ConnectionPool::ConnectionPool() : poolEmpty_(true)
{
    mysql_library_init(0, NULL, NULL);
    connList_.clear();
}

bool ConnectionPool::PoolInit(const char* host, unsigned int port, 
    const char* name, const char* passwd,
    const char* db, const int num)
{
    assert(num > 0);
    if (Initialized)
    {
        return false;
    }
    for (int i = 0; i<num; ++i){
        MYSQL* ms;
        ms = mysql_init((MYSQL*)NULL);   
        assert(ms != NULL);
        char value = 1;
        mysql_options(ms, MYSQL_OPT_RECONNECT, &value);
        if (mysql_real_connect(ms, host, name, passwd, db, port, NULL, CLIENT_MULTI_STATEMENTS)){
            connList_.push_back(ms);
        }
        else{
            fprintf(stdout, "mysql pool init error.\n");
            return false;
        }
    }
    Initialized = true;
    poolEmpty_ = true;
    return true;
}

ConnectionPool::~ConnectionPool()
{
    DestoryPool();
    mysql_library_end();
}

MYSQL* ConnectionPool::GetConnection()
{
    boost::mutex::scoped_lock lock(mutex_);
    if (connList_.size() <= 0)
    {
        poolEmpty_ = true;
        // 等待主线程发送数据。
        while (poolEmpty_)
        {
            cond_get_.wait(lock);
        }
    }
    //util::mutex_lock_guard lock(mutex_);
    MYSQL *conn = connList_.front();
    connList_.pop_front();
    mysql_ping(conn);
    return conn;
}

void ConnectionPool::ReleaseConnection(void* conn)
{
    boost::mutex::scoped_lock lock(mutex_);
    connList_.push_back((MYSQL*)conn);    //push back  
    if (connList_.size() == 1)
    {
        poolEmpty_ = false;
        cond_get_.notify_one();
    }
}

void ConnectionPool::DestoryPool()
{
    while (connList_.size()){    //if the mysql connection free list not empty, release them  
        mysql_close(connList_.front());
        connList_.pop_front();
    }
}

void ConnectionPool::CheckConnection(void* conn)
{
    mysql_ping((MYSQL*)conn);
}