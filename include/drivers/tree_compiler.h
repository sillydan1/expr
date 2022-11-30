#ifndef EXPR_TREE_COMPILER_H
#define EXPR_TREE_COMPILER_H
#include "tree_driver.h"

namespace expr {
    struct tree_compiler : tree_driver {
        using compiled_expr_t = syntax_tree_t;
#ifndef NDEBUG
        using compiled_expr_collection_t = std::map<std::string, compiled_expr_t>;
#else
        using compiled_expr_collection_t = std::unordered_map<std::string, compiled_expr_t>;
#endif
        explicit tree_compiler(const symbol_table_tree_t::iterator& scope) : tree_driver{scope}, trees{} {}
        int parse(const std::string &f) override;
        auto get_symbol(const std::string &identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;
        void add_tree(const std::string& access_modifier, const std::string& identifier, const syntax_tree_t& tree) override;

        compiled_expr_collection_t trees;
    };
}

#endif //EXPR_TREE_COMPILER_H
