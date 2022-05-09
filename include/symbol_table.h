#ifndef EXPR_SYMBOL_TABLE_H
#define EXPR_SYMBOL_TABLE_H
#include <utility>
#include <variant>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>
#include <tree>
#include "overload.h"

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
    symbol_value_t& interpret(const std::string& s) {
        std::stringstream ss{s};
        int i; float f; bool b;
        if(ss >> i)
            this->underlying_symbol_value_t::operator=(i);
        else if(ss >> f)
            this->underlying_symbol_value_t::operator=(f);
        else if(ss >> b)
            this->underlying_symbol_value_t::operator=(b);
        else
            this->underlying_symbol_value_t::operator=(s);
        return *this;
    }
    symbol_value_t& operator<<=(const std::string& s) {
        return interpret(s);
    }
};

using underlying_symbol_table_t = std::map<std::string, symbol_value_t>;
struct symbol_table_t : public underlying_symbol_table_t {
    symbol_table_t& operator+=(const symbol_table_t&);
};
symbol_table_t operator+(const symbol_table_t& a, const symbol_table_t& b);
std::ostream& operator<<(std::ostream& os, const symbol_value_t& v);
std::ostream& operator<<(std::ostream& os, const symbol_table_t& m);

namespace expr {
    struct operator_t {
        std::string operator_str;
        explicit operator_t(std::string s) : operator_str{std::move(s)} {}
    };
    struct root_t {};
    using syntax_node_t = std::variant<symbol_value_t, operator_t, root_t>;
    using syntax_tree_t = tree<syntax_node_t>;
}

auto operator<<(std::ostream& o, const expr::operator_t& p) -> std::ostream&;
auto operator<<(std::ostream& o, const expr::root_t& r) -> std::ostream&;
auto operator<<(std::ostream& o, const expr::syntax_node_t& n) -> std::ostream&;

#endif
