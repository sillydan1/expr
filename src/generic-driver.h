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
#ifndef GENERIC_DRIVER_H
#define GENERIC_DRIVER_H
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "symbol_table.h"

namespace expr {
    struct generic_driver {
        std::unordered_map<std::string, syntax_tree_t> declarations;
        std::optional<syntax_tree_t> expression;
        auto get_symbol_table(const symbol_table_t& env) -> symbol_table_t;
        auto get_expression_value(const symbol_table_t& env) -> symbol_value_t;
        void parse_expressions(const std::string& s);
        auto parse_symbol_table(const std::string& s) -> symbol_table_t;
        void print(const symbol_table_t& known, const symbol_table_t& unknown);
    };
}

#endif

