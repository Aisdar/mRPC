#ifndef MRPC_COMMON_LOG_H
#define MRPC_COMMON_LOG_H

#include <string>
#include <vector>
#include <queue>
#include <memory>

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
    kDebug   = 1,
    kInfo    = 2,
    kError   = 3
};

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
    void pushLog(std::string& msg);
    void log();

public:
    static Logger* getGlobalLogger();   /* 设计模式:单例模式 */

private:
    LogLevel                m_level;
    std::queue<std::string> m_buffer;
};

/* logPrefix + logMsg*/
#define DEBUGLOG(str, ...) \
    std::string msg = (new mrpc::LogEvent(mrpc::LogLevel::kDebug))->toString() + mrpc::formatString(str, ##__VA_ARGS__);  \
    mrpc::Logger::getGlobalLogger()->pushLog(msg);  \
    mrpc::Logger::getGlobalLogger()->log();  \

} // namespace mrpc 


#endif