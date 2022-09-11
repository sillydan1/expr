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
    interpreter::interpreter(std::initializer_list<symbol_table_ref_t> environments) : driver{environments} {}

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
        if (!contains(identifier))
            throw std::out_of_range(identifier + " not found");
        return syntax_tree_t{identifier_t{identifier}};
    }

    void interpreter::add_tree(const syntax_tree_t& tree) {
        expression_result = evaluate(tree);
    }

    void interpreter::add_tree(const std::string& identifier, const syntax_tree_t& tree) {
        result[identifier] = evaluate(tree);
    }

    auto interpreter::evaluate(const syntax_tree_t& tree) -> symbol_value_t {
        symbol_value_t v{};
        std::visit(ya::overload(
                [&](const identifier_t& r){
                    auto s = find(r.ident);
                    if(s == end)
                        throw std::out_of_range("not found: " + r.ident);
                    v = s->second;
                },
                [&](const operator_t& o) {
                    switch (o.operator_type) {
                        case operator_type_t::minus:    v = sub(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::plus:     v = add(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::star:     v = mul(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::slash:    v = div(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::percent:  v = mod(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::hat:      v = pow(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::_and: v = _and(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::_or:  v = _or(evaluate(tree.children[0]),  evaluate(tree.children[1])); break;
                        case operator_type_t::_xor: v = _xor(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::_not: v = _not(evaluate(tree.children[0])); break;
                        case operator_type_t::_implies: v = _implies(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::gt: v = gt(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::ge: v = ge(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::ne: v = ne(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::ee: v = ee(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::le: v = le(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::lt: v = lt(evaluate(tree.children[0]), evaluate(tree.children[1])); break;
                        case operator_type_t::parentheses: v = evaluate(tree.children[0]); break;
                    }
                },
                [&v](const symbol_value_t& o){ v = o; },
                [&](const root_t& r){
                    if(!tree.children.empty())
                        v = evaluate(tree.children[0]);
                },
                [](auto&&){ throw std::logic_error("operator type not recognized"); }
        ), static_cast<const underlying_syntax_node_t&>(tree.node));
        return v;
    }

    auto interpreter::evaluate(const compiler::compiled_expr_collection_t& trees) -> symbol_table_t {
        symbol_table_t res{};
        for(auto& tree : trees)
            res[tree.first] = evaluate(tree.second);
        return res;
    }
}
