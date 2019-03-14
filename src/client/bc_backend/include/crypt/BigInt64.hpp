#ifndef BIGINT64_HPP
#define BIGINT64_HPP
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <limits>
#include <initializer_list>
#include <deque>
#include <vector>
#include <string>
#include <bitset>
#include <array>
#include <iostream>
#include "intrin.hpp"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
constexpr static std::array<char, 16> chars = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
template<typename T>
inline int signum(T t){
	if(t < 0)return -1;
	if(t >= 0)return 1;
    assert(false && "t is neither >= nor < 0");
	return 0;
}
inline void singleHex(char dest[], uint64_t nr){
	uint64_t mask = 0xf000000000000000;
	for(unsigned int i = 0;i < 16;i++){
		dest[i] = chars[(nr & mask) >> (4 * (15 - i))];
		mask >>= 4;
	}
	dest[16] = 0;
}
struct BigInt{
	using lui = ::uint_128bit;
	using size_t = std::size_t;
	using ssize_t = std::int64_t;
	using uint64_t = std::uint64_t;
	using uint32_t = std::uint32_t;
	using iterator = std::deque<uint64_t>::iterator;
	using const_iterator = std::deque<uint64_t>::const_iterator;
	using reverse_iterator = std::deque<uint64_t>::reverse_iterator;
	using const_reverse_iterator = std::deque<uint64_t>::const_reverse_iterator;
	std::deque<uint64_t> data;
	int signum;
	inline BigInt() : data(1,0),signum(1){}
	inline BigInt(size_t _s, uint64_t fill) : data(_s, fill), signum(1){}
	inline BigInt(int a) :  data(1, std::abs(a)),signum(::signum(a)){}
	inline BigInt(unsigned int a) : data(1, a),signum(1){}
	inline BigInt(unsigned long long a) : data(1, a), signum(1){}
	inline BigInt(long long a) : data(1, std::abs(a)),signum(::signum(a)){}
	inline BigInt(const std::initializer_list<uint64_t>& l) : data(l), signum(1){}
	inline BigInt(std::initializer_list<uint64_t>&& l) : data(std::move(l)), signum(1){}
	inline BigInt(const BigInt& o) : data(o.data), signum(o.signum){}
	inline BigInt(BigInt&& o) : data(std::move(o.data)), signum(o.signum){}
	template<typename InputIterator>
	inline BigInt(InputIterator begin, InputIterator end) : data(begin, end), signum(1){}
	template<typename RNG>
	inline BigInt(RNG& rng, size_t length) : data(length, 0), signum(1){std::generate(data.begin(),data.end(), [&rng](){return rng();});}
	inline std::deque<uint64_t>::iterator begin(){return data.begin();}
	inline std::deque<uint64_t>::iterator end(){return data.end();}
	inline std::deque<uint64_t>::reverse_iterator rbegin(){return data.rbegin();}
	inline std::deque<uint64_t>::reverse_iterator rend(){return data.rend();}
	inline std::deque<uint64_t>::const_iterator begin()const{return data.begin();}
	inline std::deque<uint64_t>::const_iterator end()const{return data.end();}
	inline std::deque<uint64_t>::const_reverse_iterator rbegin()const{return data.rbegin();}
	inline std::deque<uint64_t>::const_reverse_iterator rend()const{return data.rend();}
	inline std::deque<uint64_t>::const_iterator cbegin()const{return data.cbegin();}
	inline std::deque<uint64_t>::const_iterator cend()const{return data.cend();}
	inline std::deque<uint64_t>::const_reverse_iterator crbegin()const{return data.crbegin();}
	inline std::deque<uint64_t>::const_reverse_iterator crend()const{return data.crend();}
	inline uint64_t& operator[](size_t i){return data[i];}
	inline const uint64_t& operator[](size_t i)const{return data.at(i);}
	inline uint64_t& at(size_t i){return data[i];}
	inline const uint64_t& at(size_t i)const{return data.at(i);}
	inline size_t size()const{return data.size();}
	inline BigInt& operator=(const BigInt& o){signum = o.signum;data = o.data;return *this;}
	inline BigInt& operator=(BigInt&& o){data = std::move(o.data);signum = o.signum;return *this;}
	inline BigInt(const std::string& o){
		signum = 1;
		auto it = o.rbegin();
		BigInt baseTen(1);
		while(it != o.rend()){
			if(*it == '-'){
				assert(it != o.rbegin());
				assert((++it) == o.rend());
				signum = -1;
				break;
			}
			BigInt l = baseTen.mult(BigInt(*it - '0'));
			adda(l);
			baseTen = baseTen.mult(BigInt(10));
			++it;
		}
		trim();
	}
	inline size_t bitscanForward()const{
		auto it = begin();
		size_t s = 0;
		while(*it == 0 && it != end()){
			++it;
			s += 64;
		}
		if(it == end())return s + 64;
		return s + _leading_zeros(*it);
	}
	inline size_t bitscanReverse()const{
		auto it = rbegin();
		size_t s = 0;
		while(*it == 0 && it != rend()){
			++it;
			s += 64;
		}
		if(it == rend())return s + 64;
		return s + _trailing_zeros(*it);
	}
	
