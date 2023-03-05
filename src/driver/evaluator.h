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
#include "operations/symbol-operator.h"
#include "symbol_table.h"

namespace expr {
    using symbol_table_ref_collection_t = std::vector<std::reference_wrapper<const expr::symbol_table_t>>;
    class evaluator {
    public:
        evaluator(const symbol_table_ref_collection_t& environments, const symbol_operator& op);
        virtual auto evaluate(const syntax_tree_t& tree) -> symbol_value_t;
        virtual auto contains(const std::string& identifier) const -> bool;
        virtual auto find(const std::string& identifier) const -> expr::symbol_table_t::const_iterator;
    private:
        symbol_table_ref_collection_t environments;
        expr::symbol_table_t::const_iterator end{};
        const symbol_operator& op;
    };
}

#endif

