#ifndef EXPR_Z3_DRIVER_H
#define EXPR_Z3_DRIVER_H
#include "operations.h"
#include "parser/driver.h"
#include <c++/z3++.h>

namespace expr {
    struct z3_driver : public driver {
        explicit z3_driver(const symbol_table_t &env);
        ~z3_driver() override = default;

        auto parse(const std::string &f) -> int override;
        auto get_symbol(const std::string& identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;

        z3::context c{};
        z3::solver s;
        void solve();

        auto as_z3_expression(const syntax_tree_t& tree) -> z3::expr;
        auto as_z3_expression(const symbol_reference_t& ref) -> z3::expr;
        auto as_z3_expression(const c_symbol_reference_t& ref) -> z3::expr;
        auto as_z3_expression(const symbol_value_t& val) -> z3::expr;

    protected:
        const symbol_table_t &environment{};
    };

}

#endif //EXPR_Z3_DRIVER_H