	inline bool operator<(const BigInt& o)const{
		if(signum > o.signum){
			return false;
		}
		if(signum < o.signum){
			return true;
		}
		const_iterator it1 = begin();
		const_iterator it2 = o.begin();
		if(size() > o.size()){
			it1 += size() - o.size();
			auto _t = begin();
			while(_t != it1){
				if(*_t)return false;
				++_t;
			}
		}
		else if(size() < o.size()){
			it2 += o.size() - size();
			auto _t = o.begin();
			while(_t != it2){
				if(*_t)return true;
				++_t;
			}
		}
		while(it1 != end() && it2 != o.end()){
			if(*(it1) < *(it2))return true;
			if(*(it1) > *(it2))return false;
			++it1;++it2;
		}
		return false;
	}
	inline bool operator>(const BigInt& o)const{
		if(signum < o.signum){
			return false;
		}
		if(signum > o.signum){
			return true;
		}
		const_iterator it1 = begin();
		const_iterator it2 = o.begin();
		if(size() > o.size()){
			it1 += size() - o.size();
			auto _t = begin();
			while(_t != it1){
				if(*_t)return true;
				++_t;
			}
		}
		else if(size() < o.size()){
			it2 += o.size() - size();
			auto _t = o.begin();
			while(_t != it2){
				if(*_t)return false;
				++_t;
			}
		}
		while(it1 != end() && it2 != o.end()){
			if(*(it1) > *(it2))return true;
			if(*(it1) < *(it2))return false;
			++it1;++it2;
		}
		return false;
	}
	inline bool operator<=(const BigInt& o)const{
		if(signum > o.signum){
			return false;
		}
		if(signum < o.signum){
			return true;
		}
		const_iterator it1 = begin();
		const_iterator it2 = o.begin();
		if(size() > o.size()){
			it1 += size() - o.size();
			auto _t = begin();
			while(_t != it1){
				if(*_t)return true;
				++_t;
			}
		}
		else if(size() < o.size()){
			it2 += o.size() - size();
			auto _t = o.begin();
			while(_t != it2){
				if(*_t)return false;
				++_t;
			}
		}
		while(it1 != end() && it2 != o.end()){
			if(*(it1) < *(it2))return true;
			if(*(it1) > *(it2))return false;
			++it1;++it2;
		}
		return true;
	}
	inline bool operator>=(const BigInt& o)const{
		if(signum < o.signum){
			return false;
		}
		if(signum > o.signum){
			return true;
		}
		const_iterator it1 = begin();
		const_iterator it2 = o.begin();
		if(size() > o.size()){
			it1 += size() - o.size();
			auto _t = begin();
			while(_t != it1){
				if(*_t)return true;
				++_t;
			}
		}
		else if(size() < o.size()){
			it2 += o.size() - size();
			auto _t = o.begin();
			while(_t != it2){
				if(*_t)return false;
				++_t;
			}
		}
		while(it1 != end() && it2 != o.end()){
			if(*(it1) > *(it2))return true;
			if(*(it1) < *(it2))return false;
			++it1;++it2;
		}
		return true;
	}
	inline bool operator==(const BigInt& o)const{
		if(signum < o.signum){
			return false;
		}
		if(signum > o.signum){
			return false;
		}
		const_iterator it1 = begin();
		const_iterator it2 = o.begin();
		if(size() > o.size()){
			it1 += size() - o.size();
			auto _t = begin();
			while(_t != it1){
				if(*_t)return false;
				++_t;
			}
		}
		else if(size() < o.size()){
			it2 += o.size() - size();
			auto _t = o.begin();
			while(_t != it2){
				if(*_t)return false;
				++_t;
			}
		}
		while(it1 != end() && it2 != o.end()){
			if(*(it1) != *(it2))return false;
			++it1;++it2;
		}
		return true;
	}
	inline bool operator==(uint64_t o){
		return size() == 1 && *rbegin() == o;
	}
	inline bool isZero()const{
		for(auto it = data.begin();it != data.end();it++){
			if(*it)return false;
		}
		return true;
	}
	inline void setZero(){
		for(auto it = begin();it != end();it++)*it = 0;
	}
	
