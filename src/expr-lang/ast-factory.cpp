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
#include "ast-factory.h"
#include "symbol_table.h"
#include <cstdlib>

namespace expr {
    ast_factory::ast_factory() {}
    ast_factory::~ast_factory() {}
    
    auto ast_factory::build_operator(const operator_type_t& op, const syntax_tree_t& child) -> syntax_tree_t {
        return build_operator(op, std::vector<syntax_tree_t>{child});
    }

    auto ast_factory::build_operator(const operator_type_t& op, const syntax_tree_t& child1, const syntax_tree_t& child2) -> syntax_tree_t {
        return build_operator(op, std::vector<syntax_tree_t>{child1, child2});
    }

    auto ast_factory::build_operator(const operator_type_t& op, const std::vector<syntax_tree_t>& children) -> syntax_tree_t {
        syntax_tree_t result{operator_t{op}};
        for(auto& child : children)
            result.concat(child);
        return result;
    }

    auto ast_factory::build_literal(const symbol_value_t& value) -> syntax_tree_t {
        return syntax_tree_t{value};
    }

    auto ast_factory::build_identifier(const std::string& identifier) -> syntax_tree_t {
        return syntax_tree_t{identifier_t{identifier}};
    }

    auto ast_factory::build_root(const syntax_tree_t& child) -> syntax_tree_t {
        return syntax_tree_t{}.concat(child);
    }
}

