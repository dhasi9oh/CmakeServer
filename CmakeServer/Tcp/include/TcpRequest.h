#ifndef TCPREQUEST_H
#define TCPREQUEST_H

#ifndef TCPREQUEST_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif // !BLOCKQUEUE

#include <string>
#include <json/json.h>

class API TcpRequest
{
public:

    void reset(const std::string& json);

    const Json::Value get(const char* ptr) const;

    const Json::Value get(const std::string& str) const;

private:

    Json::Value m_Json;

};



#endif // TCPREQUEST_H
