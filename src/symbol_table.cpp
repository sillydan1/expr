#include "symbol_table.h"
#include "operations/operation.h"

symbol_value_t add(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    OPERATOR_IMPL(a, +, b, res);
    return res;
}
symbol_value_t operator+(const symbol_value_t& a, const symbol_value_t& b) {
    return add(a,b);
}
symbol_value_t subtract(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    OPERATOR_IMPL(a, -, b, res);
    return res;
}
symbol_value_t operator-(const symbol_value_t& a, const symbol_value_t& b) {
    return subtract(a,b);
}
symbol_value_t multiply(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    OPERATOR_IMPL(a, *, b, res);
    return res;
}
symbol_value_t operator*(const symbol_value_t& a, const symbol_value_t& b) {
    return multiply(a,b);
}


std::ostream& operator<<(std::ostream& os, const symbol_value_t& v) {
    std::visit(overload{[&os](auto&& v) { os << v << " " << typeid(v).name(); }}, static_cast<underlying_symbol_value_t>(v));
    return os;
}
