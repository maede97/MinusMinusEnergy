#ifndef SIOSTREAM_HPP
#define SIOSTREAM_HPP
#include <iostream>
#include <cstdint>
#include <vector>
#include <ios>
#include <array>
#include <string>
#include <udpsocket.hpp>
class circular_buffer{
public:
	using size_t = std::size_t;
	circular_buffer(size_t size);
	void resize(size_t size);
	struct iterator;
	struct const_iterator;
	void push_front(char c);
	void push_back(char c);
	void pop_front();
	void pop_back();
    bool full()const;
    size_t size()const;
    void clear();
	struct iterator{
		iterator(const circular_buffer* p, char* source, std::size_t period, std::size_t offset);
		const circular_buffer* parent;
		char* source;
		size_t period;
		size_t offset;
		char& operator*()const;
		iterator& operator++();
		iterator operator+(size_t)const;
		iterator& operator=(const iterator& o);
		bool operator==(const iterator& o)const;
		bool operator==(const const_iterator& o)const;
		bool operator!=(const iterator& o)const;
		bool operator!=(const const_iterator& o)const;
		iterator& operator--();
		std::ptrdiff_t operator-(const iterator& o)const;
		std::ptrdiff_t operator-(const const_iterator& o)const;
		operator const_iterator()const;
	};
	struct const_iterator{
		const_iterator(const circular_buffer* p, const char* source, std::size_t period, std::size_t offset);
		const circular_buffer* parent;
		const char* source;
		size_t period;
		size_t offset;
		const char& operator*()const;
		const_iterator& operator++();
		const_iterator operator+(size_t)const;
		const_iterator& operator=(const const_iterator& o);
		bool operator==(const iterator& o)const;
		bool operator==(const const_iterator& o)const;
		bool operator!=(const iterator& o)const;
		bool operator!=(const const_iterator& o)const;
		const_iterator& operator--();
		std::ptrdiff_t operator-(const iterator& o)const;
		std::ptrdiff_t operator-(const const_iterator& o)const;
	};
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	char& operator[](size_t i);
	const char& operator[](size_t i)const;
	
private:
	std::vector<char> m_data;
	iterator m_begin, m_end;
    bool m_full;
};

class socket_ostream{
	udpsocket socket;
    circular_buffer buffer;
    sockaddr_in target_addr;
public:
	using char_type = char;
	using char_traits = std::char_traits<char>;
	socket_ostream(const std::string& target_ip, int port, udpsocket&& sock);
	socket_ostream& put(char_type c);
	socket_ostream& operator<<(const char*);
	socket_ostream& operator<<(const std::string&);
    socket_ostream& operator<<(int);
    void sendBuffer();
    void flush();
};
namespace std{
	template<>
	struct iterator_traits<circular_buffer::iterator>{
		using difference_type = ptrdiff_t;
		using pointer = char*;
		using value_type = char;
		using reference = char&;
		using iterator_category = random_access_iterator_tag;
	};
	template<>
	struct iterator_traits<circular_buffer::const_iterator>{
		using difference_type = ptrdiff_t;
		using pointer = const char*;
		using value_type = char;
		using reference = const char&;
		using iterator_category = random_access_iterator_tag;
	};
}
#endif
