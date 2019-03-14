#ifndef INTRIN_HPP
#define INTRIN_HPP
#include <cstdint>
#ifdef __clang__
#undef _MSC_VER
#endif
#ifdef _DEF_INT128
#error _DEF_INT128 cannot be directly set
#elif defined(__SIZEOF_INT128__)
#if (defined(__clang__) && !defined(_WIN32) && !defined(__aarch64__)) || \
    (defined(__CUDACC__) && __CUDACC_VER_MAJOR__ >= 9) ||                \
    (defined(__GNUC__) && !defined(__clang__) && !defined(__CUDACC__))
#define _DEF_INT128 1
#elif defined(__CUDACC__)
#if __CUDACC_VER__ >= 70000
#define _DEF_INT128 1
#endif
#endif
#endif
#if defined(__clang__) && !defined(_DEF_INT128)
#define _DEF_INT128
#endif
#ifndef _DEF_INT128
#include "uint128_t.h"
using uint_128bit = uint128_t;
#else
using uint_128bit = unsigned __int128;
#endif
#if defined(__GNUC__) || defined(__clang__)
inline int _leading_zeros(unsigned long long x){
	return __builtin_clzll(x);
}
inline int _trailing_zeros(unsigned long long x){
	return __builtin_ctzll(x);
}
inline bool _adc_u64(unsigned long long a,unsigned long long b,unsigned long long* c){
	return __builtin_uaddll_overflow(a, b, c);
}
inline bool _sbc_u64(unsigned long long a,unsigned long long b,unsigned long long* c){
	 return __builtin_usubll_overflow(a, b, c);
}
inline unsigned long long mulx_u64(unsigned long long a, unsigned long long b, unsigned long long* hi){
	__uint128_t r = ((__uint128_t)a) * b;
	*hi = (unsigned long long)(r >> 64);
	return r;
}
#elif defined(_MSC_VER)
#include <intrin.h>
inline int _leading_zeros(unsigned long long x){
	int index = 0;
	_BitScanForward64((unsigned long*)&index, x);
	return index;
}
inline int _trailing_zeros(unsigned long long x){
	int index = 0;
	_BitScanReverse64((unsigned long*)&index, x);
	return index;
}
inline bool _adc_u64(unsigned long long a,unsigned long long b,unsigned long long* c){
	return _addcarry_u64(0, a, b, c);
}
inline bool _sbc_u64(unsigned long long a,unsigned long long b,unsigned long long* c){
	 *c = a - b;
	 return b > a;
}
inline unsigned long long mulx_u64(unsigned long long a, unsigned long long b, unsigned long long* hi){
	return _mulx_u64(a,b,hi);
}
#else

#error Your compiler is neither GNU nor Clang nor MSVC

#endif
#endif //INTRIN_HPP
