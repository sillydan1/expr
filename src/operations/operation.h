#ifndef EXPR_OPERATION_H
#define EXPR_OPERATION_H
#define OPERATOR_IMPL(a, op, b, res) std::visit(overload{[&b,&res](auto&& x){std::visit(overload{[&x,&res](auto&& y){res = x op y;}}, static_cast<underlying_symbol_value_t>(b));}}, static_cast<underlying_symbol_value_t>(a))
#define FUNC_IMPL(a, func, b, res) std::visit(overload{[&b,&res](auto&& x){std::visit(overload{[&x,&res](auto&& y){res = func(x,y);}}, static_cast<underlying_symbol_value_t>(b));}}, static_cast<underlying_symbol_value_t>(a))
#endif //EXPR_OPERATION_H
