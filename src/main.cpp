#include <iostream>
#include "parser/driver.h"
#include "config.h"

int main (int argc, char *argv[]) {
    std::cout << PROJECT_NAME << " " << PROJECT_VER << std::endl;
    int res = 0;
    symbol_map_t env{};
    env["zero"] = 0;
    env["one"] = 1;
    env["two"] = 2;
    driver drv{env};
    try {
        for (int i = 1; i < argc; ++i) {
            if (argv[i] == std::string("-p"))
                drv.trace_parsing = true;
            else if (argv[i] == std::string("-s"))
                drv.trace_scanning = true;
            else if (!drv.parse(argv[i])) {
                for (auto &variable: drv.result)
                    std::cout << variable.first << " = " << variable.second << std::endl;
            } else
                res = 1;
        }
        return res;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
