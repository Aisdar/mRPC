#ifndef MRPC_COMMON_LOG_H
#define MRPC_COMMON_LOG_H

#include <string>
#include <vector>
#include <queue>
#include <memory>

#include "mutex.h"

namespace mrpc
{
/**
 * @brief 获得格式化字符
 * @tparam Args 
 * @param [in] str 字符格式 
 * @param [in] args 待填充的参数
 * @return std::string 填充后文本 
 * 
 * @details 将args中的参数填充到str内
 */
template<typename... Args>
std::string formatString(const char* str, Args&&... args)
{
    int size = snprintf(nullptr, 0, str, args...); /* 只是想拿到args填充后的size */

    std::string result;
    if(size > 0)
    {   
        result.resize(size);
        snprintf(&result[0], size + 1, str, args...);
    }

    return result;
}

enum LogLevel
{
    kUnknown = 0,
    kInfo    = 1, 
    kError   = 2,
    kDebug   = 3,
    kAny     = 4   /* 增加/修改最大枚举时这个kAny枚举值要一起修改 */   
};

std::string LogLevelToString(LogLevel level);
LogLevel StringToLogLevel(const std::string& log_level);

class LogEvent
{
public:
    LogEvent(LogLevel level): m_level(level) {};

    LogLevel getLogLevel() const
    {
        return m_level;
    }

    std::string getFileName() const
    {
        return m_file_name;
    }

    std::string toString();
private:
    LogLevel    m_level;        /* 日志等级 */

    std::string m_file_name;   /* 日志输出文件名字 */
    int32_t     m_file_line;   /* 行号 */
    int32_t     m_pid;         /* 进程ID */
    int32_t     m_tid;         /* 线程ID */
};

class Logger
{
public:
    void setLogLevel(LogLevel level);
    LogLevel getLogLevel();
    void pushLog(const std::string& msg);
    void log();

public:
    static Logger* GetInstance();   /* 设计模式:单例模式 */

private:
    LogLevel                m_level { LogLevel::kAny }; /* kAny时,所有日志都打印 */
    std::queue<std::string> m_buffer;
    Mutex                   m_mutex;
};

/* logPrefix + logMsg*/
#define MRPC_INFO(str, ...) \
    if(mrpc::Logger::GetInstance()->getLogLevel() && mrpc::Logger::GetInstance()->getLogLevel() >= mrpc::LogLevel::kInfo) \
    { \
        mrpc::Logger::GetInstance()->pushLog(mrpc::LogEvent(mrpc::LogLevel::kInfo).toString() \
        + "[" + std::string(__FILE__) + std::to_string(__LINE__) + "]" \
        + mrpc::formatString(str, ##__VA_ARGS__)); \
        mrpc::Logger::GetInstance()->log(); \
    } \
        
#define MRPC_ERROR(str, ...) \
    if(mrpc::Logger::GetInstance()->getLogLevel() && mrpc::Logger::GetInstance()->getLogLevel() >= mrpc::LogLevel::kError) \
    { \
        mrpc::Logger::GetInstance()->pushLog(mrpc::LogEvent(mrpc::LogLevel::kError).toString() \
        + "[" + std::string(__FILE__) + std::to_string(__LINE__) + "]" \
        + mrpc::formatString(str, ##__VA_ARGS__)); \
        mrpc::Logger::GetInstance()->log(); \
    } \


#define MRPC_DEBUG(str, ...) \
    if(mrpc::Logger::GetInstance()->getLogLevel() && mrpc::Logger::GetInstance()->getLogLevel() >= mrpc::LogLevel::kDebug) \
    { \
        mrpc::Logger::GetInstance()->pushLog(mrpc::LogEvent(mrpc::LogLevel::kDebug).toString() \
        + "[" + std::string(__FILE__) + std::to_string(__LINE__) + "]" \
        + mrpc::formatString(str, ##__VA_ARGS__)); \
        mrpc::Logger::GetInstance()->log(); \
    } \

/* 无视日志等级的输出 */
#define MRPC_ANY(str, ...) \
    if(mrpc::Logger::GetInstance()->getLogLevel() && mrpc::Logger::GetInstance()->getLogLevel() >= mrpc::LogLevel::kAny) \
    { \
        mrpc::Logger::GetInstance()->pushLog(mrpc::LogEvent(mrpc::LogLevel::kAny).toString() \
        + "[" + std::string(__FILE__) + std::to_string(__LINE__) + "]" \
        + mrpc::formatString(str, ##__VA_ARGS__)); \
        mrpc::Logger::GetInstance()->log(); \
    } \

} // namespace mrpc 


#endif