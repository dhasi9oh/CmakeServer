
#include "TcpResponse.h"

void TcpResponse::clear()
{
    m_Json.clear();
    Json::Value value;
    value["status"] = Json::Value();
    value["method"] = Json::Value();
    value["data"] = Json::Value();
    value["comment"] = Json::Value();
}

Json::Value& TcpResponse::get(const char* ptr)
{
    return get(std::string(ptr, sizeof(ptr)));
}

Json::Value& TcpResponse::get(const std::string& str)
{
    return m_Json[str];
}

std::string TcpResponse::toString()
{
    Json::FastWriter writer;
    std::string str = writer.write(m_Json);
    return str;
}
