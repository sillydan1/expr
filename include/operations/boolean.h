#ifndef EXPR_BOOLEAN_H
#define EXPR_BOOLEAN_H
#include "symbol_table.h"
symbol_value_t and_(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t or_(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t not_(const symbol_value_t& a);
symbol_value_t gt_(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t ge_(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t ee_(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t ne_(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t le_(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t lt_(const symbol_value_t& a, const symbol_value_t& b);
// TODO: These operators are ambiguous with symbol_value_t && std::false_type / std::true_type for some reason.
//symbol_value_t operator&&(const symbol_value_t& a, const symbol_value_t& b);
//symbol_value_t operator||(const symbol_value_t& a, const symbol_value_t& b);
//symbol_value_t operator!(const symbol_value_t& a);
#endif //EXPR_BOOLEAN_H
