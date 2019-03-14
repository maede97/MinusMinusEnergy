#include <gmp.h>
#include <gmpxx.h>
#include <keygen.hpp>
#include <iostream>
#include <functional>
#include <random>
#include <cassert>
#include <blockchain_state.hpp>
template<class T = std::mt19937_64, std::size_t N = T::state_size>
auto ProperlySeededRandomEngine () -> typename std::enable_if<!!N, T>::type {
    typename T::result_type random_data[N];
    std::random_device source;
    std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
    std::seed_seq seeds(std::begin(random_data), std::end(random_data));
    T seededEngine (seeds);
    return seededEngine;
}
gmp_randstate_t state;
mpz_class random_mpz(unsigned int size){
	mpz_class rnumber;
	mpz_urandomb(rnumber.get_mpz_t(), state, size);
	mpz_class rnumber2;
	mpz_nextprime(rnumber2.get_mpz_t(), rnumber.get_mpz_t());
	return rnumber2;
}
template<unsigned int size = 2048>
complete_keyPair<mpz_class, size> generateKeypair(){
	static_assert(!((size) & (size - 1)), "Size must be a power of two");
	mpz_class a = random_mpz(size / 2), b = random_mpz(size / 2);
	mpz_class N = a * b;
	mpz_class phi_N = (a - 1) * (b - 1);
	mpz_class public_key = random_mpz(size / 2);
	mpz_class private_key = mul_inv(public_key, phi_N);
	return complete_keyPair<mpz_class, size>(std::move(public_key), std::move(private_key), std::move(N));
}
template<typename number_t, bool complete, unsigned int size>
std::vector<char> encrypt(const keyPair<number_t, complete, size>& key, const std::string& text){
	return encrypt(key, std::vector<char>(text.begin(), text.end()));
}
template<typename number_t, bool complete, unsigned int size>
std::vector<char> encrypt(const keyPair<number_t, complete, size>& key, const std::vector<char>& text){
	static_assert(!((size) & (size - 1)), "Size must be a power of two");
	assert(text.size() > 0);
	std::vector<char> expanded((size / 8) * ((text.size() - 1) / (size / 8) + 1), 'a');
	for(unsigned int i = 0;i < text.size();i++){
		expanded[i] = text[i];
	}
	
	std::vector<char> chiffrat (expanded.size(), 'a');
	for(unsigned int i = 0;i < expanded.size();i += size / 8){
		mpz_class chif;
		mpz_import(chif.get_mpz_t(), size / 8, 1, sizeof(decltype(expanded[0])), 0, 0, expanded.data() + i);
		mpz_class exc;
		mpz_powm (exc.get_mpz_t(), chif.get_mpz_t(), key.public_key.get_mpz_t(), key.N.get_mpz_t());
		size_t _size = (mpz_sizeinbase (exc.get_mpz_t(), 2) + CHAR_BIT - 1) / CHAR_BIT;
		std::cout << _size << std::endl;
		mpz_export(chiffrat.data() + i, &_size, 1, 1, 0, 0, exc.get_mpz_t());
	}
	
	return std::move(chiffrat);
}
int main(){
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, 42);
	
	std::mt19937_64 gen = ProperlySeededRandomEngine();
	complete_keyPair<mpz_class, 2048> kp = generateKeypair();
	std::vector<char> randData(8192 << 4);
	std::generate(randData.begin(), randData.end(), std::ref(gen));
	std::vector<char> f = encrypt(kp, randData);
	for(int i = 0;i < f.size();i++){
		//std::cout << (int)f[i] << ", ";
	}
	std::cout << "\n";
	gmp_randclear(state);
	return 0;
}
