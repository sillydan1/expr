#include "operations/multiply.h"
#include "operations/util.h"
#include <sstream>
using namespace expr;

template<typename T1, typename T2>
auto t_modulo(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to modulo type " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<> auto t_modulo(const int& x, const int& y) {
    return x % y;
}

symbol_value_t modulo(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_modulo, b, res);
    return res;
}
symbol_value_t operator%(const symbol_value_t& a, const symbol_value_t& b) {
    return modulo(a,b);
}