	inline BigInt& trim(){
		while(data[0] == 0 && data.size() > 1){
			data.pop_front();
		}
		return *this;
	}
	
	inline BigInt div(uint64_t d)const{
		BigInt ret = *this;
		lui carry = 0;
		for(auto it = ret.data.begin();it != ret.data.end();it++){
			lui temp = (lui)*it;
			temp += carry;
			carry = temp % d;
			*it = (uint64_t)(temp / d);
			carry <<= 64;
		}
		return ret;
	}
	inline BigInt& div(uint64_t d){
		lui carry = 0;
		for(auto it = data.begin();it != data.end();it++){
			lui temp = (lui)*it;
			temp += carry;
			carry = temp % d;
			*it = (uint64_t)(temp / d);
			carry <<= 64;
		}
		return *this;
	}
	inline uint64_t mod(uint64_t m)const{
		lui carry = 0;
		for(auto it = data.begin();it != data.end();it++){
			carry <<= 64;
			carry = (*it + carry) % m;
		}
		return carry;
	}
	
	inline BigInt& bitshiftLeft(int c){
		if(c < 0)return bitshiftRight(-c);
		unsigned int sh = c % 64;
		unsigned int jmp = c / 64;
		if((unsigned int)jmp >= size()){std::fill(begin(),end(),0);return *this;}
		auto it1 = begin(); 
		auto it2 = it1 + jmp;
		auto beforeEnd = end() - 1;
		while(it2 != beforeEnd){
			*it1 = (*it2 << sh);
			if(sh)
			*it1 |= (*(it2 + 1) >> (64 - sh));
			++it1;++it2;
		}
		*it1 = (*it2 << sh);
		++it1;++it2;
		while(it1 != end()){
			*it1 = 0;
			++it1;
		}
		return *this;
	}
	
	inline BigInt& bitshiftLeft_expand(int c){
		if(c < 0)return bitshiftRight(-c);
		unsigned int sh = c % 64;
		unsigned int jmp = c / 64;
		int insert_count = std::max((size_t)0, jmp - bitscanForward() / 64 + 1);
		while(insert_count --> 0)
			data.push_front(0);
		auto it1 = begin(); 
		auto it2 = it1 + jmp;
		auto beforeEnd = end() - 1;
		while(it2 != beforeEnd){
			*it1 = (*it2 << sh);
			if(sh)
			*it1 |= (*(it2 + 1) >> (64 - sh));
			++it1;++it2;
		}
		*it1 = (*it2 << sh);
		++it1;++it2;
		while(it1 != end()){
			*it1 = 0;
			++it1;
		}
		return *this;
	}
	
