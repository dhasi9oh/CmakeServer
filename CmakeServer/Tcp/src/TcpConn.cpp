
#include <TcpConn.h>

TcpConn::TcpConn(std::shared_ptr<asio::ip::tcp::socket> socket)
{
    m_Socket  = socket;
    is_Close = false;
    m_Status = ESTATUS::EMETHOD;
}

bool TcpConn::process()
{
    try {

    }
    catch (system::error_code& ec) {
        LOG_ERR("usr : {} process is error!\nerror value : {}\nerror : {}", m_Request.get("usr"), ec.value(), ec.what());
        is_Close = true;
        m_Socket->close();
        return false;
    }

    return true;
}

bool TcpConn::isClose() const
{
    return is_Close;
}

void TcpConn::close()
{
    is_Close = true;
}

void TcpConn::_makeResponse()
{
    while (m_Status != EFINISH) {
        switch (m_Status) {
        case EMETHOD:
            _parseMothed(m_Request.get("method"));
            break;
        case EOPTION:
            _parseOption(m_Request.get("option"));
            break;
        case EDATA:
            break;
        }
    }
}

void TcpConn::_parseMothed(const Json::Value& value)
{
    if (value == "post") {
        m_Status = EOPTION;
    }
    else if (value == "get"){
        m_Status = EDATA;
    }
}

void TcpConn::_parseOption(const Json::Value& value)
{
    m_Status = EDATA;
}

