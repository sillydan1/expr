#ifndef EXPR_UTIL_H
#define EXPR_UTIL_H
#include <overload>
#define FUNC_IMPL(a, func, b, res) std::visit(ya::overload{[&b,&res](auto&& x){std::visit(ya::overload{[&x,&res](auto&& y){res = func(x,y);}}, static_cast<underlying_symbol_value_t>(b));}}, static_cast<underlying_symbol_value_t>(a))
#define MONOFUNC_IMPL(a, func, res) std::visit(ya::overload{[&res](auto&& x){ res = func(x); }}, static_cast<underlying_symbol_value_t>(a))
#endif //EXPR_UTIL_H
