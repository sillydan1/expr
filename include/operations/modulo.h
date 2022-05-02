#ifndef EXPR_MODULO_H
#define EXPR_MODULO_H
#include "symbol_table.h"
symbol_value_t modulo(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator%(const symbol_value_t& a, const symbol_value_t& b);
#endif //EXPR_MODULO_H
