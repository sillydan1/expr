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
#ifdef ENABLE_Z3
#define EXPR_Z3_DRIVER_H
#include "operations.h"
#include "drivers/driver.h"
#include <c++/z3++.h>

namespace expr {
    struct z3_driver : public driver {
        z3_driver(const symbol_table_t& known_env, const symbol_table_t& unknown_env);
        ~z3_driver() override;

        auto parse(const std::string &f) -> int override;
        auto get_symbol(const std::string& identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;

        auto as_symbol_value(const z3::expr& e) -> symbol_value_t;
        auto as_z3_expression(const syntax_tree_t& tree) -> z3::expr;
        auto as_z3_expression(const identifier_t& ref) -> z3::expr;
        auto as_z3_expression(const symbol_value_t& val) -> z3::expr;

        auto get_delay_amount() const -> symbol_value_t;

        symbol_table_t result{};
    protected:
        z3::context c{};
        z3::solver s;
        std::string delay_identifier;
        symbol_value_t delay_amount;
        const symbol_table_t& known;
        const symbol_table_t& unknown;
        void solve();
    };
}

#endif //ENABLE_Z3
#endif //EXPR_Z3_DRIVER_H
