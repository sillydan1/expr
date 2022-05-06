#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H
#include "parser/driver.h"

namespace expr {
    class parser : public driver {
    public:
        explicit parser(const symbol_table_t &env) : driver{env} {}
        int parse(const std::string &f) override;
        auto get_symbol(const std::string &identifier) -> symbol_value_t override;
        void set_symbol(const std::string &identifier, const symbol_value_t &value) override;
    private:
        auto add(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a + b; };
        auto sub(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a - b; };
        auto mul(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a * b; };
        auto div(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a / b; };
        auto mod(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a % b; };
        auto pow(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return a ^ b; };

        auto _not(const symbol_value_t &a) -> symbol_value_t override { return not_(a); }
        auto _and(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return and_(a,b); }
        auto _or(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return or_(a,b); }

        auto gt(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return gt_(a,b); }
        auto ge(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return ge_(a,b); }
        auto ee(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return ee_(a,b); }
        auto ne(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return ne_(a,b); }
        auto le(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return le_(a,b); }
        auto lt(const symbol_value_t &a, const symbol_value_t &b) -> symbol_value_t override { return lt_(a,b); }
    };
}

#endif //EXPR_PARSER_H
