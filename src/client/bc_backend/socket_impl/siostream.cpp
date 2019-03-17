#include <siostream.hpp>
#include <cassert>
#include <algorithm>
using std::size_t;
socket_ostream::socket_ostream(const std::string& target_ip,int port, udpsocket&& sock) : socket(std::move(sock)), buffer(1024){
    hostent *server_host;
	errno = 0;
	server_host = gethostbyname(target_ip.c_str());
	if(errno != 0){
		throw std::logic_error("Could not resolve hostname " + target_ip + ": " + strerror(errno));
	}
	/* configure the server address */
	target_addr.sin_family = AF_INET; // IPv4
	memcpy(&target_addr.sin_addr, server_host->h_addr,
			sizeof(struct in_addr)); 
			target_addr.sin_port = htons(port);
}
circular_buffer::circular_buffer(size_t size) :  m_data(size), m_begin(this, m_data.data(), size, 0), m_end(this, m_data.data(), size, 0), m_full(false){
	
}
circular_buffer::iterator::iterator(const circular_buffer* p, char* _source, size_t _period, size_t _offset):parent(p), source(_source),period(_period), offset(_offset){
	
}
circular_buffer::const_iterator::const_iterator(const circular_buffer* p, const char* _source, size_t _period, size_t _offset):parent(p), source(_source),period(_period), offset(_offset){
	
}
void circular_buffer::resize(size_t size){
	std::vector<char> newdata(size);
    assert(m_end - m_begin > 0);
	size_t os = (size_t)(m_end - m_begin);
	if(size >= os)
		std::copy(m_begin, m_begin, newdata.begin());
	else
		std::copy(m_begin, m_begin + size, newdata.begin());
	std::swap(m_data, newdata);
	m_begin = iterator(this, m_data.data(),size,0);
	m_end = iterator(this, m_data.data(),size, os);
}
void circular_buffer::clear(){
    m_begin.offset = 0;
    m_end.offset = 0;
}
char& circular_buffer::iterator::operator*()const{
	return *(source + offset);
}
const char& circular_buffer::const_iterator::operator*()const {
	return *(source + offset);
}
circular_buffer::iterator& circular_buffer::iterator::operator++(){
	++offset;
	if(offset == period)offset = 0;
	return *this;
}
circular_buffer::const_iterator& circular_buffer::const_iterator::operator++(){
	++offset;
	if(offset == period)offset = 0;
	return *this;
}
circular_buffer::iterator circular_buffer::iterator::operator+(size_t leap)const{
	return circular_buffer::iterator(parent, source, period, (offset + leap) % period);
}
circular_buffer::const_iterator circular_buffer::const_iterator::operator+(size_t leap)const{
	return circular_buffer::const_iterator(parent, source, period, (offset + leap) % period);
}
std::ptrdiff_t circular_buffer::iterator::operator-(const iterator& o)const{
	using ui = std::ptrdiff_t;
	assert(parent == o.parent);
	ui so = parent->m_begin.offset;
	ui new_this_offset = ((ui)(this->offset) - so) % period;
	ui new_o_offset = ((ui)(o.offset) - so) % period;
	return new_this_offset - new_o_offset;
}
std::ptrdiff_t circular_buffer::iterator::operator-(const const_iterator& o)const{
	using ui = std::ptrdiff_t;
	assert(parent == o.parent);
	ui so = parent->m_begin.offset;
	ui new_this_offset = ((ui)(this->offset) - so) % period;
	ui new_o_offset = ((ui)(o.offset) - so) % period;
	return new_this_offset - new_o_offset;
}
std::ptrdiff_t circular_buffer::const_iterator::operator-(const iterator& o)const{
	using ui = std::ptrdiff_t;
	assert(parent == o.parent);
	ui so = parent->m_begin.offset;
	ui new_this_offset = ((ui)(this->offset) - so) % period;
	ui new_o_offset = ((ui)(o.offset) - so) % period;
	return new_this_offset - new_o_offset;
}
std::ptrdiff_t circular_buffer::const_iterator::operator-(const const_iterator& o)const{
	using ui = std::ptrdiff_t;
	assert(parent == o.parent);
	ui so = parent->m_begin.offset;
	ui new_this_offset = ((ui)(this->offset) - so) % period;
	ui new_o_offset = ((ui)(o.offset) - so) % period;
	return new_this_offset - new_o_offset;
}
char& circular_buffer::operator[](size_t i){
	return m_data[(m_begin.offset + i) % m_data.size()];
}
const char& circular_buffer::operator[](size_t i)const{
	return m_data[(m_begin.offset + i) % m_data.size()];
}
	
