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
#ifndef EXPR_FACTORY_H
#define EXPR_FACTORY_H
#include "symbol/symbol_table.h"

namespace expr {
    /* 
     * Use this to build abstract syntax trees
     * */
    class ast_factory {
    public:
        ast_factory() = default;
        virtual ~ast_factory() = default;
        virtual auto build_operator(const operator_type_t& op, const syntax_tree_t& child) -> syntax_tree_t;
        virtual auto build_operator(const operator_type_t& op, const syntax_tree_t& child1, const syntax_tree_t& child2) -> syntax_tree_t;
        virtual auto build_operator(const operator_type_t& op, const std::vector<syntax_tree_t>& children) -> syntax_tree_t;
        virtual auto build_literal(const symbol_value_t& value) -> syntax_tree_t;
        virtual auto build_identifier(const std::string& identifier) -> syntax_tree_t;
        virtual auto build_root(const syntax_tree_t& child) -> syntax_tree_t;

        virtual auto build_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_access_modifier_t& access_modifier) -> syntax_tree_t;
        virtual auto build_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_type_name_t& type_name = symbol_type_name_t::_auto, const symbol_access_modifier_t& access_modifier = symbol_access_modifier_t::_private) -> syntax_tree_t;
    };
}

#endif

