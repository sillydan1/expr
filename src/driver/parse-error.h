#ifndef EXPR_DRIVER_H
#define EXPR_DRIVER_H
#include "symbol_table.h"
#include <stdexcept>

namespace expr {
    struct parse_error : public std::logic_error {
        parse_error(const std::string& msg) : std::logic_error(msg.c_str()) {}
    };
}

// std::istringstream s{expression};
// ast_factory factory{};
// declaration_tree_builder builder{};
// scanner sc{s, std::cerr, &factory};
// parser_args pa{&sc, &factory, &builder};
// parser p{pa};
// auto pp = p.parse();
// auto res = builder.build();
#endif

