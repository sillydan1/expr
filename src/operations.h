#ifndef EXPR_OPERATIONS_H
#define EXPR_OPERATIONS_H
#include "operations/add.h"
#include "operations/boolean.h"
#include "operations/divide.h"
#include "operations/modulo.h"
#include "operations/multiply.h"
#include "operations/pow.h"
#include "operations/subtract.h"

namespace expr {
    struct arithmetic_operator {
        virtual auto add(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto sub(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto mul(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto div(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto mod(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto pow(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
    };
    struct boolean_operator {
        virtual auto _not(const symbol_value_t &a) -> symbol_value_t = 0;
        virtual auto _and(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto _or(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto _xor(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
    };
    struct compare_operator {
        virtual auto gt(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto ge(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto ee(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto ne(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto le(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
        virtual auto lt(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t = 0;
    };
}

#endif //EXPR_OPERATIONS_H
