#ifndef EXPR_POW_H
#define EXPR_POW_H
#include "symbol_table.h"
symbol_value_t pow(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator^(const symbol_value_t& a, const symbol_value_t& b);
#endif //EXPR_POW_H
