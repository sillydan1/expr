#ifndef EXPR_TREE_INTERPRETER_H
#define EXPR_TREE_INTERPRETER_H
#include "tree_driver.h"
#include "compiler.h"
#include "generic_symbol_operator.h"

namespace expr {
    struct tree_interpreter : public tree_driver, generic_symbol_operator {
        tree_interpreter(const symbol_table_tree_t::_left_df_iterator& it);
        ~tree_interpreter() override = default;

        auto parse(const std::string &f) -> int override;
        auto interpret_declarations(const std::string& f) -> symbol_table_t;
        auto interpret_expression(const std::string& f) -> symbol_value_t;
        auto get_symbol(const std::string& identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;

        symbol_table_t result{};
        symbol_value_t expression_result{};

        auto evaluate(const syntax_tree_t& tree) -> symbol_value_t;
        auto evaluate(const compiler::compiled_expr_collection_t& tree) -> symbol_table_t;
    };
}

#endif //EXPR_TREE_INTERPRETER_H
