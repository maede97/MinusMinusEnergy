#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>
#include <netdb.h>
#include <string>
#include <cstring>
#include <cstdint>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <vector>
struct packet{
	struct sockaddr_in addr;
	int m_port;
	std::uint64_t checksum[4];
	std::uint64_t id;
	std::vector<char> content;
	packet(const std::vector<char>& content, const std::string& dest, int port);
	packet(const std::string& content, const std::string& dest, int port);
	void setContent(const std::vector<char>&);
};
class udpsocket {
	struct sockaddr_in addr;
	int s, slen = sizeof(addr);
	int m_port;
	public:
	udpsocket(int port);
	udpsocket(const udpsocket&) = delete;
	udpsocket& operator=(const udpsocket&) = delete;
	udpsocket(udpsocket&&);
	udpsocket& operator=(udpsocket&&);
	void write(const std::string&,const std::string& dest, int port)const;
	void write(const std::vector<char>&,const std::string& dest, int port)const;
	void write(const packet&)const;
	std::vector<char> receive()const;
	void close();
	int port()const;
};

