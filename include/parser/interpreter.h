#ifndef EXPR_INTERPRETER_H
#define EXPR_INTERPRETER_H
#include "parser/driver.h"

namespace expr {
    struct interpreter : public driver {
        explicit interpreter(const symbol_table_t &env);
        ~interpreter() override = default;

        auto parse(const std::string &f) -> int override;
    };
}

#endif
