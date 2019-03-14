#include <socketio.hpp>
#include <stdio.h>
#include <cstdint>
#include <cinttypes>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <string>
#include <cstring>
#include <vector>
#include <iostream>

#ifdef _MSC_VER
#undef min
#ifdef _WIN64
using ssize_t = std::int64_t;
#endif
#ifndef _WIN64
using ssize_t = std::int32_t;
#endif
template<typename T>
const T& min(const T& a,const T& b){
	return (b < a) ? b : a;
}
#else
using std::min;
#endif

using std::size_t;

struct socket_exception : public std::exception {
	std::string msg;
	socket_exception(std::string&& _msg) {
		msg = std::move(_msg);
	}
	socket_exception(const std::string& _msg) {
		msg = _msg;
	}
	socket_exception(const char* _msg) {
		msg = std::string(_msg);
	}
	const virtual char* what() const throw () {
		//return "asdasdsa";
		return msg.length() == 0 ? "Connection creation failure" : msg.c_str();
	}
};

std::string GetLastErrorAsString() {
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string();

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

cppsocket::cppsocket() {
	buffer = std::vector<char>(buffersize + 1, 0);
}
cppsocket::cppsocket(cppsocket&& o) {
	s = o.s;
	o.s = INVALID_SOCKET;
	buffer = std::move(o.buffer);
}
cppsocket::cppsocket(WSADATA d, SOCKET _s) {
	s = _s;
	wsa = d;
	buffer = std::vector<char>(buffersize + 1, 0);
}
cppsocket::cppsocket(const std::string& addr, unsigned int PORT) {
	struct sockaddr_in server;
	buffer = std::vector<char>(buffersize + 1, 0);
	int recv_size;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		throw socket_exception(std::string("Socket creation error: ") + GetLastErrorAsString());
	}

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		throw socket_exception(std::string("Socket creation error: ") + GetLastErrorAsString());
	}

	server.sin_addr.s_addr = inet_addr(addr.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		throw socket_exception(std::string("Could not connect to host: ") + GetLastErrorAsString());
	}
}
void cppsocket::write(const std::string& message) {
	std::vector<char> msg(message.c_str(), message.c_str() + message.size());
	write(msg);
}
void cppsocket::write(const std::vector<char>& message) {
	for (size_t i = 0; i < message.size(); i += buffersize) {
		char cs[buffersize + 1] = { 0 };
		std::memcpy(cs, message.data() + i, buffersize);
		if ((i + buffersize) < message.size()) {
			cs[buffersize] = 'c';
			if (send(s, cs, buffersize + 1, 0) < 0) {
				throw socket_exception(std::string("Couldn't write to peer: ") + GetLastErrorAsString());
			}
		}
		else {
			cs[message.size() - i] = (char)0;
			if (send(s, cs, message.size() - i, 0) < 0) {
				throw socket_exception(std::string("Couldn't write to peer: ") + GetLastErrorAsString());
			}
		}
	}
}
std::vector<char> cppsocket::receive() {
	std::vector<char> stor;
	while (true) {
		std::fill(buffer.begin(), buffer.end(), (char)0);
		ssize_t val = recv(s, buffer.data(), buffersize + 1, 0);
		if (val == 0)throw socket_exception("Connection closed by peer");
		if (val < 0) {
			throw socket_exception(GetLastErrorAsString());
		}
		stor.insert(stor.end(), buffer.begin(), buffer.begin() + min(val, (ssize_t)buffersize));
		if (buffer.data()[buffersize] == (char)0) { break; }
	}
	std::cout << std::endl;
	return stor;
}
void cppsocket::close() {
	closesocket(s);
}

server_socket::server_socket(int port) {
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
	}
	freeaddrinfo(result);
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}
}
cppsocket server_socket::accept_connection() {
	SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}
	return cppsocket(wsaData, ClientSocket);
}
void server_socket::close() {
	closesocket(ListenSocket);
}
cppsocket& cppsocket::operator=(cppsocket&& o) {
	s = o.s;
	o.s = INVALID_SOCKET;
	buffer = std::move(o.buffer);
	return *this;
}
