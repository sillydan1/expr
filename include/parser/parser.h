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
    };
}

#endif //EXPR_PARSER_H
