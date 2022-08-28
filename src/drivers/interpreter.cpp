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
#include "drivers/interpreter.h"
#include <utility>
#include "parser.hpp"

namespace expr {
    interpreter::interpreter(const symbol_table_t& map) : environment{map}, driver{} {
    }

    int interpreter::parse(const std::string &f) {
        if (f.empty()) {
#ifdef DEFAULT_EXPRESSION_VALUE
            expression_result = DEFAULT_EXPRESSION_VALUE;
#endif
            return 0;
        }
        file = f;
        location.initialize(&file);
        scan_begin();
        yy::parser parse(this);
        parse.set_debug_level(trace_parsing);
        try {
            int res = parse();
            scan_end();
            return res;
        } catch (std::exception &e) {
            error = e.what();
            return 1;
        }
    }

    auto interpreter::get_symbol(const std::string &identifier) -> syntax_tree_t {
#ifndef NDEBUG
        if (!environment.contains(identifier))
            throw std::out_of_range(identifier + " not found");
#endif
        return syntax_tree_t{environment.find(identifier)};
    }

    void interpreter::add_tree(const syntax_tree_t& tree) {
        expression_result = evaluate(tree);
    }

    void interpreter::add_tree(const std::string& identifier, const syntax_tree_t& tree) {
        result[identifier] = evaluate(tree);
    }

    auto interpreter::evaluate(const syntax_tree_t& tree) -> symbol_value_t {
        return evaluate(tree, *this, *this, *this, environment);
    }

    auto interpreter::evaluate(const compiler::compiled_expr_collection_t& trees) -> symbol_table_t {
        return evaluate(trees, *this, *this, *this, environment);
    }

    auto interpreter::evaluate(const syntax_tree_t& tree, arithmetic_operator& arith, boolean_operator& boolean, compare_operator& comparator, const symbol_table_t& symbols) -> symbol_value_t {
        symbol_value_t v{};
        auto eval_wrapper = [&](const syntax_tree_t& t) { return evaluate(t, arith, boolean, comparator, symbols); };
        std::visit(ya::overload(
                [&](const symbol_reference_t& r){ v = symbols.at(r->first); },   // TODO: Should we look up every time? If so, what is the point of storing an iterator in the ast?
                [&](const c_symbol_reference_t& r){ v = symbols.at(r->first); }, // TODO: Should we look up every time? If so, what is the point of storing an iterator in the ast?
                [&](const operator_t& o) {
                    switch (o.operator_type) {
                        case operator_type_t::minus:    v = arith.sub(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::plus:     v = arith.add(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::star:     v = arith.mul(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::slash:    v = arith.div(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::percent:  v = arith.mod(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::hat:      v = arith.pow(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::_and: v = boolean._and(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::_or:  v = boolean._or(eval_wrapper(tree.children[0]),  eval_wrapper(tree.children[1])); break;
                        case operator_type_t::_xor: v = boolean._xor(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::_not: v = boolean._not(eval_wrapper(tree.children[0])); break;
                        case operator_type_t::_implies: v = boolean._implies(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::gt: v = comparator.gt(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::ge: v = comparator.ge(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::ne: v = comparator.ne(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::ee: v = comparator.ee(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::le: v = comparator.le(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::lt: v = comparator.lt(eval_wrapper(tree.children[0]), eval_wrapper(tree.children[1])); break;
                        case operator_type_t::parentheses: v = eval_wrapper(tree.children[0]); break;
                    }
                },
                [&v](const symbol_value_t& o){ v = o; },
                [&](const root_t& r){
                    if(!tree.children.empty())
                        v = eval_wrapper(tree.children[0]);
                },
                [](auto&&){ throw std::logic_error("operator type not recognized"); }
        ), static_cast<const underlying_syntax_node_t&>(tree.node));
        return v;
    }

    auto interpreter::evaluate(const compiler::compiled_expr_collection_t& symbol_tree_map, expr::arithmetic_operator &arith, expr::boolean_operator &boolean, expr::compare_operator &comparator, const symbol_table_t& symbols) -> symbol_table_t {
        symbol_table_t result{};
        for(auto& tree : symbol_tree_map)
             result[tree.first] = evaluate(tree.second, arith, boolean, comparator, symbols);
        return result;
    }
}
