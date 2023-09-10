#ifndef TCPRESPONSE_H
#define TCPRESPONSE_H

#ifndef TCPRESPONSE_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif //

#include <string>
#include <json/json.h>

class API TcpResponse
{
public:

    void clear();

    Json::Value& get(const std::string& key);

    Json::Value& get(const char* ptr);

    std::string toString();

private:

    Json::Value m_Json;

};


#endif // TCPRESPONSE_H
