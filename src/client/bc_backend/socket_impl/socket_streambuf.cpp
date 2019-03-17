#include <socket_streambuf.hpp>
#include <algorithm>
#include <cassert>
#include <vector>
#include <boost/mpl/void.hpp>
socket_ostreambuf::socket_ostreambuf(udpsocket* ptr, const std::string& addr, int port) : m_sock(ptr), m_addr(addr),m_port(port),  m_buf(1024){}
std::streamsize socket_ostreambuf::xsputn(const char* data, std::streamsize n){
    std::for_each(data, data + n, [&](const char& c){m_buf.push_back(c);});
    if(m_buf.size() > 5){
        packet pack(std::vector<char>(m_buf.begin(), m_buf.end()), m_addr, m_port);
        m_sock->write(pack);
        m_buf.clear();
    }
    return n;
}
int socket_ostreambuf::pbackfail(int c){
    m_buf.push_front(c);
    return 0;
}
int socket_istreambuf::pbackfail(int c){
    std::unique_lock<std::mutex> lock(buffer_mutex);
    m_buf.push_front(c);
    return 0;
}
/*int socket_ostreambuf::sputc(char c){
    m_buf.push_back(c);
    return c;
}*/
socket_istreambuf::socket_istreambuf(udpsocket* ptr) : m_sock(ptr), m_buf(1024), reader_thread([this]{this->socket_read();}){}
std::streamsize socket_istreambuf::xsgetn(char* dest, std::streamsize n){
    assert(n >= 0);
    std::unique_lock<std::mutex> lock(buffer_mutex);
    if((circular_buffer::size_t)n > m_buf.size())
        buffer_cv.wait(lock);
    //n = std::min((std::size_t)n, m_buf.size());
    //if(n == 0)return EOF;
    std::copy(m_buf.begin(), m_buf.begin() + n, dest);
    for(int i = 0;i < n;i++)m_buf.pop_front();
    return n;
}
void socket_istreambuf::socket_read(){
    while(true){
        packet pack = m_sock->receiveP();
        if(pack.valid()){
            std::unique_lock<std::mutex> lock(buffer_mutex);
            std::for_each(pack.content.begin(), pack.content.end(), [this](const char& c){this->m_buf.push_back(c);});
            buffer_cv.notify_one();
        }
    }
}
int socket_istreambuf::overflow(int c){
    m_buf.push_back(c);
    return c;
}
int socket_istreambuf::underflow(){
    std::unique_lock<std::mutex> lock(buffer_mutex);
    if(m_buf.size() == 0)buffer_cv.wait(lock);
    return *m_buf.begin();
}
/*int socket_istreambuf::sync(){
    return EOF;
}
std::streampos socket_istreambuf::seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which){
    return EOF;
}
std::streampos socket_istreambuf::seekpos(std::streampos sp, std::ios_base::openmode which){
    return EOF;
}
std::streambuf* socket_istreambuf::setbuf(char* s, std::streamsize n){
    return nullptr;
}*/
int socket_istreambuf::uflow(){
    if(m_buf.size() == 0)return EOF;
    int c = *m_buf.begin();
    m_buf.pop_front();
    return c;
}
/*char socket_istreambuf::sgetc(){
    char c = *m_buf.begin();
    m_buf.pop_front();
    return c;
}*/
