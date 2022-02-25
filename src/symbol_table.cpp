#include "symbol_table.h"
#include "operations/operations.h"

std::ostream& operator<<(std::ostream& os, const symbol_value_t& v) {
    std::visit(overload{[&os](auto&& v) { os << v << " " << typeid(v).name(); }}, static_cast<underlying_symbol_value_t>(v));
    return os;
}
