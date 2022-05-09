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

auto operator<<(std::ostream &o, const expr::operator_t &p) -> std::ostream & { return o << p.operator_str; }
auto operator<<(std::ostream &o, const expr::root_t &r) -> std::ostream & { return o << "ROOT"; }
auto operator<<(std::ostream &o, const expr::syntax_node_t &n) -> std::ostream & {
    std::visit(overload(
            [&o](auto &&x) { o << x; }
    ), n);
    return o;
}