circular_buffer::iterator& circular_buffer::iterator::operator=(const circular_buffer::iterator& o){
	parent = o.parent;
	source = o.source;
	period = o.period;
	offset = o.offset;
    return *this;
}
circular_buffer::const_iterator& circular_buffer::const_iterator::operator=(const circular_buffer::const_iterator& o){
	parent = o.parent;
	source = o.source;
	period = o.period;
	offset = o.offset;
    return *this;
}
bool circular_buffer::iterator::operator!=(const iterator& o)const{
	return source != o.source || parent != o.parent || period != o.period || offset != o.offset;
}
bool circular_buffer::iterator::operator!=(const const_iterator& o)const{
	return source != o.source || parent != o.parent || period != o.period || offset != o.offset;
}
bool circular_buffer::const_iterator::operator!=(const iterator& o)const{
	return source != o.source || parent != o.parent || period != o.period || offset != o.offset;
}
bool circular_buffer::const_iterator::operator!=(const const_iterator& o)const{
	return source != o.source || parent != o.parent || period != o.period || offset != o.offset;
}
bool circular_buffer::iterator::operator==(const iterator& o)const{
	return source == o.source && parent == o.parent && period == o.period && offset == o.offset;
}
bool circular_buffer::iterator::operator==(const const_iterator& o)const{
	return source == o.source && parent == o.parent && period == o.period && offset == o.offset;
}
bool circular_buffer::const_iterator::operator==(const iterator& o)const{
	return source == o.source && parent == o.parent && period == o.period && offset == o.offset;
}
bool circular_buffer::const_iterator::operator==(const const_iterator& o)const{
	return source == o.source && parent == o.parent && period == o.period && offset == o.offset;
}
circular_buffer::iterator& circular_buffer::iterator::operator--(){
	if(offset == 0)
		offset = period;
	--offset;
    return *this;
}
circular_buffer::const_iterator& circular_buffer::const_iterator::operator--(){
	if(offset == 0)
		offset = period;
	--offset;
    return *this;
}
circular_buffer::iterator::operator const_iterator()const{
	return const_iterator(parent, source, period, offset);
}
circular_buffer::iterator circular_buffer::begin(){
	return m_begin;
}
circular_buffer::const_iterator circular_buffer::begin() const{
	return circular_buffer::const_iterator(m_begin.parent, m_begin.source, m_begin.period, m_begin.offset);
}
circular_buffer::iterator circular_buffer::end(){
	return m_end;
}
circular_buffer::const_iterator circular_buffer::end()const{
	return circular_buffer::const_iterator(m_end.parent, m_end.source, m_end.period, m_end.offset);
}
void circular_buffer::push_front(char c){
	--m_begin;
	*m_begin = c;
	if(m_begin.offset == m_end.offset){
		m_full = true;
	}
}
bool circular_buffer::full()const{
    return m_full;
}
size_t circular_buffer::size()const{
    return m_end - m_begin;
}
void circular_buffer::push_back(char c){
    if(m_end + 1 == m_begin){
        resize(size() * 2);
    }
	*m_end = c;
	++m_end;
	if(m_begin.offset == m_end.offset){
		m_full = true;
	}
}
void circular_buffer::pop_front(){
	++m_begin;
	m_full = false;
}
void circular_buffer::pop_back(){
	--m_end;
	m_full = false;
}
void socket_ostream::flush(){
    sendBuffer();
    buffer.clear();
}
void socket_ostream::sendBuffer(){
    std::string s(buffer.begin(), buffer.end());
    std::cout << s;
}
socket_ostream& socket_ostream::put(char_type c){
	buffer.push_back(c);
    return *this;
}
socket_ostream& socket_ostream::operator<<(const char* c){
	size_t i = 0;
	while(true){
		buffer.push_back(c[i++]);
        if(buffer.full()){
            sendBuffer();
            buffer.clear();
        }
	}
	return *this;
}
socket_ostream& socket_ostream::operator<<(const std::string& s){
	size_t i = 0;
	while(true){
		buffer.push_back(s[i++]);
        if(buffer.full()){
            sendBuffer();
            buffer.clear();
        }
	}
	return *this;
}
socket_ostream& socket_ostream::operator<<(int){
	
	return *this;
}
