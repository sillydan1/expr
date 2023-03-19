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
#include "language-builder.h"

namespace expr {
    auto declaration_tree_builder::add_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_access_modifier_t& access_modifier) -> declaration_tree_builder& {
        result.declarations[identifier] = {access_modifier, symbol_type_name_t::_auto, tree};
        return *this;
    }

    auto declaration_tree_builder::add_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_type_name_t& type_name, const symbol_access_modifier_t& access_modifier) -> declaration_tree_builder& {
        result.declarations[identifier] = {access_modifier, type_name, tree};
        return *this;
    }

    auto declaration_tree_builder::add_expression(const syntax_tree_t& tree) -> declaration_tree_builder& {
        result.raw_expression = tree;
        return *this;
    }

    auto declaration_tree_builder::build() -> result_t {
        return result;
    }
}

