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
#ifndef EXPR_TREE_COMPILER_H
#define EXPR_TREE_COMPILER_H
#include "tree_driver.h"

namespace expr {
    struct tree_compiler : tree_driver {
        using compiled_expr_t = syntax_tree_t;
#ifndef NDEBUG
        using compiled_expr_collection_t = std::map<std::string, compiled_expr_t>;
#else
        using compiled_expr_collection_t = std::unordered_map<std::string, compiled_expr_t>;
#endif
        explicit tree_compiler(const symbol_table_tree_t::iterator& scope) : tree_driver{scope}, trees{} {}
        int parse(const std::string &f) override;
        auto get_symbol(const std::string &identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;
        void add_tree(const std::string& access_modifier, const std::string& identifier, const syntax_tree_t& tree) override;

        compiled_expr_collection_t trees;
    };
}

#endif //EXPR_TREE_COMPILER_H
