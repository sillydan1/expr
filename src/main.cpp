#include <iostream>
#include "driver.h"

int main (int argc, char *argv[]) {
    int res = 0;
    symbol_map_t env{};
    env["one"] = 1;
    env["two"] = 2;
    driver drv{env};
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == std::string("-p"))
            drv.trace_parsing = true;
        else if (argv[i] == std::string("-s"))
            drv.trace_scanning = true;
        else if (!drv.parse(argv[i])) {
            for (auto &variable: drv.result)
                std::cout << variable.first << " = " << to_string(variable.second) << std::endl;
        } else
            res = 1;
    }
    return res;
}
