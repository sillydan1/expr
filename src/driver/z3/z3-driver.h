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
        z3::context c{};
        z3::solver s;
        std::string delay_identifier;
        const symbol_table_t& known;
        const symbol_table_t& unknown;
    };
}

#endif

