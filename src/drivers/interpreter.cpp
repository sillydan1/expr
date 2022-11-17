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

    void interpreter::add_tree(const std::string& access_modifier, const std::string& identifier,
                               const syntax_tree_t& tree) {
        add_tree(identifier, tree);
    }

    auto interpreter::evaluate(const syntax_tree_t& tree) -> symbol_value_t {
        return std::visit(ya::overload(
                [&](const identifier_t& r){
                    auto s = find(r.ident);
                    if(s == end)
                        throw std::out_of_range("not found: " + r.ident);
                    return s->second;
                },
                [&](const operator_t& o) {
                    switch (o.operator_type) {
                        case operator_type_t::minus:    return sub(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::plus:     return add(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::star:     return mul(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::slash:    return div(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::percent:  return mod(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::hat:      return pow(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::_and: return _and(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::_or:  return _or(evaluate(tree.children()[0]),  evaluate(tree.children()[1]));
                        case operator_type_t::_xor: return _xor(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::_not: return _not(evaluate(tree.children()[0]));
                        case operator_type_t::_implies: return _implies(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::gt: return gt(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::ge: return ge(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::ne: return ne(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::ee: return ee(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::le: return le(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::lt: return lt(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::parentheses: return evaluate(tree.children()[0]);
                    }
                    throw std::logic_error("unsupported operator type");
                },
                [](const symbol_value_t& o){ return o; },
                [&](const root_t& r){
                    if(!tree.children().empty())
                        return evaluate(tree.children()[0]);
                    throw std::logic_error("ROOT has no children()");
                },
                [](auto&&){ throw std::logic_error("operator type not recognized"); }
        ), static_cast<const underlying_syntax_node_t&>(tree.node));
    }

    auto interpreter::evaluate(const compiler::compiled_expr_collection_t& trees) -> symbol_table_t {
        symbol_table_t res{};
        for(auto& tree : trees)
            res[tree.first] = evaluate(tree.second);
        return res;
    }
}
