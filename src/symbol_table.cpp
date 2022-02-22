#include <functional>
#include "symbol_table.h"

#define OPERATOR_IMPL(op) symbol_value_t res{};\
std::visit(overload{[&b,&res](auto&& v){std::visit(overload{[&v,&res](auto&& x){res = v op x;}}, b);}}, a);\
return res

symbol_value_t add(const symbol_value_t& a, const symbol_value_t& b) {OPERATOR_IMPL(+);}
symbol_value_t operator+(const symbol_value_t& a, const symbol_value_t& b) {return add(a,b);}
symbol_value_t subtract(const symbol_value_t& a, const symbol_value_t& b) {OPERATOR_IMPL(-);}
symbol_value_t operator-(const symbol_value_t& a, const symbol_value_t& b) {return subtract(a,b);}
symbol_value_t multiply(const symbol_value_t& a, const symbol_value_t& b) {OPERATOR_IMPL(*);}
symbol_value_t operator*(const symbol_value_t& a, const symbol_value_t& b) {return multiply(a,b);}
symbol_value_t divide(const symbol_value_t& a, const symbol_value_t& b) {OPERATOR_IMPL(/);}
symbol_value_t operator/(const symbol_value_t& a, const symbol_value_t& b) {return divide(a,b);}

std::ostream& operator<<(std::ostream& os, const symbol_value_t& v) {
    std::visit(overload{[&os](auto&& v) { os << v << " " << typeid(v).name(); }}, v);
    return os;
}