	inline BigInt& bitshiftRight(int c){
		if(c < 0)return bitshiftLeft(-c);
		unsigned int sh = c % 64;
		unsigned int jmp = c / 64;
		if((unsigned int)jmp >= size()){std::fill(begin(),end(),0);return *this;}
		auto it1 = rbegin(); 
		auto it2 = it1 + jmp;
		auto beforeRend = rend() - 1;
		while(it2 != beforeRend){
			*it1 = (*it2 >> sh);
			if(sh)
			*it1 |= (*(it2 + 1) << (64 - sh));
			++it1;++it2;
		}
		*it1 = (*it2 >> sh);
		++it1;++it2;
		while(it1 != rend()){
			*it1 = 0;
			++it1;
		}
		return *this;
	}
	inline BigInt operator<<(int c)const{
		BigInt ret = *this;
		ret.bitshiftLeft(c);
		return ret;
	}
	
	inline BigInt operator>>(int c)const{
		BigInt ret = *this;
		ret.bitshiftRight(c);
		return ret;
	}
	
	inline BigInt& operator<<=(int c){
		return bitshiftLeft(c);
	}
	
	inline BigInt& operator>>=(int c){
		return bitshiftRight(c);
	}
	
	inline BigInt& operator&=(const BigInt& o){
		auto it1 = rbegin();
		auto it2 = o.rbegin();
		while(it1 != rend() && it2 != o.rend())
			*(it1++) &= *(it2++);
		return *this;
	}
	inline BigInt& operator|=(const BigInt& o){
		auto it1 = rbegin();
		auto it2 = o.rbegin();
		while(it1 != rend() && it2 != o.rend())
			*(it1++) |= *(it2++);
		return *this;
	}
	inline BigInt& operator^=(const BigInt& o){
		auto it1 = rbegin();
		auto it2 = o.rbegin();
		while(it1 != rend() && it2 != o.rend())
			*(it1++) ^= *(it2++);
		return *this;
	}
	inline BigInt operator&(const BigInt& o)const{
		BigInt ret(*this);
		ret &= o;
		return ret;
	}
	inline BigInt operator|(const BigInt& o)const{
		BigInt ret(*this);
		ret |= o;
		return ret;
	}
	inline BigInt operator^(const BigInt& o)const{
		BigInt ret(*this);
		ret ^= o;
		return ret;
	}
	inline BigInt& cut(size_t chunks){
		while(size() > chunks)data.pop_front();
		return *this;
	}
	inline int bitDifference(const BigInt& o){
		int pos1(0),pos2(0);
		auto it1 = begin();
		auto it2 = o.begin();
		bool bitfound = false;
		while(it1 != end()){
			if(*it1 == 0)
				pos1 += 64;
			else{
				pos1 += _leading_zeros(*it1);
				bitfound = true;
				break;
			}
			++it1;
		}
		if(!bitfound)pos1 = size() * 64;
		bitfound = false;
		while(it2 != o.end()){
			if(*it2 == 0)
				pos2 += 64;
			else{
				pos2 += _leading_zeros(*it2);
				bitfound = true;
				break;
			}
			++it2;
		}
		if(!bitfound)pos2 = o.size() * 64;
		int sizediff = ((signed int)size()) - ((signed int)o.size());
		return pos2 - pos1 + sizediff * 64;
	}
	inline BigInt& chunkshiftLeft(int c){
		if(c < 0)return chunkshiftRight(-c);
		if((unsigned int)c >= size()){std::fill(begin(),end(),0);return *this;}
		auto it1 = data.begin(); 
		auto it2 = it1 + c;
		while(it2 != data.end())*(it1++) = *(it2++);
		while(it1 != data.end())*(it1++) = 0;
		return *this;
	}
	
