/* MIT License
 *
 * Copyright (c) 2022 Asger Gitz-Johansen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef EXPR_SYMBOL_TABLE_H
#define EXPR_SYMBOL_TABLE_H
#include <utility>
#include <variant>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>
#include <tree>
#include <hashcombine>
#include <overload>
#include <optional>
#include "clock.h"

namespace expr {
    using underlying_symbol_value_t = std::variant<int, float, bool, std::string, clock_t>;
    struct symbol_value_t : public underlying_symbol_value_t {
        symbol_value_t() = default;

        template<typename T> 
        symbol_value_t(const T &x) : underlying_symbol_value_t{x} {}

        template<typename T>
        auto &operator=(const T &t) {
            this->underlying_symbol_value_t::operator=(t);
            return *this;
        }

        auto interpret(const std::string &s) -> symbol_value_t& {
            std::stringstream ss{s};
            int i;
            float f;
            bool b;
            if (ss >> i)
                this->underlying_symbol_value_t::operator=(i);
            else if (ss >> f)
                this->underlying_symbol_value_t::operator=(f);
            else if (ss >> b)
                this->underlying_symbol_value_t::operator=(b);
            else
                this->underlying_symbol_value_t::operator=(s);
            return *this;
        }

        symbol_value_t &operator<<=(const std::string &s) {
            return interpret(s);
        }
    };

    using underlying_symbol_table_t = std::map<std::string, symbol_value_t>;
    struct symbol_table_t : public underlying_symbol_table_t {
        auto operator+=(const symbol_table_t &) -> symbol_table_t&;
        auto operator*=(const symbol_table_t &) -> symbol_table_t&;
        auto get(const std::string& key) const -> const symbol_value_t&;
        auto get(const std::string& key) -> symbol_value_t&;
        auto put(const symbol_table_t &) -> symbol_table_t&;
        auto overwrite_elements(const symbol_table_t &) -> symbol_table_t&;
        auto is_overlapping(const symbol_table_t& other) -> bool;
        auto is_overlapping_and_not_idempotent(const symbol_table_t& other) -> bool;
        auto is_completely_overlapping(const symbol_table_t& other) -> bool;
        void delay();
        void delay_but_dont_reset_amount();
        void delay(const expr::symbol_value_t& time_units);
        void set_delay_amount(const expr::symbol_value_t& time_units);
        auto get_delay_amount() const -> std::optional<expr::symbol_value_t>;
    private:
        std::optional<expr::symbol_value_t> delay_amount{};
    };

    // TODO: operator+/* should be slightly different here
    using symbol_table_tree_t = ya::tree<symbol_table_t>;

    auto operator+(const symbol_table_t &a, const symbol_table_t &b) -> symbol_table_t;
    auto operator<<(std::ostream &os, const symbol_value_t &v) -> std::ostream&;
    auto operator<<(std::ostream &os, const symbol_table_t &m) -> std::ostream&;
    auto operator<<(std::ostream &os, const symbol_table_tree_t &t) -> std::ostream&;
    auto as_string(const symbol_value_t& v) -> std::string;

    enum class operator_type_t {
        minus, plus, star, slash, percent, hat,
        _and, _or, _xor, _not, _implies,
        gt, ge, ne, ee, le, lt,
        parentheses
    };

    struct operator_t {
        operator_type_t operator_type;
        explicit operator_t(operator_type_t t) : operator_type{t} {}
    };

    struct identifier_t {
        // TODO: For better lookup performance, save the hashes of identifiers instead (with a #ifndef NDEBUG option so you can debug the string)
        std::string ident;
    };
    struct root_t {};

    using underlying_syntax_node_t = std::variant<identifier_t, operator_t, root_t, symbol_value_t>;
    struct syntax_node_t : public underlying_syntax_node_t {
        syntax_node_t() : underlying_syntax_node_t{root_t{}} {}

        template<typename T>
        syntax_node_t(const T &t) : underlying_syntax_node_t{t} {}

        template<typename T>
        auto &operator=(const T &t) {
            this->underlying_syntax_node_t::operator=(t);
            return *this;
        }
    };

    using syntax_tree_t = ya::tree<syntax_node_t>;

    auto operator<<(std::ostream &o, const operator_t &p) -> std::ostream &;
    auto operator<<(std::ostream &o, const root_t &r) -> std::ostream &;
    auto operator<<(std::ostream &o, const identifier_t &r) -> std::ostream &;
    auto operator<<(std::ostream &o, const underlying_syntax_node_t &n) -> std::ostream &;
    auto operator<<(std::ostream &o, const syntax_tree_t &t) -> std::ostream &;

    auto stob(const char *s) -> bool;
}

namespace std {
    template<> struct hash<expr::symbol_value_t> {
        auto operator()(const expr::symbol_value_t& v) const -> size_t;
    };
    template<> struct hash<expr::symbol_table_t> {
        auto operator()(const expr::symbol_table_t& v) const -> size_t;
    };
}

// old defines - use the ast_factory instead
//#ifndef BINOP_CTOR
//#define BINOP_CTOR(op,arg1,arg2) expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::op}}.concat(arg1).concat(arg2)
//#endif
//#ifndef IDENT_CTOR
//#define IDENT_CTOR(arg1) drv->get_symbol(arg1);
//#endif
//#ifndef MONOOP_CTOR
//#define MONOOP_CTOR(op,arg1) expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::op}}.concat(arg1)
//#endif
//#ifndef LIT_CTOR
//#define LIT_CTOR(arg1) expr::syntax_tree_t{arg1}
//#endif

#endif

