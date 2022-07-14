#ifndef EXPR_INTERPRETER_H
#define EXPR_INTERPRETER_H
#include "operations.h"
#include "drivers/driver.h"
#include "compiler.h"

namespace expr {
    struct interpreter : public driver, arithmetic_operator, boolean_operator, compare_operator {
        explicit interpreter(const symbol_table_t &env);
        ~interpreter() override = default;

        auto parse(const std::string &f) -> int override;
        auto get_symbol(const std::string& identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;

        auto add(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return ::operator+(a,b); };
        auto sub(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a - b; };
        auto mul(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a * b; };
        auto div(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a / b; };
        auto mod(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a % b; };
        auto pow(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a ^ b; };

        auto _not(const symbol_value_t &a) -> symbol_value_t override { return not_(a); }
        auto _and(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return and_(a,b); }
        auto _or(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return or_(a,b); }
        auto _xor(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return xor_(a,b); }

        auto gt(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return gt_(a,b); }
        auto ge(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return ge_(a,b); }
        auto ee(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return ee_(a,b); }
        auto ne(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return ne_(a,b); }
        auto le(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return le_(a,b); }
        auto lt(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return lt_(a,b); }

        symbol_table_t result{};
        symbol_value_t expression_result{};

        static auto evaluate(const syntax_tree_t& tree, expr::arithmetic_operator& arith, expr::boolean_operator& boolean, expr::compare_operator& comparator) -> symbol_value_t;
        static auto evaluate(const compiler::compiled_expr_collection_t& symbol_tree_map, expr::arithmetic_operator& arith, expr::boolean_operator& boolean, expr::compare_operator& comparator) -> symbol_table_t;

    protected:
        const symbol_table_t &environment{};
    };
}

#endif
