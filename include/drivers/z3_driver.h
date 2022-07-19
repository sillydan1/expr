#ifndef EXPR_Z3_DRIVER_H
#ifdef ENABLE_Z3
#define EXPR_Z3_DRIVER_H
#include "operations.h"
#include "drivers/driver.h"

namespace expr {
    struct z3_driver : public driver {
        struct impl;
        explicit z3_driver(const symbol_table_t &env);
        ~z3_driver() override;

        auto parse(const std::string &f) -> int override;
        auto get_symbol(const std::string& identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;

        symbol_table_t result{};
    protected:
        const symbol_table_t& environment{};
        std::unique_ptr<impl> pimpl;

        void solve();
    };

}

#endif //ENABLE_Z3
#endif //EXPR_Z3_DRIVER_H
