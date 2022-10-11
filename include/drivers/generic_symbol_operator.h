/* MIT License
 *
 * Copyright (c) 2022 Asger Gitz-Johansen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef EXPR_GENERIC_SYMBOL_OPERATOR_H
#define EXPR_GENERIC_SYMBOL_OPERATOR_H
#include "operations.h"

namespace expr {
    struct generic_symbol_operator : public arithmetic_operator, boolean_operator, compare_operator {
        auto add(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return ::operator+(a,b); };
        auto sub(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return a - b; };
        auto mul(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return a * b; };
        auto div(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return a / b; };
        auto mod(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return a % b; };
        auto pow(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return a ^ b; };

        auto _not(const symbol_value_t &a) const -> symbol_value_t override { return not_(a); }
        auto _and(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return and_(a,b); }
        auto _or(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return or_(a,b); }
        auto _xor(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return xor_(a,b); }
        auto _implies(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return implies_(a,b); }

        auto gt(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return gt_(a,b); }
        auto ge(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return ge_(a,b); }
        auto ee(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return ee_(a,b); }
        auto ne(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return ne_(a,b); }
        auto le(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return le_(a,b); }
        auto lt(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t override { return lt_(a,b); }
    };
}

#endif //EXPR_GENERIC_SYMBOL_OPERATOR_H
