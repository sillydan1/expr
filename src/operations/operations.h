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
        virtual auto add(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto sub(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto mul(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto div(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto mod(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto pow(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
    };
    struct boolean_operator {
        virtual auto _not(const symbol_value_t &a) const -> symbol_value_t = 0;
        virtual auto _and(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto _or(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto _xor(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto _implies(const symbol_value_t& a, const symbol_value_t& b) const -> symbol_value_t = 0;
    };
    struct compare_operator {
        virtual auto gt(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto ge(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto ee(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto ne(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto le(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
        virtual auto lt(const symbol_value_t &a, const symbol_value_t &b) const -> symbol_value_t = 0;
    };
}

#endif //EXPR_OPERATIONS_H

