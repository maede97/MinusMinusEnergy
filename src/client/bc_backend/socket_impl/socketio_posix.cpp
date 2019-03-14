#include <socketio.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include <cinttypes>
using std::size_t;
struct socket_exception : public std::exception{
	std::string msg;
	socket_exception(std::string&& _msg){
		msg = std::move(_msg);
	}
	socket_exception(const std::string& _msg){
		msg = _msg;
	}
	socket_exception(const char* _msg){
		msg = std::string(_msg);
	}
	const virtual char* what() const throw (){
    	return msg.length() == 0 ? "Connection creation failure" : msg.c_str();
    }
};
cppsocket::cppsocket(sockaddr_in _serv_addr,int _sock) : sock(_sock), serv_addr(_serv_addr){
	buffer = std::vector<char>(buffersize + 1,0);
}
cppsocket::cppsocket(){buffer = std::vector<char>(buffersize + 1, 0);};
cppsocket::cppsocket(const std::string& addr, unsigned int PORT){
	sock = 0;
	buffer = std::vector<char>(buffersize + 1, 0);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw socket_exception("Socket creation error");
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, addr.c_str(), &serv_addr.sin_addr)<=0){
		throw std::invalid_argument("Invalid address: " + addr);
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		throw socket_exception("Could not reach server " + addr);
	}
}

cppsocket::cppsocket(cppsocket&& o){
	sock = o.sock;
	o.sock = 0;
	buffer = std::move(o.buffer);
}
int cppsocket::socket_id(){
	return sock;
}
void cppsocket::close(){
	shutdown(sock, 2);
}
void cppsocket::write(const std::string& message){
	std::vector<char> msg(message.c_str(), message.c_str() + message.size());
	write(msg);
}
void cppsocket::write(const std::vector<char>& message){
	for(size_t i = 0;i < message.size();i += buffersize){
		char cs[buffersize + 1] = {0};
		std::memcpy(cs, message.data() + i,buffersize);
		if((i + buffersize) < message.size()){
			cs[buffersize] = 'c';
			if(send(sock, cs, buffersize + 1, 0) < 0){
				throw socket_exception(std::string("Couldn't write to peer: ") + strerror(errno));
			}
		}
		else{
			cs[message.size() - i] = (char)0;
			if(send(sock, cs, message.size() - i, 0) < 0){
				throw socket_exception(std::string("Couldn't write to peer: ") + strerror(errno));
			}
		}
	}
}
std::vector<char> cppsocket::receive(){
	std::vector<char> stor;
	while(true){
		std::fill(buffer.begin(), buffer.end(), (char)0);
		ssize_t val = read(sock, buffer.data(), buffersize + 1);
		if(val == 0)throw socket_exception("Connection closed by peer");
		if(val < 0){
			throw socket_exception(strerror(errno));
		}
		stor.insert(stor.end(), buffer.begin(), buffer.begin() + std::min(val, (ssize_t)buffersize));
		if(buffer.data()[buffersize] == (char)0){break;}
	}
	std::cout << std::endl;
	return stor;
}
server_socket::server_socket(int _port) : port_(_port){
	int opt = 1;
	addrlen = sizeof(address);
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		throw socket_exception(std::string("server_socket creation failed: ") + strerror(errno));
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		throw socket_exception(std::string("server_socket creation failed: ") + strerror(errno));
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port());
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
		throw socket_exception("Couldn't bind to port " + std::to_string(port()) + ", is it already in use?");
	}
	if (listen(server_fd, 16) < 0){
		throw socket_exception("Error in listen() call, no clue why");
	}
}
void server_socket::close(){
	shutdown(server_fd, SHUT_RDWR);
}
cppsocket server_socket::accept_connection(){
	int new_socket;
	sockaddr client_addr;
	if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr,(socklen_t*)&addrlen))<0){
		throw socket_exception(std::string("Socket accept failed: ") + strerror(errno));
	}
	return cppsocket(*((sockaddr_in*)&client_addr), new_socket);
}

int server_socket::port(){return port_;}
cppsocket& cppsocket::operator=(cppsocket&& o){
	sock = o.sock;
	o.sock = 0;
	buffer = std::move(o.buffer);
	return *this;
}
