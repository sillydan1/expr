#include "parser/driver.h"
#include <utility>
#include "parser.hpp"

driver::driver(const symbol_table_t& map) : trace_parsing (false), trace_scanning (false), environment(map) {
}

int driver::parse(const std::string &f) {
    if(f.empty()) {
#ifdef DEFAULT_EXPRESSION_VALUE
        expression_result = DEFAULT_EXPRESSION_VALUE;
#endif
        return 0;
    }
    file = f;
    location.initialize (&file);
    scan_begin();
    yy::parser parse(this);
    parse.set_debug_level(trace_parsing);
    try {
        int res = parse();
        scan_end();
        return res;
    } catch(std::exception& e) {
        error = e.what();
        return 1;
    }
}

auto driver::get_symbol(const std::string &identifier) -> symbol_value_t {
#ifndef NDEBUG
    if(!environment.contains(identifier))
        throw std::out_of_range(identifier + " not found");
#endif
    return environment.at(identifier);
}
