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
#include "drivers/interpreter.h"
#include "drivers/compiler.h"
#include "drivers/z3_driver.h"
#include "config.h"
#include "drivers/tree_interpreter.h"
#include <argvparse.h>
#include <timer>
#include <memory>

int main (int argc, char *argv[]) {
    using namespace expr;
    symbol_table_t env{};
    env["false_b"] = false;
    env["one_i"] = 1;
    env["two_f"] = 2.0f;
    env["hello_s"] = "Hello";
    std::vector<option_t> my_options = {
        {"expression", 'e',    argument_requirement::REQUIRE_ARG, "(required) provide the expression to process"},
        {"driver", 'd',        argument_requirement::REQUIRE_ARG, "(required) determine which driver to use [z3, interpreter, compiler]"},
        {"environment", 'm',   argument_requirement::OPTIONAL_ARG, "provide an environment"},
        {"unknown-environment", 'u',   argument_requirement::REQUIRE_ARG, "provide an environment of unknown variables (z3 driver only)"},
        {"parser-trace", 'p',  argument_requirement::NO_ARG, "enable tracing for the parser"},
        {"scanner-trace", 's', argument_requirement::NO_ARG, "enable tracing for the scanner"},
    };
    auto cli_arguments = get_arguments(my_options, argc, argv);
    if(cli_arguments["help"] || !cli_arguments["expression"] || !cli_arguments["driver"]) {
        std::cout
            << "=================== Welcome to the " << PROJECT_NAME << " v" << PROJECT_VER << " demo ==================\n"
            << "USAGE: " << argv[0] << " [OPTIONS]\n"
            << "\n"
            << "For this demo, a simple environment has been provided. (see below)\n"
            << "You can use these variables on the right-hand-side of your expressions\n"
            << "Another sentence in my help message"
            << "like so: 'a := one + 30'. Variable assignments are done atomically, so\n"
            << "statements lie these: 'a := 2 ; b := a + 1' will not compile, because\n"
            << "Another thing"
            << "the variable 'a' is not defined before AFTER all assignments have been\n"
            << "evaluated and performed.\n"
            << "PROVIDED ENVIRONMENT:\n"
            << env
            << "\n"
            << "OPTIONS:\n"
            << my_options
            << "======================================================================\n";
        return 0;
    }
    try {
        if(cli_arguments["environment"]) {
            interpreter i{{}};
            auto res = i.parse(cli_arguments["environment"].as_string());
            if(res != 0) {
                std::cout << "error: " << i.error << std::endl;
                return res;
            }
            env = i.result;
        }
        symbol_table_t unknowns{};
        if(cli_arguments["unknown-environment"]) {
            interpreter i{{}};
            auto res = i.parse(cli_arguments["unknown-environment"].as_string());
            if(res != 0) {
                std::cout << "error: " << i.error << std::endl;
                return res;
            }
            unknowns = i.result;
        }

        std::shared_ptr<driver> drv{};
        if(cli_arguments["driver"].as_string() == "compiler")
            drv = std::make_shared<compiler>(std::initializer_list<std::reference_wrapper<const expr::symbol_table_t>>{env});
        else if(cli_arguments["driver"].as_string() == "interpreter")
            drv = std::make_shared<interpreter>(std::initializer_list<std::reference_wrapper<const expr::symbol_table_t>>{env});
        else if(cli_arguments["driver"].as_string() == "z3")
            drv = std::make_shared<z3_driver>(env,unknowns);
        else {
            std::cerr << "no such driver available " << cli_arguments["driver"].as_string()
                      << " please check your spelling and compilation flags" << std::endl;
            return 1;
        }

        drv->trace_parsing = static_cast<bool>(cli_arguments["parser-trace"]);
        drv->trace_scanning = static_cast<bool>(cli_arguments["scanner-trace"]);
        ya::timer<int> t{};
        auto res = drv->parse(cli_arguments["expression"].as_string());
        if(res != 0) {
            std::cout << "error: " << drv->error << "\n";
            return res;
        }

        if(cli_arguments["driver"].as_string() == "compiler") {
            auto drv_c = std::dynamic_pointer_cast<compiler>(drv);
            for(auto& tree : drv_c->trees)
                std::cout << tree.first << ": " << tree.second << "\n";
            std::cout << "\n";
            interpreter i{{env}};
            std::cout << i.evaluate(drv_c->trees) << "\n";
        }
        if(cli_arguments["driver"].as_string() == "interpreter") {
            auto drv_i = std::dynamic_pointer_cast<interpreter>(drv);
            if(!drv_i->result.empty())
                std::cout << drv_i->result << "\n";
            std::cout << "expression_result: " << drv_i->expression_result << std::endl;
        }
        if(cli_arguments["driver"].as_string() == "z3") {
            auto drv_z = std::dynamic_pointer_cast<z3_driver>(drv);
            std::cout << "result: \n" << drv_z->result;

            std::cout << "\n==========\n";
            std::cout << "env + result: \n" << (env + drv_z->result);
        }
        std::cout << "\n" << t.milliseconds_elapsed() << "ms" << std::endl;
        return res;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
