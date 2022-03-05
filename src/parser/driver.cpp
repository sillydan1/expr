#include "parser/driver.h"
#include <utility>
#include "parser.hpp"

driver::driver(const symbol_table_t& map) : trace_parsing (false), trace_scanning (false), environment(map) {
}

int driver::parse(const std::string &f) {
    file = f;
    location.initialize (&file);
    scan_begin();
    yy::parser parse(*this);
    parse.set_debug_level(trace_parsing);
    int res = parse();
    scan_end();
    return res;
}
