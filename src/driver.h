#ifndef EXPR_DRIVER_H
#define EXPR_DRIVER_H
#include <string>
#include <map>
#include "parser.hpp"
#define YY_DECL yy::parser::symbol_type yylex (driver& drv)
YY_DECL;

struct driver {
    driver();
    std::map<std::string, int> variables{};
    int result{};

    int parse(const std::string& f);
    std::string file;
    bool trace_parsing;

    void scan_begin();
    void scan_end();
    bool trace_scanning;
    yy::location location;
};

#endif
