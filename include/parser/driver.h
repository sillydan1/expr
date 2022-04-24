#ifndef EXPR_DRIVER_H
#define EXPR_DRIVER_H
#include <string>
#include "symbol_table.h"
#include "parser.hpp"
#define YY_DECL yy::parser::symbol_type yylex (driver* drv)
YY_DECL;

struct driver {
    explicit driver(const symbol_table_t& env);
    virtual ~driver() = default;
    const symbol_table_t& environment{};
    symbol_table_t result{};
    symbol_value_t error{};
    symbol_value_t expression_result{};
    std::string file;
    bool trace_parsing;
    bool trace_scanning;
    yy::location location;

    virtual int parse(const std::string& f);
    virtual auto get_symbol(const std::string& identifier) -> symbol_value_t;
    void scan_begin();
    void scan_end();
};

#endif
