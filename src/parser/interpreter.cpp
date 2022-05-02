#include "parser/interpreter.h"
#include <utility>
#include "parser.hpp"

namespace expr {
    interpreter::interpreter(const symbol_table_t &map) : driver{map} {
    }

    int interpreter::parse(const std::string &f) {
        if (f.empty()) {
#ifdef DEFAULT_EXPRESSION_VALUE
            expression_result = DEFAULT_EXPRESSION_VALUE;
#endif
            return 0;
        }
        file = f;
        location.initialize(&file);
        scan_begin();
        yy::parser parse(this);
        parse.set_debug_level(trace_parsing);
        try {
            int res = parse();
            scan_end();
            return res;
        } catch (std::exception &e) {
            error = e.what();
            return 1;
        }
    }

    auto interpreter::get_symbol(const std::string &identifier) -> symbol_value_t  {
#ifndef NDEBUG
        if (!environment.contains(identifier))
            throw std::out_of_range(identifier + " not found");
#endif
        return environment.at(identifier);
    }

    void interpreter::set_symbol(const std::string &identifier, const symbol_value_t &value) {
        result[identifier] = value;
    }
}
