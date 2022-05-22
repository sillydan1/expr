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
    symbol_value_t(const T& x) : underlying_symbol_value_t{x} { }
    template<typename T>
    auto& operator=(const T& t) {
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

enum class operator_type_t {
    minus, plus, star, slash, percent, hat,
    _and, _or, _not,
    gt, ge, ne, ee, le, lt,
    parentheses
};
struct operator_t {
    operator_type_t operator_type;
    explicit operator_t(operator_type_t t) : operator_type{t} {}
};
using symbol_reference_t = symbol_table_t::iterator;
using c_symbol_reference_t = symbol_table_t::const_iterator;
struct root_t {};
using underlying_syntax_node_t = std::variant<symbol_reference_t, c_symbol_reference_t, operator_t, root_t, symbol_value_t>;
struct syntax_node_t : public underlying_syntax_node_t {
    syntax_node_t() : underlying_syntax_node_t{root_t{}} {}
    template<typename T>
    syntax_node_t(const T& t) : underlying_syntax_node_t{t} {}
    syntax_node_t(symbol_reference_t r) : underlying_syntax_node_t{r} {}
    syntax_node_t(c_symbol_reference_t r) : underlying_syntax_node_t{r} {}
    template<typename T>
    auto& operator=(const T& t) {
        this->underlying_syntax_node_t::operator=(t);
        return *this;
    }
};
using syntax_tree_t = tree<syntax_node_t>;

auto operator<<(std::ostream& o, const operator_t& p) -> std::ostream&;
auto operator<<(std::ostream& o, const root_t& r) -> std::ostream&;
auto operator<<(std::ostream& o, const symbol_reference_t& r) -> std::ostream&;
auto operator<<(std::ostream& o, const c_symbol_reference_t& r) -> std::ostream&;
auto operator<<(std::ostream& o, const underlying_syntax_node_t& n) -> std::ostream&;
auto operator<<(std::ostream& o, const syntax_tree_t& t) -> std::ostream&;

#endif
