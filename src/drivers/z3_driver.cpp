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
#include <c++/z3++.h>

namespace expr {
    struct z3_driver::impl {
        z3::context c{};
        z3::solver s;
        const symbol_table_t& known;
        const symbol_table_t& unknown;
        auto as_symbol_value(const z3::expr& e) -> symbol_value_t;
        auto as_z3_expression(const syntax_tree_t& tree) -> z3::expr;
        auto as_z3_expression(const symbol_reference_t& ref) -> z3::expr;
        auto as_z3_expression(const c_symbol_reference_t& ref) -> z3::expr;
        auto as_z3_expression(const symbol_value_t& val) -> z3::expr;
        void solve();
        impl(const symbol_table_t& known, const symbol_table_t& unknown) : c{}, s{c}, known{known}, unknown{unknown} {}
    };

    z3_driver::z3_driver(const symbol_table_t& known_env,
                         const symbol_table_t& unknown_env)
                         : pimpl{std::make_unique<z3_driver::impl>(known_env,unknown_env)}, driver{}
                         {}

    z3_driver::~z3_driver() {}

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
        if (!pimpl->known.contains(identifier)) {
            if(!pimpl->unknown.contains(identifier))
                throw std::out_of_range(identifier + " not found");
            return syntax_tree_t{pimpl->unknown.find(identifier)};
        }
        return syntax_tree_t{pimpl->known.at(identifier)};
    }

    void z3_driver::add_tree(const syntax_tree_t& tree) {
        pimpl->s.add(pimpl->as_z3_expression(tree)); // Note: Only accepts boolean expressions (will throw if not)
        solve();
    }

    void z3_driver::add_tree(const std::string& identifier, const syntax_tree_t& tree) {
        std::cout << "z3_driver ignoring declaration\n";
    }

    void z3_driver::solve() {
        switch (pimpl->s.check()) {
            case z3::unsat: pimpl->s.reset(); throw std::domain_error("unsat");
            case z3::unknown: pimpl->s.reset(); throw std::logic_error("unknown");
            case z3::sat:
                auto m = pimpl->s.get_model();
                pimpl->s.reset();
                for(int i = 0; i < m.size(); i++) {
                    auto xx = m[i];
                    auto interp = xx.is_const() ? m.get_const_interp(xx) : m.get_func_interp(xx).else_value();
                    result[xx.name().str()] = pimpl->as_symbol_value(interp);
                }
                break;
        }
    }

    auto z3_driver::impl::as_symbol_value(const z3::expr &e) -> symbol_value_t {
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

    auto z3_driver::impl::as_z3_expression(const symbol_value_t& val) -> z3::expr {
        z3::expr v = c.int_val(0); // placeholder value
        std::visit(ya::overload(
            [&v, this](const int& i)          { v = c.int_val(i); },
            [&v, this](const float& f)        { v = c.real_val(std::to_string(f).c_str()); },
            [&v, this](const bool& b)         { v = c.bool_val(b); },
            [&v, this](const std::string& sv) { v = c.string_val(sv); },
            [](auto&& x){ throw std::logic_error("unable to convert symbol value to z3::expr"); }
        ), static_cast<const underlying_symbol_value_t&>(val));
        return v;
    }

    auto z3_driver::impl::as_z3_expression(const symbol_reference_t &ref) -> z3::expr {
        z3::expr v = c.int_val(0); // placeholder value
        std::visit(ya::overload(
                [&v, this, &ref](const int& _)          { v = c.int_const(ref->first.c_str()); },
                [&v, this, &ref](const float& _)        { v = c.real_const(ref->first.c_str()); },
                [&v, this, &ref](const bool& _)         { v = c.bool_const(ref->first.c_str()); },
                [&v, this, &ref](const std::string& _)  { v = c.string_const(ref->first.c_str()); },
                [](auto&& x){ throw std::logic_error("unable to convert symbol reference to z3::expr"); }
        ), static_cast<const underlying_symbol_value_t&>(ref->second));
        return v;
    }

    auto z3_driver::impl::as_z3_expression(const c_symbol_reference_t &ref) -> z3::expr {
        z3::expr v = c.int_val(0); // placeholder value
        std::visit(ya::overload(
            [&v, this, &ref](const int& _)          { v = c.int_const(ref->first.c_str()); },
            [&v, this, &ref](const float& _)        { v = c.real_const(ref->first.c_str()); },
            [&v, this, &ref](const bool& _)         { v = c.bool_const(ref->first.c_str()); },
            [&v, this, &ref](const std::string& _)  { v = c.string_const(ref->first.c_str()); },
            [](auto&& x){ throw std::logic_error("unable to convert const symbol reference to z3::expr"); }
        ), static_cast<const underlying_symbol_value_t&>(ref->second));
        return v;
    }

    auto z3_driver::impl::as_z3_expression(const syntax_tree_t &tree) -> z3::expr {
        z3::expr v = c.int_val(0); // placeholder value
        std::visit(ya::overload(
                [&v,this](const symbol_reference_t& r) { v = as_z3_expression(r); },
                [&v,this](const c_symbol_reference_t& r) { v = as_z3_expression(r); },
                [&](const operator_t& o) {
                    switch (o.operator_type) {
                        case operator_type_t::minus:    v = as_z3_expression(tree.children[0]) - as_z3_expression(tree.children[1]); break;
                        case operator_type_t::plus:     v = as_z3_expression(tree.children[0]) + as_z3_expression(tree.children[1]); break;
                        case operator_type_t::star:     v = as_z3_expression(tree.children[0]) * as_z3_expression(tree.children[1]); break;
                        case operator_type_t::slash:    v = as_z3_expression(tree.children[0]) / as_z3_expression(tree.children[1]); break;
                        case operator_type_t::percent:  v = as_z3_expression(tree.children[0]) % as_z3_expression(tree.children[1]); break;
                        case operator_type_t::hat:      v = z3::pw(as_z3_expression(tree.children[0]), as_z3_expression(tree.children[1])); break;
                        case operator_type_t::_and: v = as_z3_expression(tree.children[0]) && as_z3_expression(tree.children[1]); break;
                        case operator_type_t::_or:  v = as_z3_expression(tree.children[0]) || as_z3_expression(tree.children[1]); break;
                        case operator_type_t::_xor: v = as_z3_expression(tree.children[0]) xor as_z3_expression(tree.children[1]); break;
                        case operator_type_t::_not: v =!as_z3_expression(tree.children[0]); break;
                        case operator_type_t::_implies: v = implies(as_z3_expression(tree.children[0]),as_z3_expression(tree.children[1])); break;
                        case operator_type_t::gt: v = (as_z3_expression(tree.children[0]) > as_z3_expression(tree.children[1])); break;
                        case operator_type_t::ge: v = (as_z3_expression(tree.children[0]) >= as_z3_expression(tree.children[1])); break;
                        case operator_type_t::ne: v = (as_z3_expression(tree.children[0]) != as_z3_expression(tree.children[1])); break;
                        case operator_type_t::ee: v = (as_z3_expression(tree.children[0]) == as_z3_expression(tree.children[1])); break;
                        case operator_type_t::le: v = (as_z3_expression(tree.children[0]) <= as_z3_expression(tree.children[1])); break;
                        case operator_type_t::lt: v = (as_z3_expression(tree.children[0]) < as_z3_expression(tree.children[1])); break;
                        case operator_type_t::parentheses: v = (as_z3_expression(tree.children[0])); break;
                    }
                },
                [&v,this](const symbol_value_t& o){ v = as_z3_expression(o); },
                [&](const root_t& r){ v = as_z3_expression(tree.children[0]); },
                [](auto&&){ throw std::logic_error("tree node type not recognized"); }
        ), static_cast<const underlying_syntax_node_t&>(tree.node));
        return v;
    }
}
