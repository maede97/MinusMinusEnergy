#ifndef BLOCKCHAIN_STATE_HPP
#define BLOCKCHAIN_STATE_HPP
#include <vector>
#include <array>
#include <list>
#include <transaction.hpp>
struct block{
    std::array<uint64_t, 8> previous_hash;
    std::uint64_t id;
    std::vector<transaction> transactions;
    std::array<uint64_t, 8> nonce;
};
class blockchain_state{
    std::list<block> blocks;
};
#endif
