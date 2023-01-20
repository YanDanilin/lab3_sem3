#ifndef SUP_H
#define SUP_H
#include <limits>
#include <string>

template <typename T>
struct sup {
    T operator()() {
        return std::numeric_limits<int>::max();
    }
};

template<>
struct sup<std::string> {
    std::string operator() () {
        std::string s = "~~~~~~";
        return s;
    }
};


#endif // SUP_H
