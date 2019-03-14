#ifndef MY_H_G
#define MY_H_G
#include <chrono>
#include <cmath>
#include <cassert>
#include <string>
#include <cstring>
#include <random>
#include <algorithm>
#include <iostream>
#include <pcg_random.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <complex>
#include <map>
#include <unordered_map>
#include <atomic>
#include <limits>
#include <thread>
namespace std{
    inline unsigned long long nanoTime(){
        return std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }
}
static pcg64_fast h_gen(std::nanoTime());
static std::uniform_int_distribution<unsigned long long> h_dis_ulong(std::numeric_limits<unsigned long long>::min(),std::numeric_limits<unsigned long long>::max());
static std::uniform_int_distribution<unsigned int> h_dis_uint(std::numeric_limits<unsigned int>::min(),std::numeric_limits<unsigned int>::max());
static std::uniform_real_distribution<double> h_dis_double(0.0,1.0);
template<typename T>
inline std::ostream& operator<<(std::ostream& o,const std::vector<T>& vec){
    o << "[";
    for(unsigned int i = 0;i < vec.size();i++){
        o << vec[i];
        if(i < vec.size() - 1)o << ", ";
    }
    o << "]";
    return o;
}
template<>
inline std::ostream& operator<<<char>(std::ostream& o,const std::vector<char>& vec){
    o << "[";
    for(unsigned int i = 0;i < vec.size();i++){
        o << (int)vec[i];
        if(i < vec.size() - 1)o << ", ";
    }
    o << "]";
    return o;
}
namespace std{
    inline double random(){
        return h_dis_double(h_gen);
    }
    template<typename T>
    inline T random(){
        return (T)0;
    }
    template<>
    inline double random<double>(){
        return h_dis_double(h_gen);
    }
    template<>
    inline unsigned long long random<unsigned long long>(){
        return h_dis_ulong(h_gen);
    }
    template<>
    inline unsigned int random<unsigned int>() {
        return h_dis_uint(h_gen);
    }
}
#endif
