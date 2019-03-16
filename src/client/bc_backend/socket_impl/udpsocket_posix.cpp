#include <udpsocket.hpp>
#include <cstdint>
#include <hash.hpp>
#include <algorithm>
std::uint64_t global_id_counter = 0;
packet::packet(const std::vector<char>& content, const std::string& dest, int port) : m_port(port), id(global_id_counter++){
	hostent *server_host;
	errno = 0;
	server_host = gethostbyname(dest.c_str());
	if(errno != 0){
		throw std::logic_error("Could not resolve hostname " + dest + ": " + strerror(errno));
	}
	/* configure the server address */
	addr.sin_family = AF_INET; // IPv4
	memcpy(&addr.sin_addr, server_host->h_addr,
			sizeof(struct in_addr)); 
			addr.sin_port = htons(port);
	this->content = content;
    char* id_it = (char*)&id;
    this->content.insert(this->content.end(), id_it, id_it + 8);
    hash_checksum(checksum, this->content);
    for(int i = 0;i < 8;i++)
    this->content.pop_back();
}
void packet::setContent(const std::vector<char>& _content){
	this->content = _content;
	char* id_it = (char*)&id;
    this->content.insert(this->content.end(), id_it, id_it + 8);
	hash_checksum(checksum, this->content);
    for(int i = 0;i < 8;i++)
        this->content.pop_back();
}
packet::packet(const std::string& content, const std::string& dest, int port) : packet(std::vector<char>(content.begin(), content.end()),dest,port){}
udpsocket::udpsocket(int port) : m_port(port){
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		throw std::logic_error(std::string("Socket creation failed: ") + strerror(errno));
	}
	std::memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(this->port());

	if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		throw std::logic_error(std::string("Socket binding failed: ") + strerror(errno));
	}
}
udpsocket::udpsocket(udpsocket&& o) : addr(o.addr),s(o.s),m_port(o.m_port) {
	o.s = 0;
	o.m_port = 0;
}
udpsocket& udpsocket::operator=(udpsocket&& o){
	addr = o.addr;
	s = o.s;
	m_port = o.m_port;
	o.s = 0;
	o.m_port = 0;
	return *this;
}
void udpsocket::write(const std::string& msg, const std::string& dest, int port)const{
	std::vector<char> _msg(msg.begin(), msg.end());
	write(_msg, dest, port);
}
void udpsocket::write(const std::vector<char>& msg, const std::string& dest, int port)const{
	hostent *server_host;
	errno = 0;
	server_host = gethostbyname(dest.c_str());
	if(errno){
		throw std::logic_error("Could not resolve hostname " + dest + ": " + strerror(errno));
	}
	/* configure the server address */
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; // IPv4
	memcpy(&server_addr.sin_addr, server_host->h_addr,
			sizeof(struct in_addr)); server_addr.sin_port = htons(port);
	/* send a message */
	if(sendto(s, msg.data(), msg.size(), 0,(const sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		throw std::logic_error(std::string("Could not send packet: ") + strerror(errno));
	}
}

void udpsocket::write(const packet& pack)const{
    std::vector<char> output;
    output.reserve(pack.content.size() + 32 + 8);
    for(int i = 0;i < 4;i++)
        for(int ih = 0;ih < 8;ih++)
            output.push_back((pack.checksum[i] & (0xFFULL << ih * 8)) >> (ih * 8));
    for(int ih = 0;ih < 8;ih++)
        output.push_back((pack.id & (0xFFULL << ih * 8)) >> (ih * 8));
    for(int i = 0;i < 4;i++){
        std::cout << pack.checksum[i] << ", ";
    }
    std::copy(pack.content.begin(), pack.content.end(), std::back_inserter(output));
	if(sendto(s, output.data(), output.size(), 0,(const sockaddr*)&pack.addr, sizeof(pack.addr)) < 0){
		throw std::logic_error(std::string("Could not send packet: ") + strerror(errno));
	}
}

std::vector<char> udpsocket::receive()const{
	std::vector<char> ret(1024);
	int l;
	if ((l = read(s, ret.data(), 1024)) <= 0) {
		throw std::logic_error(std::string("Could not receive packet: ") + strerror(errno));
	}
	ret.resize(l);
	return ret;
}
void udpsocket::close(){
	shutdown(s, 2);
}
int udpsocket::port()const {
	return this->m_port;
}
