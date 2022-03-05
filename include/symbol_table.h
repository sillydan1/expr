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

using underlying_symbol_table_t = std::map<std::string, symbol_value_t>;
struct symbol_table_t : public underlying_symbol_table_t {
    symbol_table_t& operator+=(const symbol_table_t&);
};
symbol_table_t operator+(const symbol_table_t& a, const symbol_table_t& b);
std::ostream& operator<<(std::ostream& os, const symbol_value_t& v);
std::ostream& operator<<(std::ostream& os, const symbol_table_t& m);

#endif