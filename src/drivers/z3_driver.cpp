#include "drivers/z3_driver.h"

namespace expr {
    z3_driver::z3_driver(const symbol_table_t& map) : environment{map}, c{}, s{c}, driver{} {
    }

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
        } catch (std::exception &e) {
            error = e.what();
            return 1;
        }
    }

    auto z3_driver::get_symbol(const std::string &identifier) -> syntax_tree_t {
#ifndef NDEBUG
        if (!environment.contains(identifier))
            throw std::out_of_range(identifier + " not found");
#endif
        return syntax_tree_t{environment.find(identifier)};
    }

    void z3_driver::add_tree(const syntax_tree_t& tree) {
        s.add(as_z3_expression(tree)); // Note: Only accepts boolean expressions (will throw if not)
        solve();
        s = z3::solver{c};
    }

    void z3_driver::add_tree(const std::string& identifier, const syntax_tree_t& tree) {
        std::cout << "z3_driver ignoring simple declaration" << std::endl;
    }

    void z3_driver::solve() {
        switch (s.check()) {
            case z3::unsat:   std::cout << "unsat\n"; break;
            case z3::unknown: std::cout << "unknown\n"; break;
            case z3::sat:
                auto m = s.get_model();
                for(int i = 0; i < m.size(); i++) {
                    auto xx = m[i];
                    auto interp = xx.is_const() ? m.get_const_interp(xx) : m.get_func_interp(xx).else_value();
                    result[xx.name().str()] = as_symbol_value(interp);
                }
                break;
        }
    }

    auto z3_driver::as_symbol_value(const z3::expr &e) -> symbol_value_t {
        if(e.is_int())
            return (int)e.as_int64();
        if(e.is_real())
            return (float)e.as_double();
        if(e.is_bool())
            return (bool)e;
        if(e.is_string_value())
            return e.get_string();
        throw std::logic_error("uhh");
    }

    auto z3_driver::as_z3_expression(const symbol_value_t& val) -> z3::expr {
        z3::expr v = c.int_val(0); // placeholder value
        std::visit(overload(
            [&v, this](const int& i)          { v = c.int_val(i); },
            [&v, this](const float& f)        { v = c.real_val(std::to_string(f).c_str()); },
            [&v, this](const bool& b)         { v = c.bool_val(b); },
            [&v, this](const std::string& sv) { v = c.string_val(sv); },
            [](auto&& x){ throw std::logic_error("unable to convert symbol value to z3::expr"); }
        ), static_cast<const underlying_symbol_value_t&>(val));
        return v;
    }

    auto z3_driver::as_z3_expression(const symbol_reference_t &ref) -> z3::expr {
        z3::expr v = c.int_val(0); // placeholder value
        std::visit(overload(
                [&v, this, &ref](const int& _)          { v = c.int_const(ref->first.c_str()); },
                [&v, this, &ref](const float& _)        { v = c.real_const(ref->first.c_str()); },
                [&v, this, &ref](const bool& _)         { v = c.bool_const(ref->first.c_str()); },
                [&v, this, &ref](const std::string& _)  { v = c.string_const(ref->first.c_str()); },
                [](auto&& x){ throw std::logic_error("unable to convert symbol value to z3::expr"); }
        ), static_cast<const underlying_symbol_value_t&>(ref->second));
        return v;
    }

    auto z3_driver::as_z3_expression(const c_symbol_reference_t &ref) -> z3::expr {
        z3::expr v = c.int_val(0); // placeholder value
        std::visit(overload(
            [&v, this, &ref](const int& _)          { v = c.int_const(ref->first.c_str()); },
            [&v, this, &ref](const float& _)        { v = c.real_const(ref->first.c_str()); },
            [&v, this, &ref](const bool& _)         { v = c.bool_const(ref->first.c_str()); },
            [&v, this, &ref](const std::string& _)  { v = c.string_const(ref->first.c_str()); },
            [](auto&& x){ throw std::logic_error("unable to convert symbol value to z3::expr"); }
        ), static_cast<const underlying_symbol_value_t&>(ref->second));
        return v;
    }

    auto z3_driver::as_z3_expression(const syntax_tree_t &tree) -> z3::expr {
        z3::expr v = c.int_val(0); // placeholder value
        std::visit(overload(
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
                        case operator_type_t::_not: v =!as_z3_expression(tree.children[0]); break;
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
                [](auto&&){ throw std::logic_error("operator type not recognized"); }
        ), static_cast<const underlying_syntax_node_t&>(tree.node));
        return v;
    }
}
