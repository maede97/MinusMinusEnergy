#ifndef HASH_HPP
#define HASH_HPP
#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <crypt/BigInt64.hpp>
inline std::uint64_t reverse(std::uint64_t value){
  return (value & 0x00000000000000FFULL) << 56 | (value & 0x000000000000FF00ULL) << 40 |
         (value & 0x0000000000FF0000ULL) << 24 | (value & 0x00000000FF000000ULL) << 8 |
         (value & 0x000000FF00000000ULL) >> 8 | (value & 0x0000FF0000000000ULL) >> 24 |
         (value & 0x00FF000000000000ULL) >> 40 | (value & 0xFF00000000000000ULL) >> 56;
}
void sha256_(std::uint64_t* dest, const std::vector<char>& input);
void sha512_(std::uint64_t* dest, const std::vector<char>& input);
void hash_checksum(std::uint64_t* dest, const std::vector<char>& input);
template<typename ForwardIterator>
BigInt sha256(ForwardIterator begin, ForwardIterator end){
	std::array<std::uint64_t, 4> ret;
    std::vector<typename std::iterator_traits<ForwardIterator>::value_type> rcopy(end - begin);
    std::copy(begin, end, rcopy.begin());
	std::size_t bytes = (end - begin) * sizeof(*begin);
	std::vector<char> kak(bytes);
	std::memcpy(kak.data(), (char*)rcopy.data(), bytes);
	sha256_(ret.data(), kak);
	for(std::uint64_t& x : ret)x = reverse(x);
	return BigInt(ret.begin(), ret.end());
}
template<typename ForwardIterator>
BigInt sha512(ForwardIterator begin, ForwardIterator end){
	std::array<std::uint64_t, 8> ret;
    std::vector<typename std::iterator_traits<ForwardIterator>::value_type> rcopy(end - begin);
    std::copy(begin, end, rcopy.begin());
	std::size_t bytes = (end - begin) * sizeof(*begin);
	std::vector<char> kak(bytes);
	std::memcpy(kak.data(), (char*)rcopy.data(), bytes);
	sha512_(ret.data(), kak);
	for(std::uint64_t& x : ret)x = reverse(x);
	return BigInt(ret.begin(), ret.end());
}
template<typename Iterable>
BigInt sha512(const Iterable& arg){
    return sha512(arg.begin(), arg.end());
}
template<typename Iterable>
BigInt sha256(const Iterable& arg){
    return sha256(arg.begin(), arg.end());
}
#endif
