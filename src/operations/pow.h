#ifndef EXPR_POW_H
#define EXPR_POW_H
#include "symbol_table.h"
expr::symbol_value_t pow(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t operator^(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
#endif //EXPR_POW_H
