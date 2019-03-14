#ifndef DE_SERIALIZABLE2_HPP
#define DE_SERIALIZABLE2_HPP
#include <type_traits>
#include <vector>
struct de_serializable2{
    de_serializable2(const std::vector<char>&);
    virtual std::vector<char> serialize();
};
#endif
