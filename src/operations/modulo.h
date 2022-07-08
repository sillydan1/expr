#ifndef EXPR_MODULO_H
#define EXPR_MODULO_H
#include "symbol_table.h"
expr::symbol_value_t modulo(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t operator%(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
#endif //EXPR_MODULO_H
