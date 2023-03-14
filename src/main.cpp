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
#include <iostream>
#include <sstream>
#include <istream>
#include <memory>
#include <argvparse.h>
#include <stdexcept>
#include <timer>
#include "config.h"
#include "ast-factory.h"
#include "driver/evaluator.h"
#include "symbol_table.h"
#include "expr-scanner.hpp"
#include "expr-parser.hpp"
#ifdef ENABLE_Z3
#include "driver/z3/z3-driver.h"
#endif

struct language_result {
    std::map<std::string, expr::syntax_tree_t> declarations;
    std::optional<expr::syntax_tree_t> expression;
    auto get_symbol_table() -> expr::symbol_table_t {
        expr::symbol_operator op{};
        expr::evaluator e{{}, op};
        expr::symbol_table_t env{};
        for(auto& r : declarations)
            env[r.first] = e.evaluate(r.second);
        return env;
    }
};

struct cli_context {
    cli_context(const std::string& environment, const std::string& unknown) : known_environment{}, unknown_environment{} {
        known_environment = parse_expressions(environment).get_symbol_table();
        unknown_environment = parse_expressions(unknown).get_symbol_table();
    }

    void evaluate(const language_result& result) {
        expr::symbol_operator op{};
        expr::evaluator e{{known_environment}, op};
        expr::symbol_table_t env{};
        for(auto& r : result.declarations)
            env[r.first] = e.evaluate(r.second);
        std::cout << " evaluated: \n";
        for(auto& s : env)
            std::cout << "\t" << s.first << " :-> " << s.second << "\n";
        if(result.expression)
            std::cout << "\t" << e.evaluate(result.expression.value()) << "\n";
    }

    void compile(const language_result& result) {
        std::cout << " result: \n";
        for(auto& r : result.declarations)
            std::cout << "\t" << r.first << " :=> " << r.second << "\n";
        if(result.expression)
            std::cout << "\t expression = " << result.expression.value() << "\n";
    }

#ifdef ENABLE_Z3
    void sat_check(const language_result& result) {
        std::cout << " sat check: \n";
        expr::z3_driver z{known_environment, unknown_environment};
        if(result.expression) {
            auto sol =  z.find_solution(result.expression.value());
            if(!sol)
                std::cout << "\tunsat\n";
            else {
                if(sol.value().empty())
                    std::cout << "\t already satisfied\n";
                else
                    std::cout << "\t" << sol.value() << "\n";
            }
        }
    }
#endif

    auto parse_expressions(const std::string& s) -> language_result {
        std::istringstream iss{s};
        expr::ast_factory factory{};
        expr::declaration_tree_builder builder{};
        expr::scanner sc{iss, std::cerr, &factory};
        expr::parser_args pa{s, &sc, &factory, &builder};
        expr::parser p{pa};
        if(p.parse() != 0)
            throw std::logic_error("unable to parse the expression(s)");
        auto res = builder.build();
        language_result result{};
        for(auto& e : res.declarations)
            result.declarations[e.first] = e.second.tree;
        if(res.raw_expression)
            result.expression = res.raw_expression.value();
        return result;
    }

public:
    expr::symbol_table_t known_environment{};
    expr::symbol_table_t unknown_environment{};
};

int main (int argc, char *argv[]) {
    using namespace expr;
    symbol_table_t env{};
    env["false_b"] = false;
    env["one_i"] = 1;
    env["two_f"] = 2.0f;
    env["hello_s"] = "Hello";
    std::vector<option_t> my_options = {
        {"expression", 'e',            argument_requirement::REQUIRE_ARG,  "(required) provide the expression to process"},
        {"driver", 'd',                argument_requirement::REQUIRE_ARG,  "(required) determine which driver to use [z3, evaluator, compiler]"},
        {"environment", 'm',           argument_requirement::OPTIONAL_ARG, "provide an environment"},
        {"unknown-environment", 'u',   argument_requirement::REQUIRE_ARG,  "provide an environment of unknown variables (z3 driver only)"},
    };
    auto cli_arguments = get_arguments(my_options, argc, argv);
    if(cli_arguments["help"] || !cli_arguments["expression"] || !cli_arguments["driver"]) {
        std::cout
            << "=================== Welcome to the " << PROJECT_NAME << " v" << PROJECT_VER << " demo ==================\n"
            << "USAGE: " << argv[0] << " [OPTIONS]\n"
            << "\n"
            << "With this demo, you can experiment with the language of expr.\n"
            << "You can manipulate variables by assigning them directly to a value, or\n"
            << "calculate based on some expression.\n"
            << "Assignment is done like so: 'a := one + 30'. Variable assignments are\n"
            << "done atomically, so statements like these: 'a := 2 ; b := a + 1' will\n"
            << "not compile, because the variable 'a' is not defined before AFTER all\n"
            << "assignments have been evaluated and performed.\n"
            << "\n"
            << "OPTIONS:\n"
            << my_options
            << "======================================================================\n";
        return 0;
    }
    std::string environment = "";
    if(cli_arguments["environment"].as_string_or_default("") == "-") {
        std::rewind(stdin);
        std::cout << "provide an environment. End with <<EOF>> (ctrl+d):\n<<\n";
        std::istreambuf_iterator<char> begin(std::cin), end;
        environment = std::string(begin, end);
        std::cout << "\n>>\n";
    }
    std::string expression = cli_arguments["expression"].as_string_or_default("-");
    if(expression == "-") {
        std::rewind(stdin);
        std::cout << "provide an expression. End with <<EOF>> (ctrl+d):\n<<\n";
        std::istreambuf_iterator<char> begin(std::cin), end;
        expression = std::string(begin, end);
        std::cout << "\n>>\n";
    }

    cli_context c{environment, cli_arguments["unknown-environment"].as_string_or_default("")};
    auto res = c.parse_expressions(expression);
    if(cli_arguments["driver"].as_string() == "evaluator")
        c.evaluate(res);
    else if(cli_arguments["driver"].as_string() == "compiler")
        c.compile(res);
#ifdef ENABLE_Z3
    else if(cli_arguments["driver"].as_string() == "z3")
        c.sat_check(res);
#endif
    else
        throw std::logic_error("unsupported driver");
    return 0;
}

