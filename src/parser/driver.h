#ifndef EXPR_DRIVER_H
#define EXPR_DRIVER_H
#include <string>
#include "symbol_table.h"
#include "parser.hpp"
#define YY_DECL yy::parser::symbol_type yylex (expr::driver* drv)
YY_DECL;

namespace expr {
    struct driver : public arithmetic_operator, boolean_operator, compare_operator {
        explicit driver(const symbol_table_t &env) : trace_parsing(false), trace_scanning(false), environment(env) {}
        virtual ~driver() = default;

        virtual int parse(const std::string &f) = 0;
        virtual auto get_symbol(const std::string &identifier) -> symbol_value_t = 0;
        virtual void set_symbol(const std::string &identifier, const symbol_value_t &value) = 0;
        void scan_begin();
        void scan_end();

        const symbol_table_t &environment{};
        symbol_table_t result{};
        symbol_value_t error{};
        symbol_value_t expression_result{};
        std::string file;
        bool trace_parsing;
        bool trace_scanning;
        yy::location location;
    };
}

#endif //EXPR_DRIVER_H
