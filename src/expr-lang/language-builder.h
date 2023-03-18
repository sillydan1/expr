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
#ifndef EXPR_LANG_SYMBOL_TABLE_BUILDER_H
#define EXPR_LANG_SYMBOL_TABLE_BUILDER_H
#include "symbol/symbol_table.h"
#include <string>

namespace expr {
    class language_builder {
    public:
        language_builder() = default;
        virtual ~language_builder() = default;
        virtual auto add_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_access_modifier_t& access_modifier) -> language_builder& = 0;
        virtual auto add_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_type_name_t& type_name = symbol_type_name_t::_auto, const symbol_access_modifier_t& access_modifier = symbol_access_modifier_t::_private) -> language_builder& = 0;
        virtual auto add_expression(const syntax_tree_t& tree) -> language_builder& = 0;
    };

    class declaration_tree_builder : public language_builder {
    public:
        declaration_tree_builder() = default;
        virtual ~declaration_tree_builder() = default;
        struct result_t {
            struct decl_t {
                symbol_access_modifier_t access_modifier{};
                symbol_type_name_t type_name{};
                syntax_tree_t tree;
            };
            std::map<std::string, decl_t> declarations{};
            std::optional<syntax_tree_t> raw_expression{};
        };
        auto add_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_access_modifier_t& access_modifier) -> declaration_tree_builder& override;
        auto add_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_type_name_t& type_name = symbol_type_name_t::_auto, const symbol_access_modifier_t& access_modifier = symbol_access_modifier_t::_private) -> declaration_tree_builder& override;
        auto add_expression(const syntax_tree_t& tree) -> declaration_tree_builder& override;
        virtual auto build() -> result_t;
    private:
        result_t result{};
    };
}

#endif

