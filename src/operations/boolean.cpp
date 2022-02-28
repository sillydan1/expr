#include "operations/boolean.h"
#include "util.h"
#include <sstream>

template<typename T1, typename T2>
auto t_and(const T1&, const T2&) {
    throw std::domain_error((std::ostringstream{} << "Unable to AND types " << typeid(T1).name() << " and " << typeid(T2).name()).str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_or(const T1&, const T2&) {
    throw std::domain_error((std::ostringstream{} << "Unable to OR types " << typeid(T1).name() << " and " << typeid(T2).name()).str());
    return nullptr; // Must return something
}
template<typename T1>
auto t_not(const T1&) {
    throw std::domain_error((std::ostringstream{} << "Unable to NOT type " << typeid(T1).name()).str());
    return nullptr; // Must return something
}

template<>
auto t_and(const bool& a, const bool& b) {
    return a && b;
}
template<>
auto t_or(const bool& a, const bool& b) {
    return a || b;
}
template<>
auto t_not(const bool& a) {
    return !a;
}

symbol_value_t and_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_and, b, res);
    return res;
}
symbol_value_t operator&&(const symbol_value_t& a, const symbol_value_t& b) {
    return and_(a,b);
}
symbol_value_t or_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_or, b, res);
    return res;
}
symbol_value_t operator||(const symbol_value_t& a, const symbol_value_t& b) {
    return and_(a,b);
}
symbol_value_t not_(const symbol_value_t& a) {
    symbol_value_t res{};
    MONOFUNC_IMPL(a, t_not, res);
    return res;
}
symbol_value_t operator!(const symbol_value_t& a) {
    return not_(a);
}
