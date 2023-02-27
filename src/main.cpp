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
#include "config.h"
#include "symbol_table.h"
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
    std::cout << "this main file is still not working. Look again later" << std::endl;
    return 0;
}
