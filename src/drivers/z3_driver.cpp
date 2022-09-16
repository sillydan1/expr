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
#include "drivers/z3_driver.h"

namespace expr {
    z3_driver::z3_driver(const symbol_table_t& known_env, const symbol_table_t& unknown_env)
    // TODO: delay_identifier should be randomly generated
     : driver{{known_env, unknown_env}}, c{}, s{c},
       delay_identifier{"d"}, known{known_env}, unknown{unknown_env} {}

    z3_driver::~z3_driver() = default;

    int z3_driver::parse(const std::string &f) {
        if (f.empty())
            return 0;
        file = f;
        location.initialize(&file);
        scan_begin();
        yy::parser parse(this);
        parse.set_debug_level(trace_parsing);
        try {
            int res = parse();
            scan_end();
            return res;
        } catch (std::domain_error& e) {
            error = e.what();
            return 2;
        } catch (std::exception &e) {
            error = e.what();
            return 1;
        }
    }

    auto z3_driver::get_symbol(const std::string &identifier) -> syntax_tree_t {
        if(!contains(identifier))
            throw std::out_of_range(identifier + " not found");
        return syntax_tree_t{identifier_t{identifier}};
    }

    void z3_driver::add_tree(const syntax_tree_t& tree) {
        s.add(as_z3_expression(tree)); // Note: Only accepts boolean expressions (will throw if not)
        solve();
    }

    void z3_driver::add_tree(const std::string& identifier, const syntax_tree_t& tree) {
        std::cout << "z3_driver ignoring declaration\n";
    }

    void z3_driver::solve() {
        switch (s.check()) {
            case z3::unsat: s.reset(); throw std::domain_error("unsat");
            case z3::unknown: s.reset(); throw std::logic_error("unknown");
            case z3::sat:
                auto m = s.get_model();
                s.reset();
                for(int i = 0; i < m.size(); i++) {
                    auto xx = m[i];
                    auto interp = xx.is_const() ? m.get_const_interp(xx) : m.get_func_interp(xx).else_value();
                    if(xx.name().str() == delay_identifier)
                        result.set_delay_amount(as_symbol_value(interp));
                    else
                        result[xx.name().str()] = as_symbol_value(interp);
                }
        }
    }

    auto z3_driver::as_symbol_value(const z3::expr &e) -> symbol_value_t {
        if(e.is_int())
            return (int) e.as_int64();
        if(e.is_real())
            return (float) e.as_double();
        if(e.is_bool())
            return e.bool_value() == 1; // bool_value() is -1/1
        if(e.is_string_value())
            return e.get_string();
        throw std::logic_error("invalid z3::expr value - unable to convert to expr::symbol_value_t");
    }

    auto z3_driver::as_z3_expression(const symbol_value_t& val) -> z3::expr {
        return std::visit(ya::overload(
            [this](const int& i)          { return c.int_val(i); },
            [this](const float& f)        { return c.real_val(std::to_string(f).c_str()); },
            [this](const bool& b)         { return c.bool_val(b); },
            [this](const std::string& sv) { return c.string_val(sv); },
            [this](const expr::clock_t& v){ return c.int_val(v.time_units); },
            [](auto&& x){ throw std::logic_error("unable to convert symbol value to z3::expr"); }
        ), static_cast<const underlying_symbol_value_t&>(val));
    }

    auto z3_driver::as_z3_expression(const identifier_t& ref) -> z3::expr {
        if(known.contains(ref.ident))
            return std::visit(ya::overload(
                    [this,&ref](const expr::clock_t& v) { return as_z3_expression(known.at(ref.ident)) + c.int_const(delay_identifier.c_str()); },
                    [this,&ref](auto&& x) { return as_z3_expression(known.at(ref.ident)); }
                    ), static_cast<const underlying_symbol_value_t&>(known.at(ref.ident)));
        auto it = find(ref.ident);
        return std::visit(ya::overload(
                [this, &ref](const int& _)          { return c.int_const(ref.ident.c_str()); },
                [this, &ref](const float& _)        { return c.real_const(ref.ident.c_str()); },
                [this, &ref](const bool& _)         { return c.bool_const(ref.ident.c_str()); },
                [this, &ref](const std::string& _)  { return c.string_const(ref.ident.c_str()); },
                [this, &ref](const expr::clock_t& _){ return c.int_const(ref.ident.c_str()); },
                [](auto&& x){ throw std::logic_error("unable to convert symbol reference to z3::expr"); }
        ), static_cast<const underlying_symbol_value_t&>(it->second));
    }

    auto z3_driver::as_z3_expression(const syntax_tree_t &tree) -> z3::expr {
        return std::visit(ya::overload(
                [this](const identifier_t& r) { return as_z3_expression(r); },
                [&](const operator_t& o) {
                    switch (o.operator_type) {
                        case operator_type_t::minus:    return as_z3_expression(tree.children[0]) - as_z3_expression(tree.children[1]); break;
                        case operator_type_t::plus:     return as_z3_expression(tree.children[0]) + as_z3_expression(tree.children[1]); break;
                        case operator_type_t::star:     return as_z3_expression(tree.children[0]) * as_z3_expression(tree.children[1]); break;
                        case operator_type_t::slash:    return as_z3_expression(tree.children[0]) / as_z3_expression(tree.children[1]); break;
                        case operator_type_t::percent:  return as_z3_expression(tree.children[0]) % as_z3_expression(tree.children[1]); break;
                        case operator_type_t::hat:      return z3::pw(as_z3_expression(tree.children[0]), as_z3_expression(tree.children[1])); break;
                        case operator_type_t::_and: return  as_z3_expression(tree.children[0]) && as_z3_expression(tree.children[1]); break;
                        case operator_type_t::_or:  return  as_z3_expression(tree.children[0]) || as_z3_expression(tree.children[1]); break;
                        case operator_type_t::_xor: return  as_z3_expression(tree.children[0]) xor as_z3_expression(tree.children[1]); break;
                        case operator_type_t::_not: return !as_z3_expression(tree.children[0]); break;
                        case operator_type_t::_implies: return implies(as_z3_expression(tree.children[0]),as_z3_expression(tree.children[1])); break;
                        case operator_type_t::gt: return (as_z3_expression(tree.children[0]) > as_z3_expression(tree.children[1])); break;
                        case operator_type_t::ge: return (as_z3_expression(tree.children[0]) >= as_z3_expression(tree.children[1])); break;
                        case operator_type_t::ne: return (as_z3_expression(tree.children[0]) != as_z3_expression(tree.children[1])); break;
                        case operator_type_t::ee: return (as_z3_expression(tree.children[0]) == as_z3_expression(tree.children[1])); break;
                        case operator_type_t::le: return (as_z3_expression(tree.children[0]) <= as_z3_expression(tree.children[1])); break;
                        case operator_type_t::lt: return (as_z3_expression(tree.children[0]) < as_z3_expression(tree.children[1])); break;
                        case operator_type_t::parentheses: return (as_z3_expression(tree.children[0])); break;
                    }
                },
                [this](const symbol_value_t& o){ return as_z3_expression(o); },
                [&](const root_t& r){ return as_z3_expression(tree.children[0]); },
                [](auto&&){ throw std::logic_error("tree node type not recognized"); }
        ), static_cast<const underlying_syntax_node_t&>(tree.node));
    }
}
