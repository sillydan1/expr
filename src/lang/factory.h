#ifndef EXPR_FACTORY_H
#define EXPR_FACTORY_H
#include "symbol_table.h"

namespace expr {
    /// AST factory
    /// Use this to build abstract syntax trees
    class ast_factory {
    public:
        factory() = default;
        virtual ~factory() = default;
        virtual auto build_operator(const expr::operator_type_t& op, const std::vector<expr::syntax_tree_t>& children) -> expr::syntax_tree_t;
        virtual auto build_literal(const expr::symbol_value_t& value) -> expr::syntax_tree_t;
        virtual auto build_identifier(const std::string& identifier) -> expr::syntax_tree_t;
        virtual auto build_root(const syntax_tree_t& child) -> expr::syntax_tree_t;
        virtual auto build_declaration(const std::string& identifier, const syntax_tree_t& tree) -> expr::syntax_tree_t;
        virtual auto build_declaration(const std::string& identifier, const std::string& access_modifier, const syntax_tree_t& tree) -> expr::syntax_tree_t;
    };
}

#endif

