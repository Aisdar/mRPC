#include "mRPC/common/config.h"

#define READ_XML_NODE(var, nodeName, parent) \
    TiXmlElement* var = parent->FirstChildElement(#nodeName); \
    if (!var) \
    { \
        MRPC_ERROR("Start rocket server error, failed to read node [%s]\n", #nodeName); \
        exit(0); \
    } \

 #define READ_STR_FROM_XML_NODE(var, nodeName, parent) \
    TiXmlElement* nodeName = parent->FirstChildElement(#nodeName); \
    if (!nodeName || !nodeName->GetText()) \
    { \
        MRPC_ERROR("Start rocket server error, failed to read node [%s]\n", #nodeName); \
        exit(0); \
    } \
    std::string var = std::string(nodeName->GetText()); \

namespace mrpc
{
static Config* g_config = nullptr;

/**
 * @brief 
 * @param [in] xmlfile !必须要传入,XML配置文件的路径
 * @return Config* 
 * 
 * @details TODO:需要考虑配置文件被中途更改的情况吗?
 */
Config *Config::GetInstance(const char *xmlfile)
{
    if(g_config)
    {
        return g_config;
    }
    else
    {
        if(!xmlfile)
        {
            MRPC_ERROR("Start rocket server error, cause no config file.\n", NULL);
            exit(0);
        }

        g_config = new Config(xmlfile);

        return g_config;
    }

}

Config::Config(const char *xmlfile)
    : m_log_level(LogLevel::kAny), /* 默认为kAny,什么都打印 */
      m_log_max_file_size(0),
      m_log_sync_inteval(0),
      m_port(0),
      m_io_threads(0)
{
    m_xml_document = new TiXmlDocument();
    bool rt = m_xml_document->LoadFile(xmlfile);
    if (!rt) 
    {
        MRPC_ERROR("Start rocket server error, failed to read config file '%s', error info[%s]\n", xmlfile, m_xml_document->ErrorDesc());
        exit(0);
    }
    
    /* 读取节点信息 */
    READ_XML_NODE(root_node, root, m_xml_document);
    READ_XML_NODE(log_node, log, root_node);
    READ_XML_NODE(server_node, server, root_node);

    READ_STR_FROM_XML_NODE(log_level_str, log_level, log_node);
    READ_STR_FROM_XML_NODE(log_file_name_str, log_file_name, log_node);
    READ_STR_FROM_XML_NODE(log_file_path_str, log_file_path, log_node);
    READ_STR_FROM_XML_NODE(log_max_file_size_str, log_max_file_size, log_node);
    READ_STR_FROM_XML_NODE(log_sync_interval_str, log_sync_interval, log_node);

    m_log_level = StringToLogLevel(log_level_str);
    m_log_file_name = log_file_name_str;
    m_log_file_path = log_file_path_str;
    m_log_max_file_size = std::atoi(log_max_file_size_str.c_str()) ;
    m_log_sync_inteval = std::atoi(log_sync_interval_str.c_str());

    MRPC_ANY("LOG -- CONFIG LEVEL[%s], FILE_NAME[%s],FILE_PATH[%s] MAX_FILE_SIZE[%d B], SYNC_INTEVAL[%d ms]\n", 
            LogLevelToString(m_log_level).c_str(), m_log_file_name.c_str(), m_log_file_path.c_str(), m_log_max_file_size, m_log_sync_inteval);

    READ_STR_FROM_XML_NODE(port_str, port,server_node);
    READ_STR_FROM_XML_NODE(io_threads_str, io_threads, server_node);

    m_port       = std::atoi(port_str.c_str());
    m_io_threads = std::atoi(io_threads_str.c_str());
    MRPC_ANY("LOG -- PORT[%d], IO_THREADS[%d]\n", m_port, m_io_threads);
}

} // namespace mrpc

