#include "operations/multiply.h"
#include "util.h"
#include <sstream>

template<typename T1, typename T2>
auto t_multiply(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to multiply type " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<> auto t_multiply(const int& x, const int& y) {
    return x * y;
}
template<> auto t_multiply(const float& x, const int& y) {
    return x * y;
}
template<> auto t_multiply(const int& x, const float& y) {
    return x * y;
}
template<> auto t_multiply(const float& x, const float& y) {
    return x * y;
}

symbol_value_t multiply(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_multiply, b, res);
    return res;
}
symbol_value_t operator*(const symbol_value_t& a, const symbol_value_t& b) {
    return multiply(a,b);
}
