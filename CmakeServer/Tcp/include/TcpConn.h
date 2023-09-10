#ifndef TCPCONN_H
#define TCPCONN_H


#ifndef TCPCONN_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif //

#include <map>
#include <vector>
#include <memory>
#include <boost/random.hpp>
#include <boost/asio.hpp>
#include <boost/random/random_device.hpp>
#include "Log.h"
#include "Buffer.h"
#include "TcpRequest.h"
#include "TcpResponse.h"
#include "MysqlConn.h"
#include "json/json.h"

using namespace boost;

class TcpConn
{
public:

    TcpConn(std::shared_ptr<asio::ip::tcp::socket> socket);

    TcpConn() = delete;

    TcpConn(const TcpConn&) = delete;

    TcpConn& operator=(const TcpConn&) = delete;

    bool process();

    bool isClose() const;

    void close();

private:

    void _makeResponse();

    void _write() throw(system::error_code);

    void _parseMothed(const Json::Value& value);

    void _parseOption(const Json::Value& value);

private:

    enum ESTATUS {
        EMETHOD,
        EOPTION,
        EDATA,
        EFINISH
    };

private:

    ESTATUS m_Status;

    bool is_Close;

    std::shared_ptr<asio::ip::tcp::socket> m_Socket;

    Buffer m_ReadBuf;

    Buffer m_WriteBuf;

    TcpRequest m_Request;

    TcpResponse m_Response;

};

#endif
