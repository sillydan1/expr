#include "symbol_table.h"
#include "operations/operations.h"

std::ostream& operator<<(std::ostream& os, const symbol_value_t& v) {
    std::visit(overload{[&os](auto&& v) { os << v << " " << typeid(v).name(); }}, static_cast<underlying_symbol_value_t>(v));
    return os;
}

std::ostream& operator<<(std::ostream& os, const symbol_map_t& m) {
    for(auto& v : m)
        os << v.first << " :-> " << v.second << "\n";
    return os;
}
