#ifndef SQLCONNPOOL_H
#define SQLCONNPOOL_H

#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include "../log/log.h"

class SqlConnPool {
public:
    static SqlConnPool *Instance();     //单例

    MYSQL *GetConn();   //获取一个MYSQL连接
    void FreeConn(MYSQL * conn);    //释放一个MYSQL连接
    int GetFreeConnCount(); //获取空闲用户数量

    void Init(const char* host, int port,
              const char* user,const char* pwd, 
              const char* dbName, int connSize);
    void ClosePool();   //关闭数据库连接池

private:
    SqlConnPool();
    ~SqlConnPool();

    int MAX_CONN_;  // 最大的连接数
    int useCount_;  // 当前的用户数
    int freeCount_; // 空闲的用户数

    std::queue<MYSQL *> connQue_;   // 队列（MYSQL *）
    std::mutex mtx_;    // 互斥锁
    sem_t semId_;   // 信号量
};


#endif // SQLCONNPOOL_H