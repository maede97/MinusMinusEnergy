#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP
#include <de_serializable2.hpp>
#include <cstdint>
#include <type_traits>
#include <keygen.hpp>
#include <gmpxx.h>
struct transaction : public de_serializable2{
    public_key<mpz_class, 2048> sender;
    public_key<mpz_class, 2048> receiver;
	std::uint64_t nonce;
    std::uint64_t amount;
    std::vector<char> serialize() override;
    transaction(const std::vector<char>&);
};
#endif
