#include "operations/pow.h"
#include "operations/util.h"
#include <sstream>
#include <cmath>
using namespace expr;

template<typename T1, typename T2>
auto t_pow(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to power type " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<> auto t_pow(const int& x, const int& y) {
    return static_cast<int>(pow(x,y));
}
template<> auto t_pow(const float& x, const int& y) {
    return static_cast<float>(pow(x,y));
}
template<> auto t_pow(const int& x, const float& y) {
    return static_cast<float>(pow(x,y));
}
template<> auto t_pow(const float& x, const float& y) {
    return static_cast<float>(pow(x,y));
}

symbol_value_t pow(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_pow, b, res);
    return res;
}
symbol_value_t operator^(const symbol_value_t& a, const symbol_value_t& b) {
    return pow(a,b);
}
