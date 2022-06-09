#ifndef EXPR_COMPILER_H
#define EXPR_COMPILER_H
#include "parser/driver.h"

namespace expr {
    class compiler : public driver {
    public:
        explicit compiler(const symbol_table_t &env) : driver{}, trees{}, environment{env} {}
        int parse(const std::string &f) override;
        auto get_symbol(const std::string &identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;

        std::map<std::string, syntax_tree_t> trees;
    protected:
        const symbol_table_t& environment{};
    };
}

#endif //EXPR_COMPILER_H
