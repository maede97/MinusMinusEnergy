#ifndef SOCKETIO_HPP
#define SOCKETIO_HPP
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <exception>
#include <cmath>
#include <stdexcept>
#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
class cppsocket {
private:
#ifdef _WIN32
	WSADATA wsa;
	SOCKET s;
#else
	int sock;
    struct sockaddr_in serv_addr;
#endif
	std::vector<char> buffer;
public:
	const static std::size_t buffersize = 1024;
#ifdef _WIN32
	cppsocket(WSADATA d, SOCKET _s);
#else
	cppsocket(sockaddr_in _serv_addr, int _sock);
#endif
	cppsocket();
	cppsocket(const cppsocket& o) = delete;
	cppsocket(cppsocket&& o);
	cppsocket(const std::string& addr, unsigned int PORT);
	void close();
	int socket_id();
	void write(const std::string& message);
	void write(const std::vector<char>& message);
	std::vector<char> receive();
	cppsocket& operator=(const cppsocket& o) = delete;
	cppsocket& operator=(cppsocket&& o);
};
class server_socket {
private:
#ifdef _WIN32
	WSADATA wsaData;
	int iResult;
	SOCKET ListenSocket = INVALID_SOCKET;
#else
	int port_;
	int server_fd;
	struct sockaddr_in address;
	socklen_t addrlen;
#endif
public:
	server_socket(const server_socket& o) = delete;
	server_socket(server_socket&& o);
	int port();
	server_socket(int _port);
	cppsocket accept_connection();
	void close();
	server_socket& operator=(const server_socket& o) = delete;
	server_socket& operator=(server_socket&& o);
};
#endif
