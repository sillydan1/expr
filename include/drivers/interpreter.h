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
#ifndef EXPR_INTERPRETER_H
#define EXPR_INTERPRETER_H
#include "operations.h"
#include "drivers/driver.h"
#include "compiler.h"
#include "generic_symbol_operator.h"

namespace expr {
    struct interpreter : public driver, generic_symbol_operator {
        interpreter(std::initializer_list<symbol_table_ref_t> environments);
        ~interpreter() override = default;

        auto parse(const std::string &f) -> int override;
        auto interpret_declarations(const std::string& f) -> symbol_table_t;
        auto interpret_expression(const std::string& f) -> symbol_value_t;
        auto get_symbol(const std::string& identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;
        void add_tree(const std::string& access_modifier, const std::string& identifier, const syntax_tree_t& tree) override;

        symbol_table_t result{};
        symbol_value_t expression_result{};

        auto evaluate(const syntax_tree_t& tree) -> symbol_value_t;
        auto evaluate(const compiler::compiled_expr_collection_t& tree) -> symbol_table_t;
    };
}

#endif
