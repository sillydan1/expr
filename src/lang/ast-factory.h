#ifndef EXPR_FACTORY_H
#define EXPR_FACTORY_H
#include "symbol_table.h"

namespace expr {
    /* 
     * Use this to build abstract syntax trees
     * */
    class ast_factory {
    public:
        ast_factory() = default;
        virtual ~ast_factory() = default;
        virtual auto build_operator(const operator_type_t& op, const syntax_tree_t& child) -> syntax_tree_t;
        virtual auto build_operator(const operator_type_t& op, const syntax_tree_t& child1, const syntax_tree_t& child2) -> syntax_tree_t;
        virtual auto build_operator(const operator_type_t& op, const std::vector<syntax_tree_t>& children) -> syntax_tree_t;
        virtual auto build_literal(const symbol_value_t& value) -> syntax_tree_t;
        virtual auto build_identifier(const std::string& identifier) -> syntax_tree_t;
        virtual auto build_root(const syntax_tree_t& child) -> syntax_tree_t;

        virtual auto build_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_access_modifier_t& access_modifier) -> syntax_tree_t;
        virtual auto build_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_type_name_t& type_name = symbol_type_name_t::_auto, const symbol_access_modifier_t& access_modifier = symbol_access_modifier_t::_private) -> syntax_tree_t;
    };
}

#endif
