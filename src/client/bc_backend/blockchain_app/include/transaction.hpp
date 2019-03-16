#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
#include <cstdint>
#include <type_traits>
#include <keygen.hpp>
#include <gmpxx.h>
struct transaction{
    public_key<mpz_class, 2048> sender;
    public_key<mpz_class, 2048> receiver;
	std::uint64_t nonce;
    std::uint64_t amount;
};
#endif