	inline BigInt& chunkshiftRight(int c){
		if(c < 0)return chunkshiftLeft(-c);
		if((unsigned int)c >= size()){std::fill(begin(),end(),0);return *this;}
		auto it1 = data.rbegin(); 
		auto it2 = it1 + c;
		while(it2 != data.rend())*(it1++) = *(it2++);
		while(it1 != data.rend())*(it1++) = 0;
		return *this;
	}
	inline BigInt bitshiftLeft(int c)const{
		if(c < 0)return bitshiftRight(-c);
		BigInt _this = *this;
		_this.bitshiftLeft(c);
		return _this;
	}
	inline BigInt bitshiftRight(int c)const{
		if(c < 0)return bitshiftLeft(-c);
		BigInt _this = *this;
		_this.bitshiftRight(c);
		return _this;
	}
	inline BigInt chunkshiftLeft(int c)const{
		if(c < 0)return chunkshiftRight(-c);
		BigInt _this = *this;
		_this.chunkshiftLeft(c);
		return _this;
	}
	inline BigInt chunkshiftRight(int c)const{
		if(c < 0)return chunkshiftLeft(-c);
		BigInt _this = *this;
		_this.chunkshiftRight(c);
		return _this;
	}
	inline BigInt add(const BigInt& o){
		BigInt ret = *this;
		ret.adda(o);
		return ret;
	}
	
	inline BigInt& adda(const BigInt& o){
		while(size() < o.size())data.push_front(0);
		bool carry = 0;
		auto it1 = rbegin();
		auto it2 = o.rbegin();
		while(it1 != rend() && it2 != o.rend()){
			carry = _adc_u64(*it1, *it2 + carry, (unsigned long long*)(&(*it1)));
			carry |= (*it2 + carry) == 0 && *it2;
			++it1;
			++it2;
		}
		while(it1 != rend() && carry){
			carry = _adc_u64(*it1, carry, (unsigned long long*)(&(*it1)));
			++it1;
		}
		if(carry)data.push_front(1);
		return *this;
	}
	inline BigInt& suba(const BigInt& o){
		assert((*this) >= (o));
		bool carry = 0;
		auto it1 = rbegin();
		auto it2 = o.rbegin();
		while(it1 != rend() && it2 != o.rend()){
			carry = _sbc_u64(*it1 - carry, *it2, (unsigned long long*)(&(*it1)));
			if(!carry)carry = ((*it1 - carry) == std::numeric_limits<uint64_t>::max());
			++it1;++it2;
		}
		while(it1 != rend() && carry){
			carry = _sbc_u64(*it1, carry, (unsigned long long*)(&(*it1)));
			++it1;
		}
		return *this;
	}
	inline BigInt& moda(const BigInt& o){
		assert(!o.isZero());
		if(o > *this)return *this;
		BigInt mo = o;
		mo = o;
		int bd = bitDifference(o);
		mo.bitshiftLeft_expand(bd);
		if(mo > *this)mo.bitshiftRight(1);
		this->suba(mo);
		if(*this < o)return *this;
		while(true){
			bd = mo.bitDifference(*this);
			mo >>= bd;
			mo.trim();
			if(mo > *this)mo >>= 1;
			this->suba(mo);
			if(*this < o)return *this;
		}
		return *this;
	}
	inline bool even(){
		return !(*rbegin() & 1);
	}
	inline BigInt modPow(BigInt o, const BigInt& mod)const{
		BigInt t = *this;
		BigInt odd(1);
		while(true){
			if(o.even()){
				t = t.mult(t);
				t.trim();
				t.moda(mod);
				o.div(2);
			}
			else{
				odd = odd.mult(t);
				odd.moda(mod);
				odd.trim();
				t.trim();
				t.moda(mod);
				o.suba(BigInt(1));
			}
			//::cout << "Reduced" << std::endl;
			o.trim();
			if(o == 1){t = t.mult(odd);t.moda(mod);return t;}
		}
	}
	inline BigInt mult(const BigInt& o)const{
		BigInt result(size() + o.size() + 1,0);
		BigInt temp(size() + o.size() + 1,0);
		int p = 0;
		for(auto it1 = rbegin();it1 != rend();it1++){
			auto it = temp.rbegin();
			uint64_t carry = 0;
			for(auto it2 = o.rbegin();it2 != o.rend();it2++){
				uint64_t hi;
				uint64_t lo = mulx_u64(*it1, *it2, (unsigned long long*)&hi);
				bool ac = _adc_u64(lo, carry, (unsigned long long*)(&(*(it++))));
				carry = hi + ac;
			}
			if(carry)(*it) += carry;
			temp.chunkshiftLeft(p++);
			result.adda(temp);
			temp.setZero();
		}
		result.trim();
		return result;
	}
	
