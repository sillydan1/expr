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
#include "operations/operations.h"
#include "symbol_table.h"

namespace expr {
    struct symbol_operator {
        virtual auto add(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return ::operator+(a,b); };
        virtual auto sub(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return a - b; };
        virtual auto mul(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return a * b; };
        virtual auto div(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return a / b; };
        virtual auto mod(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return a % b; };
        virtual auto pow(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return a ^ b; };
        virtual auto _not(const symbol_value_t &a) const -> symbol_value_t { return not_(a); }
        virtual auto _and(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return and_(a,b); }
        virtual auto _or(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return or_(a,b); }
        virtual auto _xor(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return xor_(a,b); }
        virtual auto _implies(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return implies_(a,b); }
        virtual auto gt(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return gt_(a,b); }
        virtual auto ge(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return ge_(a,b); }
        virtual auto ee(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return ee_(a,b); }
        virtual auto ne(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return ne_(a,b); }
        virtual auto le(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return le_(a,b); }
        virtual auto lt(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t { return lt_(a,b); }
    };
}

#endif

