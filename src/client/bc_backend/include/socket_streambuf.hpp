#ifndef SOCKET_OSTREAMBUF_HPP
#define SOCKET_OSTREAMBUF_HPP
#include <siostream.hpp>
#include <streambuf>
#include <memory>
#include <ios>
#include <iostream>
class socket_ostreambuf : public std::streambuf{
public:
    socket_ostreambuf(udpsocket* ptr, const std::string& addr, int port);
private:
    udpsocket* m_sock;
    std::string m_addr;
    int m_port;
    circular_buffer m_buf;
    virtual std::streamsize xsputn(const char* data, std::streamsize n)override;
    virtual int pbackfail(int c)override;
};
class socket_istreambuf : public std::streambuf{
public:
    socket_istreambuf(udpsocket* ptr);
private:
    udpsocket* m_sock;
    circular_buffer m_buf;
    virtual std::streamsize xsgetn(char* dest, std::streamsize n)override;
    virtual int pbackfail(int c)override;
    virtual int overflow(int c)override;
    virtual int underflow()override;
    virtual int uflow()override;
    virtual int sync()override;
    virtual std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which)override;
    virtual std::streampos seekpos(std::streampos sp, std::ios_base::openmode which)override;
    virtual std::streambuf* setbuf (char* s, std::streamsize n)override;
};
#endif
