#include <iostream>
#include "parser/interpreter.h"
#include "config.h"
#include "parser/compiler.h"
#include "parser/z3_driver.h"
#include <argvparse.h>
#include <Timer.hpp>
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
                << "like so: 'a := one + 30'. Variable assignments are done atomically, so\n"
                << "statements like these: 'a := 2 ; b := a + 1' will not compile, because\n"
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

        std::shared_ptr<driver> drv{};
        if(cli_arguments["driver"].as_string() == "compiler")
            drv = std::make_shared<compiler>(env);
        if(cli_arguments["driver"].as_string() == "interpreter")
            drv = std::make_shared<interpreter>(env);
        if(cli_arguments["driver"].as_string() == "z3")
            drv = std::make_shared<z3_driver>(env);

        drv->trace_parsing = static_cast<bool>(cli_arguments["parser-trace"]);
        drv->trace_scanning = static_cast<bool>(cli_arguments["scanner-trace"]);
        Timer<int> t{};
        t.start();
        auto res = drv->parse(cli_arguments["expression"].as_string());
        if(res != 0) {
            std::cout << "error: " << drv->error << "\n";
            return res;
        }

        if(cli_arguments["driver"].as_string() == "compiler") {
            auto drv_c = std::dynamic_pointer_cast<compiler>(drv);
            for(auto& tree : drv_c->trees)
                std::cout << tree.first << ": " << tree.second << "\n";
        }
        if(cli_arguments["driver"].as_string() == "interpreter") {
            auto drv_i = std::dynamic_pointer_cast<interpreter>(drv);
            if(!drv_i->result.empty())
                std::cout << drv_i->result << "\n";
            std::cout << "expression_result" << drv_i->expression_result << std::endl;
        }
        if(cli_arguments["driver"].as_string() == "z3") {
            auto drv_z = std::dynamic_pointer_cast<z3_driver>(drv);
            std::cout << "result: \n" << drv_z->result;
        }
        std::cout << "\n" << t.milliseconds_elapsed() << "ms" << std::endl;
        return res;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
