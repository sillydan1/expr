#include "symbol_table.h"
#include "operation.h"

template<typename T1, typename T2>
auto _divide(const T1&, const T2&);
template<>
auto _divide(const int& x, const int& y) {
    if(y == 0 || (x == INT32_MIN && y == -1)) throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<>
auto _divide(const float& x, const int& y) {
    if(y == 0) throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<>
auto _divide(const int& x, const float& y) {
    if(y == 0.0f) throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<>
auto _divide(const float& x, const float& y) {
    if(y == 0.0f) throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
symbol_value_t divide(const symbol_value_t& a, const symbol_value_t& b) {
    // TODO: Check for division by zero and INT_MIN / -1
    symbol_value_t res{};
    FUNC_IMPL(a, _divide, b, res);
    return res;
}
symbol_value_t operator/(const symbol_value_t& a, const symbol_value_t& b) {
    return divide(a,b);
}
