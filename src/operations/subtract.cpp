#include "subtract.h"
#include "util.h"
#include <sstream>

template<typename T1, typename T2>
auto t_subtract(const T1&, const T2&) {
    throw std::domain_error((std::ostringstream{} << "Unable to subtract type " << typeid(T1).name() << " and " << typeid(T2).name()).str());
    return nullptr; // Must return something
}
template<> auto t_subtract(const int& x, const int& y) {
    return x - y;
}
template<> auto t_subtract(const float& x, const int& y) {
    return x - y;
}
template<> auto t_subtract(const int& x, const float& y) {
    return x - y;
}
template<> auto t_subtract(const float& x, const float& y) {
    return x - y;
}

symbol_value_t subtract(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_subtract, b, res);
    return res;
}
symbol_value_t operator-(const symbol_value_t& a, const symbol_value_t& b) {
    return subtract(a,b);
}
