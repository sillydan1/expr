#ifndef EXPR_SYMBOL_TABLE_H
#define EXPR_SYMBOL_TABLE_H
#include <variant>
#include <map>
#include <functional>
using symbol_value_t = std::variant<int, float>;
using symbol_map_t = std::map<std::string, symbol_value_t>;
template<typename T>
using modifier_t = std::function<T(const T&, const T&)>;

std::string to_string(const symbol_value_t& s);
symbol_value_t add(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t subtract(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t multiply(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t divide(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator+(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator-(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator*(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator/(const symbol_value_t& a, const symbol_value_t& b);

template<typename T, typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::variant<T, Ts...>& v) {
    return std::visit([&os](auto&& arg) { return os << arg; }, v);
}

#endif
