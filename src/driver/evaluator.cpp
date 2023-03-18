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
#include "evaluator.h"
#include "driver/parse-error.h"
#include <overload>
#include <algorithm>

namespace expr {
    evaluator::evaluator(const symbol_table_ref_collection_t& environments, const symbol_operator& op) : environments{environments}, op{op} {
        
    }

    auto evaluator::contains(const std::string& identifier) const -> bool {
        return find(identifier) != end;
    }

    auto evaluator::find(const std::string& identifier) const -> expr::symbol_table_t::const_iterator {
        for(auto& env : environments) {
            auto env_it = env.get().find(identifier);
            if(env_it != env.get().end())
                return env_it;
        }
        return end;
    }

    auto evaluator::evaluate(const syntax_tree_t& tree) -> symbol_value_t {
        return std::visit(ya::overload(
                [&](const identifier_t& r){
                    auto s = find(r.ident);
                    if(s == end)
                        throw parse_error("not found: '" + r.ident + "'");
                    return s->second;
                },
                [&](const operator_t& o) {
                    switch (o.operator_type) {
                        // FIX: This this does not consider if children().size() > 2 
                        case operator_type_t::minus:    return op.sub(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::plus:     return op.add(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::star:     return op.mul(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::slash:    return op.div(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::percent:  return op.mod(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::hat:      return op.pow(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::_and:     return op._and(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::_or:      return op._or(evaluate(tree.children()[0]),  evaluate(tree.children()[1]));
                        case operator_type_t::_xor:     return op._xor(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::_not:     return op._not(evaluate(tree.children()[0]));
                        case operator_type_t::_implies: return op._implies(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::gt:       return op.gt(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::ge:       return op.ge(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::ne:       return op.ne(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::ee:       return op.ee(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::le:       return op.le(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::lt:       return op.lt(evaluate(tree.children()[0]), evaluate(tree.children()[1]));
                        case operator_type_t::parentheses: return evaluate(tree.children()[0]);
                    }
                    throw std::logic_error("unsupported operator type");
                },
                [](const symbol_value_t& o){ return o; },
                [&](const root_t& r){
                    if(!tree.children().empty())
                        return evaluate(tree.children()[0]);
                    throw parse_error("ROOT has no children()");
                },
                [](auto&&){ throw parse_error("operator type not recognized"); }
        ), static_cast<const underlying_syntax_node_t&>(tree.node));
    }
}

