#ifndef EXPR_DRIVER_H
#define EXPR_DRIVER_H
#include <string>
#include <map>
#include "parser.hpp"
#define YY_DECL yy::parser::symbol_type yylex (driver& drv)
YY_DECL;
using symbol_map_t = std::map<std::string, int>;

struct driver {
    explicit driver(const symbol_map_t& env);
    const symbol_map_t& environment{};
    symbol_map_t result{};

    int parse(const std::string& f);
    std::string file;
    bool trace_parsing;

    void scan_begin();
    void scan_end();
    bool trace_scanning;
    yy::location location;
};

#endif
