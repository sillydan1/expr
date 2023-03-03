#ifndef EXPR_INTERPRETER_H
#define EXPR_INTERPRETER_H
#include "operations/symbol-operator.h"
#include "symbol_table.h"

namespace expr {
    using symbol_table_ref_collection_t = std::vector<std::reference_wrapper<const expr::symbol_table_t>>;
    class evaluator {
    public:
        evaluator(const symbol_table_ref_collection_t& environments, const symbol_operator& op);
        virtual auto evaluate(const syntax_tree_t& tree) -> symbol_value_t;
        virtual auto contains(const std::string& identifier) const -> bool;
        virtual auto find(const std::string& identifier) const -> expr::symbol_table_t::const_iterator;
    private:
        symbol_table_ref_collection_t environments;
        expr::symbol_table_t::const_iterator end{};
        const symbol_operator& op;
    };
}

#endif