	inline BigInt multOld(const BigInt& o)const{
		BigInt result(size() + o.size() + 1,0);
		BigInt temp(size() + o.size() + 1,0);
		int p = 0;
		for(auto it1 = rbegin();it1 != rend();it1++){
			auto it = temp.rbegin();
			lui carry = 0;
			for(auto it2 = o.rbegin();it2 != o.rend();it2++){
				lui prod = ((lui)*it1) * (*it2);
				prod += carry;
				*(it++) = (uint64_t)prod;
				carry = (prod >> 64);
			}
			if(carry)(*it) += carry;
			temp.chunkshiftLeft(p++);
			result.adda(temp);
			temp.setZero();
		}
		result.trim();
		return result;
	}
	
	inline std::string rawString(bool flag = false)const{
		std::string s = "";
		for(unsigned int i = 0;i < size();i++){
			if(data[i])flag = true;
			if(flag)
				s += data[i] == 0 ? "0" : std::to_string(data[i]);
			if(flag)if(i < size() - 1)s += "|";
		}
		return s;
	}
	inline std::string bitString()const{
		auto it = begin();
		std::string ret = "";
		//std::cout << size() << "; " << std::flush;
		while(it != end()){
			std::bitset<64> bits(*it);
			for(unsigned int i = 0;i < 64;i++){
				ret += chars.at(((*it) & (1ULL << (64 - i))) != 0);
			}
			++it;
		}
		return ret;
	}
	inline std::string hexString()const{
		auto it = begin();
		bool flag = false;
		std::string ret = "";
		while(it != end()){
			uint64_t curr(*it);
			if(curr == 0){++it;continue;}
			char ccurr[17];
			singleHex(ccurr, curr);
			if(flag){
				ret += ccurr;
				++it;
				continue;
			}
			flag = true;
			size_t start = 0;
			while(ccurr[start] == '0')++start;
			ret += (char*)(ccurr + start);
			++it;
		}
		return ret;
	}
	inline std::string toString()const{
		if(isZero())return std::to_string(0);
		std::deque<char> c_str;
		const uint64_t q = 1000000000000000000ULL;
		BigInt diver = *this;
		while(!diver.isZero()){
			std::string frac = std::to_string(diver.mod(q));
			int a = 0;
			for(auto it = frac.rbegin();it != frac.rend();it++){
				c_str.push_front(*it);
				a++;
			}
			while(a < 18){
				c_str.push_front('0');
				a++;
			}
			diver.div(q);
		}
		auto it = c_str.begin();
		while(*(it) == '0')++it;
		return std::string(it, c_str.end());
	}
	
	inline std::string toString(unsigned int base)const{
		if(isZero())return std::to_string(0);
		if(base == 2)return bitString();
		if(base == 10)return toString();
		if(base == 16)return hexString();
		std::vector<char> c_str;
		c_str.reserve(size() * (unsigned int)(64.0 * std::log(2) / std::log((double)base)));
		BigInt diver = *this;
		while(!diver.isZero()){
			c_str.push_back(chars.at(diver.mod(base)));
			diver.div(base);
		}
		return std::string(c_str.rbegin(), c_str.rend());
	}
};
namespace std{
	template<>
	struct hash<BigInt>{
		inline size_t operator()(const BigInt& o)const{
			size_t ret = 0;
			std::for_each(o.begin(), o.end(), [&ret](const uint64_t& ui){ret ^= ui;});
			return ret;
		}
	};
}
const static BigInt secure_prime("25517712857249265246309662191040714920292930135958602873503082695880945015180270627160886016284304866241119009429935511497986916016509065559298646199688497746399172174316028774533924795864096565081478741603241830675436336762053778667047857025632695617746551090247164369324008907433218665135569658200641651876344533506145721941113011977317356006176781796659698883765657005845351846184505291996942442336931455986790727248315517902731173678888064950798931396279140373592203530274617983159864665935475637811846793653407441533829095478201308785445059955697867933027578011378694502392722655274554801068451419037021634697683");
#endif //BIGINT64_HPP
