
#include "Buffer.h"

void Buffer::read(asio::ip::tcp::socket& socket, bool deencode)
{
    std::function<void(asio::ip::tcp::socket&,char*,int,system::error_code&)> func =
        [](asio::ip::tcp::socket& sock, char* buf, int len, system::error_code& ec) -> void
    {
        int idx = 0;
        while (idx != len) {
            idx += sock.read_some(asio::buffer(buf + idx, len - idx), ec);
        }
        if (ec) {
            throw system::system_error(ec);
        }
    };

    system::error_code ec;
    // 读取前三位拿到加密用的 key
    const int KEYLEN = 3;
    char key_len_str[KEYLEN] = {'\0', };
    func(socket, key_len_str, KEYLEN, ec);
    int key = std::stoi(std::string(key_len_str, KEYLEN));

    // 读取第四位到第十三位看看报文有多长
    const int BUFFERLEN = 10;
    char buf_len_str[BUFFERLEN+1] = {'\0', };
    func(socket, buf_len_str, BUFFERLEN, ec);
    int buffer_len = std::stoi(std::string(buf_len_str, BUFFERLEN));

    std::vector<char> buf(buffer_len);
    func(socket, std::addressof(*buf.begin()), buffer_len, ec);
    m_Buffer = std::move(buf);

    // 是否需要解密
    if (deencode) {
        deencoding(m_Buffer, 0, m_Buffer.size(), key);
    }

}

void Buffer::write(asio::ip::tcp::socket& socket, bool encode, int key)
{
    std::function<std::string(int,int)> func =
        [] (int len, int num){
            std::string s = std::to_string(num);
            while (s.size() < len) {
                s = "0" + s;
            }
            return s;
        };

    const int KEYLEN = 3;
    std::string key_len_str = "000";
    if (encode) {
        // ascll 码的上限是 255
        key_len_str = func(KEYLEN, key%256);
    }

    // 第四位到第十三位是 buffer 的长度
    const int BUFFERLEN = 10;
    std::string buffer_len_str = func(BUFFERLEN, m_Buffer.size());

    // 先将数据加密
    deencoding(m_Buffer, 0, m_Buffer.size(), key);
    // 将数据先拼接起来
    std::string buf;
    buf.append(key_len_str);
    buf.append(buffer_len_str);
    buf.append(m_Buffer.begin(), m_Buffer.end());
    // 发送数据
    socket.send(asio::buffer(buf));
    m_Buffer.clear();
}

std::string Buffer::getAll()
{
    return std::string(m_Buffer.begin(), m_Buffer.end());
}

void Buffer::append(const std::string& str)
{
    m_Buffer.insert(m_Buffer.end(), str.begin(), str.end());
}

void Buffer::append(const char* ptr, size_t len)
{
    m_Buffer.insert(m_Buffer.end(), ptr, ptr + len);
}
