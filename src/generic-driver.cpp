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
#include "generic-driver.h"
#include "expr-scanner.hpp"
#include "expr-parser.hpp"
#include "ast-factory.h"
#include "expr-lang/language-builder.h"
#include "symbol_table.h"
#include "driver/evaluator.h"
#include <stdexcept>
#ifdef ENABLE_Z3
#include "driver/z3/z3-driver.h"
#endif

namespace expr {
    auto generic_driver::get_symbol_table(const symbol_table_t& env) -> symbol_table_t {
        symbol_operator op{};
        evaluator e{{env}, op};
        symbol_table_t result{};
        for(auto& r : declarations)
            result[r.first] = e.evaluate(r.second);
        return result;
    }

    auto generic_driver::get_expression_value(const symbol_table_t& env) -> symbol_value_t {
        if(!expression)
            throw std::logic_error("language result does not contain a rhs expression");
        symbol_operator op{};
        evaluator e{{env}, op};
        return e.evaluate(expression.value());
    }

    void generic_driver::print(const symbol_table_t& known, const symbol_table_t& unknown) {
        std::cout << "ast:\n";
        for(auto& d : declarations)
            std::cout << "\t> " << d.first << " :-> " << d.second << "\n";
        if(expression)
            std::cout << "\t> raw rhs: " << expression.value() << "\n";
        std::cout << "evaluated:\n";
        for(auto& s : get_symbol_table(known + unknown))
            std::cout << "\t> " << s.first << " :-> " << s.second << "\n";
        if(expression)
            std::cout << "\t> raw rhs: " << get_expression_value(known + unknown) << "\n";
        std::cout << "z3 sat check: (apply these changes to satisfy the expression)\n";
#ifdef ENABLE_Z3
        if(!expression) {
            std::cout << "\t> not a raw rhs, cannot check for sat\n";
            return;
        }
        auto ex = get_expression_value(known + unknown);
        if(!std::holds_alternative<bool>(ex)) {
            std::cout << "\t> z3 requires the expression to be a boolean expression\n";
            return;
        }
        z3_driver z{known, unknown};
        auto sol = z.find_solution(expression.value());
        if(!sol)
            std::cout << "\t> unsatisfiable\n";
        else {
            if(sol.value().empty())
                std::cout << "\t> already satisfied\n";
            else
                std::cout << "\t> " << sol.value() << "\n";
        }
#else
        std::cout << "\t> not compiled with z3 support\n"; 
#endif
    }

    void generic_driver::parse_expressions(const std::string& s) { 
        std::istringstream iss{s};
        ast_factory factory{};
        declaration_tree_builder builder{};
        scanner sc{iss, std::cerr, &factory};
        parser_args pa{s, &sc, &factory, &builder};
        parser p{pa};
        if(p.parse() != 0)
            throw std::logic_error("unable to parse the expression(s)" + s);
        auto res = builder.build();
        for(auto& e : res.declarations)
            declarations[e.first] = e.second.tree;
        if(res.raw_expression)
            expression = res.raw_expression.value();
    }

    auto generic_driver::parse_symbol_table(const std::string& s) -> symbol_table_t {
        std::istringstream iss{s};
        ast_factory factory{};
        declaration_tree_builder builder{};
        scanner sc{iss, std::cerr, &factory};
        parser_args pa{s, &sc, &factory, &builder};
        parser p{pa};
        if(p.parse() != 0)
            throw std::logic_error("unable to parse the expression(s): " + s);
        auto res = builder.build();
        symbol_operator op{};
        evaluator e{{}, op};
        symbol_table_t result{};
        for(auto& r : res.declarations)
            result[r.first] = e.evaluate(r.second.tree);
        return result;
    }
}

