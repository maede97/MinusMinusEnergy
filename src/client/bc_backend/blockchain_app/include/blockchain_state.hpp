#ifndef BLOCKCHAIN_STATE_HPP
#define BLOCKCHAIN_STATE_HPP
#include <vector>
#include <array>
#include <list>
#include <transaction.hpp>
struct block : public de_serializable2{
    std::array<uint64_t, 8> previous_hash;
    std::uint64_t id;
    std::vector<transaction> transactions;
    std::array<uint64_t, 8> nonce;
    std::vector<char> serialize() override;
    block(const std::vector<char>&);
};
class blockchain_state{
    std::list<block> blocks;
};
#endif
