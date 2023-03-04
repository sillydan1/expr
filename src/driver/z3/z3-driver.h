#ifndef EXPR_Z3_DRIVER_H
#define EXPR_Z3_DRIVER_H
#include "../parse-error.h"
#include "symbol_table.h"
#include <z3++.h>

namespace expr {
    class z3_driver {
    public:
        z3_driver(const symbol_table_t& known, const symbol_table_t& unknown);
        auto find_solution(const syntax_tree_t& expression) -> std::optional<symbol_table_t>;
    private:
        auto as_z3(const syntax_tree_t &tree) -> z3::expr;
        auto as_z3(const identifier_t& ref) -> z3::expr;
        auto as_z3(const symbol_value_t& val) -> z3::expr;
        auto as_symbol_value(const z3::expr& e) -> symbol_value_t;
        z3::context c;
        z3::solver s;
        std::string delay_identifier;
        const symbol_table_t& known;
        const symbol_table_t& unknown;
    };
}

#endif

