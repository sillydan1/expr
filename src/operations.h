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
        virtual auto add(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return a + b; };
        virtual auto sub(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return a - b; };
        virtual auto mul(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return a * b; };
        virtual auto div(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return a / b; };
        virtual auto mod(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return a % b; };
        virtual auto pow(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return a ^ b; };
    };
    struct boolean_operator {
        virtual auto _not(const symbol_value_t &a) -> symbol_value_t { return not_(a); }
        virtual auto _and(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return and_(a,b); }
        virtual auto _or(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return or_(a,b); }
    };
    struct compare_operator {
        virtual auto gt(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return gt_(a,b); }
        virtual auto ge(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return ge_(a,b); }
        virtual auto ee(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return ee_(a,b); }
        virtual auto ne(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return ne_(a,b); }
        virtual auto le(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return le_(a,b); }
        virtual auto lt(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t { return lt_(a,b); }
    };
}

#endif //EXPR_OPERATIONS_H
