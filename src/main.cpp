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
#include <unistd.h>
#include "config.h"
#include "generic-driver.h"

int main (int argc, char *argv[]) {
    using namespace expr;
    std::vector<option_t> my_options = {
        {"expression", 'e',            argument_requirement::REQUIRE_ARG,  "provide the expression to process, prompt appears if not provided"},
        {"environment", 'm',           argument_requirement::OPTIONAL_ARG, "provide an environment, use \"-\" for interactive prompt"},
        {"unknown-environment", 'u',   argument_requirement::REQUIRE_ARG,  "provide an environment of unknown variables (z3 only), use \"-\" for interactive prompt"},
    };
    auto cli_arguments = get_arguments(my_options, argc, argv);
    if(cli_arguments["help"]) {
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
    std::string environment = cli_arguments["environment"].as_string_or_default("");
    if(environment == "-") {
        std::rewind(stdin);
        std::cout << "provide an environment. End with <<EOF>> (ctrl+d):\n<<\n";
        std::istreambuf_iterator<char> begin(std::cin), end;
        environment = std::string(begin, end);
        std::cout << "\n>>\n";
    }
    std::string unknown_environment = cli_arguments["unknown-environment"].as_string_or_default("");
    if(unknown_environment == "-") {
        std::rewind(stdin);
        std::cout << "provide an environment. End with <<EOF>> (ctrl+d):\n<<\n";
        std::istreambuf_iterator<char> begin(std::cin), end;
        unknown_environment  = std::string(begin, end);
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

    expr::generic_driver c{};
    c.parse_expressions(expression);
    c.print(c.parse_symbol_table(environment), c.parse_symbol_table(unknown_environment)); 
    return 0;
}

