#include "operations/divide.h"
#include "operations/util.h"
#include <sstream>

template<typename T1, typename T2>
auto t_divide(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to divide type " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<> auto t_divide(const int& x, const int& y) {
    if(y == 0 || (x == INT32_MIN && y == -1))
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<> auto t_divide(const float& x, const int& y) {
    if(y == 0)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<> auto t_divide(const int& x, const float& y) {
    if(y == 0.0f)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<> auto t_divide(const float& x, const float& y) {
    if(y == 0.0f)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}

symbol_value_t divide(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_divide, b, res);
    return res;
}
symbol_value_t operator/(const symbol_value_t& a, const symbol_value_t& b) {
    return divide(a,b);
}
