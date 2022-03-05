#ifndef EXPR_SYMBOL_TABLE_H
#define EXPR_SYMBOL_TABLE_H
#include <variant>
#include <map>
#include <functional>
#include <iostream>

using underlying_symbol_value_t = std::variant<int,float,bool,std::string>;
struct symbol_value_t : public underlying_symbol_value_t {
    symbol_value_t() = default;
    template<typename T>
    symbol_value_t(const T& x) : underlying_symbol_value_t(x) {}
    template<typename T>
    symbol_value_t& operator=(const T& t) {
        this->underlying_symbol_value_t::operator=(t);
        return *this;
    }
};
using symbol_map_t = std::map<std::string, symbol_value_t>;
std::ostream& operator<<(std::ostream& os, const symbol_value_t& v);
std::ostream& operator<<(std::ostream& os, const symbol_map_t& m);

#endif
