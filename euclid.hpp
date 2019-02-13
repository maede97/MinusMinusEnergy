#ifndef EUCLID_HPP
#define EUCLID_HPP
template<typename number_t>
number_t mul_inv(number_t a, number_t b){
	number_t b0 = b, t, q;
	number_t x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = std::move(t);
	}
	if (x1 < 0) x1 += b0;
	return x1;
}
template<typename number_t, bool complete, unsigned int _size>
struct keyPair{
	number_t public_key;
	number_t private_key;
	number_t N;
	inline keyPair(number_t puk, number_t pvk, number_t _N) : public_key(std::move(puk)), private_key(std::move(pvk)), N(std::move(_N)){}
	inline keyPair(){}
	static constexpr unsigned int size = _size;
};
template<typename number_t, unsigned int _size>
struct keyPair<number_t, false, _size>{
	number_t public_key;
	number_t N;
	inline keyPair(number_t puk, number_t _N) : public_key(std::move(puk)), N(std::move(_N)){}
	inline keyPair(){}
	static constexpr unsigned int size = _size;
};
template<typename number_t, unsigned int _size>
using complete_keyPair = keyPair<number_t, true, _size>;
#endif
