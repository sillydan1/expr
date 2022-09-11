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

    auto interpreter::interpret_declarations(const std::string &f) -> symbol_table_t {
        result = {};
        auto res = parse(f);
        if(res != 0)
            throw std::logic_error(error);
        return result;
    }

    auto interpreter::interpret_expression(const std::string &f) -> symbol_value_t {
        expression_result = {};
        auto res = parse(f);
        if(res != 0)
            throw std::logic_error(error);
        return expression_result;
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
        return evaluate(tree, *this, environment);
    }

    auto interpreter::evaluate(const compiler::compiled_expr_collection_t& trees) -> symbol_table_t {
        return evaluate(trees, *this, environment);
    }

    auto interpreter::evaluate(const syntax_tree_t& tree, const interpreter& op, const symbol_table_t& symbols) -> symbol_value_t {
        symbol_value_t v{};
        std::visit(ya::overload(
                [&](const symbol_reference_t& r){ v = symbols.at(r->first); },   // TODO: Should we look up every time? If so, what is the point of storing an iterator in the ast?
                [&](const c_symbol_reference_t& r){ v = symbols.at(r->first); }, // TODO: Should we look up every time? If so, what is the point of storing an iterator in the ast?
                [&](const operator_t& o) {
                    switch (o.operator_type) {
                        case operator_type_t::minus:    v = op.sub(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::plus:     v = op.add(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::star:     v = op.mul(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::slash:    v = op.div(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::percent:  v = op.mod(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::hat:      v = op.pow(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::_and: v = op._and(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::_or:  v = op._or(evaluate(tree.children[0], op, symbols),  evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::_xor: v = op._xor(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::_not: v = op._not(evaluate(tree.children[0], op, symbols)); break;
                        case operator_type_t::_implies: v = op._implies(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::gt: v = op.gt(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::ge: v = op.ge(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::ne: v = op.ne(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::ee: v = op.ee(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::le: v = op.le(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::lt: v = op.lt(evaluate(tree.children[0], op, symbols), evaluate(tree.children[1], op, symbols)); break;
                        case operator_type_t::parentheses: v = evaluate(tree.children[0], op, symbols); break;
                    }
                },
                [&v](const symbol_value_t& o){ v = o; },
                [&](const root_t& r){
                    if(!tree.children.empty())
                        v = evaluate(tree.children[0], op, symbols);
                },
                [](auto&&){ throw std::logic_error("operator type not recognized"); }
        ), static_cast<const underlying_syntax_node_t&>(tree.node));
        return v;
    }

    auto interpreter::evaluate(const compiler::compiled_expr_collection_t& symbol_tree_map, const interpreter& op, const symbol_table_t& symbols) -> symbol_table_t {
        symbol_table_t result{};
        for(auto& tree : symbol_tree_map)
             result[tree.first] = evaluate(tree.second, op, symbols);
        return result;
    }
}
