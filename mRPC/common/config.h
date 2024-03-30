#ifndef MRPC_COMMON_CONFIG_H
#define MRPC_COMMON_CONFIG_H

#include <string>
#include <tinyxml/tinyxml.h>

#include "mRPC/common/log.h"

namespace mrpc
{
class Config
{
public:
    static Config* GetInstance(const char *xmlfile = nullptr);
    

private:
    Config() {};    /* 禁用, TODO 禁用拷贝 */ 
    Config(const char* xmlfile);
    
/* 成员为配置项 不需要设置private属性 */
public:
    LogLevel       m_log_level;
    std::string    m_log_file_name;
    std::string    m_log_file_path;
    int            m_log_max_file_size;
    int            m_log_sync_inteval;   // 日志同步间隔，ms
    int            m_port;
    int            m_io_threads;

private:
    TiXmlDocument* m_xml_document;
};

} // namespace mrpc

#endif