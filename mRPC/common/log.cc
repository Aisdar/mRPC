#include <sys/time.h>
#include <sstream>
#include <stdio.h>

#include "mRPC/common/log.h"
#include "mRPC/common/util.h"

namespace mrpc
{
static Logger* g_logger = NULL; /* global logger */

/* as you can see */
std::string LogLevelToString(LogLevel level) 
{
    switch (level) 
    {
    case LogLevel::kDebug:
        return "Debug";
    case LogLevel::kInfo:
        return "Info";
    case LogLevel::kError:
        return "Error";
    case LogLevel::kAny:
        return "Any";
    default:
        return "Unknown";
    }
}

/* as you can see */
LogLevel StringToLogLevel(const std::string& log_level) 
{
    if (log_level == "DEBUG") 
    {
        return LogLevel::kDebug;
    } 
    else if (log_level == "INFO") 
    {
        return LogLevel::kInfo;
    } 
    else if (log_level == "ERROR") 
    {
        return LogLevel::kError;
    } 
    else if (log_level == "ANY")
    {
        return LogLevel::kAny;
    }
    else 
    {
        return LogLevel::kUnknown;
    }
}

/**
 * @brief 返回对应日志等级的字符串前缀信息
 * @return std::string 
 * 
 * @details 返回值是该等级的日志字符串前缀信息,格式为 [LogLevel][Year-Month-Day Hour:Minute:Second.MillionSecond]
 */
std::string LogEvent::toString()
{
    struct timeval now_time = { 0 };                /* 时间戳 */
    gettimeofday(&now_time, nullptr);               /* 获得时间戳,微秒级 */

    struct tm now_time_t = { 0 };                   /* 时间结构体 */
    localtime_r(&(now_time.tv_sec), &now_time_t);   /* localtime_r是线程安全的 */

    /* 时间转换为日志 */
    char buf[128];
    strftime(&buf[0], 128, "%y-%m-%d %H:%M:%S", &now_time_t);
    std::string time_str(buf);
    
    int ms = now_time.tv_usec / 1000;
    time_str = time_str + "." + std::to_string(ms);
    
    m_pid = getPid();
    m_tid = getTid();

    std::stringstream ss;
    ss << "[" << LogLevelToString(m_level) << "]\t"
        << "[" << time_str << "]\t"
        << "["<< getPid() << ":" << getTid() << "]";

    return ss.str();
}

void Logger::setLogLevel(LogLevel level)
{
    m_level = level;
}

LogLevel Logger::getLogLevel()
{
    return m_level;
}

void Logger::pushLog(const std::string &msg)
{
    ScopeMutex<Mutex> lock(m_mutex);
    m_buffer.push(msg);
}

void Logger::log()
{
    /* TODO 日志打印这里需要优化 */
    ScopeMutex<Mutex> lock(m_mutex);
    while(!m_buffer.empty())
    {
        std::string msg = m_buffer.front();
        m_buffer.pop();

        printf("%s", msg.c_str());
    }
}

Logger *Logger::GetInstance()
{
    if(g_logger)
    {
        return g_logger;
    }

    g_logger = new Logger();
    return g_logger;
}

}

