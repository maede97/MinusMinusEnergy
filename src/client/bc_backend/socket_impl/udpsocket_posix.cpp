#include <udpsocket.hpp>
#include <cstdint>
#include <hash.hpp>
#include <util.hpp>
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
packet::packet(const std::vector<char>& content, sockaddr_in addr, std::uint64_t id, std::uint64_t* checksum){
    this->addr = addr;
    this->content = content;
    this->id = id;
    std::copy(checksum, checksum + 4, this->checksum);
}

void packet::setContent(const std::vector<char>& _content){
	this->content = _content;
	char* id_it = (char*)&id;
    this->content.insert(this->content.end(), id_it, id_it + 8);
	hash_checksum(checksum, this->content);
    for(int i = 0;i < 8;i++)
        this->content.pop_back();
}
bool packet::valid() const{
    std::vector<char> c = content;
    std::uint64_t ch[4];
    char* id_it = (char*)&id;
    c.insert(c.end(), id_it, id_it + 8);
    hash_checksum(ch, c);
    for(unsigned int i = 0;i < 4;i++){
        if(ch[i] != checksum[i])return false;
    }
    return true;
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
    std::copy(pack.content.begin(), pack.content.end(), std::back_inserter(output));
    for(int i = 0;i < 4;i++)
        for(int ih = 7;ih >= 0;ih--)
            output.push_back((pack.checksum[i] & (0xFFULL << ih * 8)) >> (ih * 8));
    for(int ih = 7;ih >= 0;ih--)
        output.push_back((pack.id & (0xFFULL << ih * 8)) >> (ih * 8));
    
    if(sendto(s, output.data(), output.size(), 0,(const sockaddr*)&pack.addr, sizeof(pack.addr)) < 0){
        throw std::logic_error(std::string("Could not send packet: ") + strerror(errno));
    }
}
packet udpsocket::receiveP()const{
    auto data_addr = receiveFrom();
    char addrString[sizeof(data_addr.second.sin_addr)];
    inet_ntop(AF_INET, &(((struct sockaddr_in *)&data_addr.second)->sin_addr),addrString, sizeof(data_addr.second.sin_addr));
    std::vector<char> data = std::move(data_addr.first);
    std::uint64_t _id = 0;
    std::uint64_t _checksum[4] = {0};
    for(unsigned int i = 0;i < 8;i++){
        char c = data.back();
        data.pop_back();
        _id |= (((std::uint64_t)c) << (i * 8));
    }
    
    for(int i = 4 - 1;i >= 0;i--) {
        for(unsigned int ih = 0;ih < 8;ih++) {
            unsigned char c = data.back();
            data.pop_back();
            _checksum[i] |= (((std::uint64_t)c) << (ih * 8));
        }
    }
    packet pack(data, data_addr.second, _id, _checksum);
    return pack;
}
std::pair<std::vector<char>, sockaddr_in> udpsocket::receiveFrom()const{
	std::vector<char> ret(1024);
	int l = 0;
    sockaddr_in a;
    socklen_t len = sizeof(sockaddr);
	if ((l = recvfrom(s, ret.data(), 1024, 0, (sockaddr*)&a, &len)) <= 0) {
    //if ((l = read(s, ret.data(), 1024)) <= 0) {
		throw std::logic_error(std::string("Could not receive packet: ") + strerror(errno));
	}
	ret.resize(l);
	return {ret, a};
}
std::vector<char> udpsocket::receive()const{
	std::vector<char> ret(1024);
	int l = 0;
    sockaddr_in a;
    socklen_t len = sizeof(sockaddr);
	if ((l = recvfrom(s, ret.data(), 1024, 0, (sockaddr*)&a, &len)) <= 0) {
    //if ((l = read(s, ret.data(), 1024)) <= 0) {
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
