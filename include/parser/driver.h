#ifndef EXPR_DRIVER_H
#define EXPR_DRIVER_H
#include <string>
#include "symbol_table.h"
#include "parser.hpp"
#define YY_DECL yy::parser::symbol_type yylex (driver& drv)
YY_DECL;

struct driver {
    explicit driver(const symbol_table_t& env);
    const symbol_table_t& environment{};
    symbol_table_t result{};

    int parse(const std::string& f);
    std::string file;
    bool trace_parsing;

    void scan_begin();
    void scan_end();
    bool trace_scanning;
    yy::location location;
};

#endif
