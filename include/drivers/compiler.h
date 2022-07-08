#ifndef EXPR_COMPILER_H
#define EXPR_COMPILER_H
#include <map>
#include <unordered_map>
#include "drivers/driver.h"

namespace expr {
    class compiler : public driver {
    public:
        using compiled_expr_t = syntax_tree_t;
#ifndef NDEBUG
        using compiled_expr_collection_t = std::map<std::string, compiled_expr_t>;
#else
        using compiled_expr_collection_t = std::unordered_map<std::string, compiled_expr_t>;
#endif
        explicit compiler(const symbol_table_t &env) : driver{}, trees{}, environment{env} {}
        int parse(const std::string &f) override;
        auto get_symbol(const std::string &identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;
        auto get_environment() const -> const symbol_table_t& { return environment; }

        compiled_expr_collection_t trees;
    protected:
        const symbol_table_t& environment{};
    };
}

#endif //EXPR_COMPILER_H
