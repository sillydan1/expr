#ifndef EXPR_INTERPRETER_H
#define EXPR_INTERPRETER_H
#include "parser/driver.h"

namespace expr {
    struct interpreter : public driver {
        explicit interpreter(const symbol_table_t &env);
        ~interpreter() override = default;

        auto parse(const std::string &f) -> int override;
        auto get_symbol(const std::string& identifier) -> symbol_value_t override;
        void set_symbol(const std::string& identifier, const symbol_value_t& value) override;
    };
}

#endif
