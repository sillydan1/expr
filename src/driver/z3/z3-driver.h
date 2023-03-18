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
#ifndef EXPR_Z3_DRIVER_H
#define EXPR_Z3_DRIVER_H
#include "../parse-error.h"
#include "symbol_table.h"
#include <z3++.h>

namespace expr {
    class z3_driver {
    public:
        z3_driver(const symbol_table_t& known, const symbol_table_t& unknown);
        auto find_solution(const syntax_tree_t& expression) -> std::optional<symbol_table_t>;
    private:
        auto as_z3(const syntax_tree_t &tree) -> z3::expr;
        auto as_z3(const identifier_t& ref) -> z3::expr;
        auto as_z3(const symbol_value_t& val) -> z3::expr;
        auto as_symbol_value(const z3::expr& e) -> symbol_value_t;
        z3::context c;
        z3::solver s;
        std::string delay_identifier;
        const symbol_table_t& known;
        const symbol_table_t& unknown;
    };
}

#endif

