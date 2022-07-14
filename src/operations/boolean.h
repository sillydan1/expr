#ifndef EXPR_BOOLEAN_H
#define EXPR_BOOLEAN_H
#include "symbol_table.h"
expr::symbol_value_t and_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t or_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t xor_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t not_(const expr::symbol_value_t& a);
expr::symbol_value_t gt_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t ge_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t ee_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t ne_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t le_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t lt_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
// TODO: These operators are ambiguous with symbol_value_t && std::false_type / std::true_type for some reason.
//symbol_value_t operator&&(const symbol_value_t& a, const symbol_value_t& b);
//symbol_value_t operator||(const symbol_value_t& a, const symbol_value_t& b);
//symbol_value_t operator!(const symbol_value_t& a);
#endif //EXPR_BOOLEAN_H
