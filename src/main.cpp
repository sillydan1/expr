#include <iostream>
#include "parser/interpreter.h"
#include "config.h"
#include "parser/compiler.h"
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
            {"expression", 'e',   argument_requirement::REQUIRE_ARG, "(required) provide the expression to process"},
            {"parser-trace",'p',  argument_requirement::NO_ARG, "enable tracing for the parser"},
            {"scanner-trace",'s', argument_requirement::NO_ARG, "enable tracing for the scanner"},
            {"compile", 'c',      argument_requirement::NO_ARG, "compile expression instead of interpretation"},
    };
    auto cli_arguments = get_arguments(my_options, argc, argv);
    if(cli_arguments["help"] || !cli_arguments["expression"]) {
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
        std::shared_ptr<driver> drv = std::make_shared<interpreter>(env);
        if(cli_arguments["compile"])
            drv = std::make_unique<compiler>(env);
        drv->trace_parsing = static_cast<bool>(cli_arguments["parser-trace"]);
        drv->trace_scanning = static_cast<bool>(cli_arguments["scanner-trace"]);
        Timer<int> t{};
        t.start();
        auto res = drv->parse(cli_arguments["expression"].as_string());
        if(cli_arguments["compile"]) {
            auto drv_c = std::dynamic_pointer_cast<compiler>(drv);
            if(res != 0)
                std::cout << "error: " << drv_c->error;
            for(auto& tree : drv_c->trees)
                std::cout << tree.first << ": " << tree.second << "\n";
        } else {
            auto drv_i = std::dynamic_pointer_cast<interpreter>(drv);
            if (res != 0)
                std::cout << "error: " << drv_i->error;
            else
                std::cout << "result: " << drv_i->result;
        }
        std::cout << "\n" << t.milliseconds_elapsed() << "ms" << std::endl;
        return res;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
