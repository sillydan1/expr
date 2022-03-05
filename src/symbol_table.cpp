#include "symbol_table.h"
#include <overload.h>

symbol_table_t& symbol_table_t::operator+=(const symbol_table_t& other) {
    for(auto& e : other)
        this->insert_or_assign(e.first, e.second);
    return *this;
}

symbol_table_t operator+(const symbol_table_t& a, const symbol_table_t& b) {
    symbol_table_t r{};
    r += a;
    r += b;
    return r;
}

std::ostream& operator<<(std::ostream& os, const symbol_value_t& v) {
    std::visit(overload{
        [&os](const bool& b) { os << std::boolalpha << b << " " << typeid(b).name(); },
        [&os](const std::string& s) { os << "\"" << s << "\" s"; },
        [&os](auto&& v) { os << v << " " << typeid(v).name(); }}, static_cast<const underlying_symbol_value_t&>(v));
    return os;
}

std::ostream& operator<<(std::ostream& os, const symbol_table_t& m) {
    for(auto& v : m)
        os << v.first << " :-> " << v.second << "\n";
    return os;
}
