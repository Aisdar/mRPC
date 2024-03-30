
#include <pthread.h>
#include "mRPC/common/log.h"
#include "mRPC/common/config.h"

void *fun(void *)
{
    int i = 20;
    while (i--)
    {
        MRPC_DEBUG("debug this is thread in %s\n", "fun");
        MRPC_INFO("info this is thread in %s\n", "fun");
    }

    return NULL;
}

int main()
{

    mrpc::Config::GetInstance("../conf/rocket.xml");

    mrpc::Logger::GetInstance()->setLogLevel(mrpc::Config::GetInstance()->m_log_level);

    pthread_t thread;
    pthread_create(&thread, NULL, &fun, NULL);

    int i = 20;
    while (i--)
    {
        MRPC_DEBUG("test debug log %s\n", "11");
        MRPC_INFO("test info log %s\n", "11");
    }

    pthread_join(thread, NULL);
    return 0;
}