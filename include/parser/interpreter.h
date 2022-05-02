#ifndef EXPR_INTERPRETER_H
#define EXPR_INTERPRETER_H
#include <string>
#include "symbol_table.h"
#include "parser.hpp"
#define YY_DECL yy::parser::symbol_type yylex (expr::interpreter* drv)
YY_DECL;

namespace expr {
    struct interpreter {
        explicit interpreter(const symbol_table_t &env);
        virtual ~interpreter() = default;

        virtual int parse(const std::string &f);
        virtual auto get_symbol(const std::string &identifier) -> symbol_value_t;
        virtual void set_symbol(const std::string &identifier, const symbol_value_t &value);
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

#endif
