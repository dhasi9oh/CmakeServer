
#include "TcpRequest.h"

void TcpRequest::reset(const std::string& json)
{
    m_Json.clear();
    Json::Reader reader;
    reader.parse(json, m_Json);
}

const Json::Value TcpRequest::get(const char*ptr) const
{
    return get(std::string(ptr, sizeof(ptr)));
}

const Json::Value TcpRequest::get(const std::string& str) const
{
    return m_Json.get(str, Json::Value());
}
