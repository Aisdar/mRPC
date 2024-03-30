#ifndef MRPC_COMMON_UTIL_H
#define MRPC_COMMON_UTIL_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace mrpc
{
static int              g_pid = 0;
static thread_local int g_tid = 0;
/**
 * @brief 进程ID
 * @return pid_t 
 * 
 * @details 
 */
pid_t getPid()
{
    if(g_pid != 0)
    {
        return g_pid;
    }
    return getpid();
}

/**
 * @brief 线程ID
 * @return pid_t 
 * 
 * @details 
 */
pid_t getTid()
{
    if(g_tid != 0)
    {
        return g_tid;
    }
    return syscall(SYS_gettid); /* glibc库可能不提供gettid,最elegant的方式就是这么拿tid */
}
} // namespace mrpc

#endif