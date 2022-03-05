#include <iostream>
#include "parser/driver.h"
#include "config.h"

int main (int argc, char *argv[]) {
    symbol_table_t env{};
    env["false_b"] = false;
    env["one_i"] = 1;
    env["two_f"] = 2.0f;
    env["hello_s"] = "Hello";
    std::cout
        << "=================== Welcome to the " << PROJECT_NAME << " v" << PROJECT_VER << " demo ==================\n"
        << "USAGE: " << argv[0] << " [OPTIONS] EXPR_STR or '-' for using stdin\n"
        << "OPTIONS:\n"
        << " -p\t| enable tracing for the parser\n"
        << " -s\t| enable tracing for the scanner\n"
        << "\n"
        << "For this demo, a simple environment has been provided. (see below)\n"
        << "You can use these variables on the right-hand-side of your expressions\n"
        << "like so: 'a := one + 30'. Variable assignments are done atomically, so\n"
        << "statements like these: 'a := 2 ; b := a + 1' will not compile, because\n"
        << "the variable 'a' is not defined before AFTER all assignments have been\n"
        << "evaluated and performed.\n"
        << "PROVIDED ENVIRONMENT:\n"
        << env
        << "======================================================================\n" << std::endl;
    try {
        int res = 0;
        driver drv{env};
        for (int i = 1; i < argc; ++i) {
            if (argv[i] == std::string("-p"))
                drv.trace_parsing = true;
            else if (argv[i] == std::string("-s"))
                drv.trace_scanning = true;
            else if (!drv.parse(argv[i]))
                std::cout << drv.result;
            else
                res = 1;
        }
        return res;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
