/**
 * @file mutex.h
 * @author cym
 * @brief 锁
 * @version 1
 * @date 2024-03-30
 * 
 * @copyright Copyright (c) 2024 cym
 */

#ifndef MRPC_COMMON_MUTEX_H
#define MRPC_COMMON_MUTEX_H

#include <pthread.h>

namespace mrpc
{
/**
 * @brief 互斥锁
 * 
 * @details 
 */
class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_lock(&m_mutex);
    }
    ~Mutex()
    {
        pthread_mutex_unlock(&m_mutex);
    }
    void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }
    pthread_mutex_t* get_mutex()
    {
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
};

/**
 * @brief 
 * 
 * @details 
 */
template <class T>
class ScopeMutex
{
public:
    ScopeMutex(T& mutex)
    : m_mutex(mutex)
    {
        m_mutex.lock();
        m_is_lock = true;
    }
    ~ScopeMutex()
    {
        m_mutex.unlock();
        m_is_lock = false;
    }

    void lock()
    {
        if(!m_is_lock)
        {
            m_mutex.lock();
        }
    }
    void unlock()
    {
        if(m_is_lock)
        {
            m_mutex.unlock();
        }
    }

private:
    T& m_mutex;
    bool m_is_lock { false };
};

} // namespace mrpc


#endif