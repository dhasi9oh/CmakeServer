#ifndef BUFFER_H
#define BUFFER_H


#ifndef BUFFER_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif // !BLOCKQUEUE

#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>

using namespace boost;

API void encoding(std::vector<char>& v, int first, int last, int key)
{
    while (first != last) {
        v[first] ^= key;
        first++;
    }
}

API void deencoding(std::vector<char>& v, int first, int last, int key)
{
    while (first != last) {
        v[first] ^= key;
        first++;
    }
}

class API Buffer
{
public:

    Buffer() = default;

    ~Buffer() = default;

    void read(asio::ip::tcp::socket& socket, bool deencode = true);

    void write(asio::ip::tcp::socket& socket, bool encode = true, int key = 255);

    std::string getAll();

    void append(const std::string& str);

    void append(const char* ptr, size_t len);

private:

    std::vector<char> m_Buffer;
};


#endif
