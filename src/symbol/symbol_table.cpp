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
#include "symbol_table.h"
#include "extensions.h"
#include <overload>
#include <stdexcept>
#include <algorithm>

namespace expr {
    auto symbol_table_t::operator+=(const symbol_table_t &other) -> symbol_table_t& {
        return put(other);
    }

    auto symbol_table_t::get(const std::string& key) -> symbol_value_t& {
        return this->operator[](key);
    }

    auto symbol_table_t::get(const std::string& key) const -> const symbol_value_t& {
        return this->at(key);
    }

    auto symbol_table_t::put(const symbol_table_t &other) -> symbol_table_t & {
        for (auto &e: other)
            this->insert_or_assign(e.first, e.second);
        if(other.delay_amount.has_value())
            delay(other.delay_amount.value());
        return *this;
    }

    auto symbol_table_t::operator*=(const symbol_table_t &other) -> symbol_table_t & {
        return overwrite_elements(other);
    }

    auto symbol_table_t::overwrite_elements(const symbol_table_t &other) -> symbol_table_t & {
        for (auto &e: other)
            if(contains(e.first))
                this->insert_or_assign(e.first, e.second);
        if(other.delay_amount.has_value())
            delay(other.delay_amount.value());
        return *this;
    }

    auto symbol_table_t::is_completely_overlapping(const symbol_table_t &other) -> bool {
        return std::all_of(other.begin(), other.end(), [this](const auto& value){ return this->find(value.first) != this->end(); });
    }

    auto symbol_table_t::is_overlapping(const symbol_table_t &other) -> bool {
        return std::any_of(other.begin(), other.end(), [this](const auto& value){ return this->find(value.first) != this->end(); });
    }

    auto symbol_table_t::is_overlapping_and_not_idempotent(const symbol_table_t &other) -> bool {
        auto comparator = [this](const auto& value){
            if(this->find(value.first) != this->end())
                return this->operator[](value.first) != value.second;
            return false;
        };
        return std::any_of(other.begin(), other.end(), comparator);
    }

    void symbol_table_t::delay() {
        delay_but_dont_reset_amount();
        delay_amount = {};
    }

    void symbol_table_t::delay_but_dont_reset_amount() {
        if(delay_amount.has_value())
            delay(delay_amount.value());
    }

    void symbol_table_t::delay(const expr::symbol_value_t& time_units) {
        auto tu = std::visit(ya::overload(
                [](const int& i) -> long{ return i; },
                [](const clock_t& c) -> long { return c.time_units; },
                [](auto&& v) -> long{ throw std::logic_error(std::string{"cannot delay with a symbol_value of type: "} + typeid(v).name()); }
        ), static_cast<const underlying_symbol_value_t&>(time_units));
        for(auto& e : *this)
            std::visit(ya::overload(
                    [&tu](clock_t& v){ v.delay(tu); },
                    [](auto&&){ /* ignored - don't delay non-clocks */ }),
                       static_cast<underlying_symbol_value_t&>(e.second));
    }

    void symbol_table_t::set_delay_amount(const expr::symbol_value_t& time_units) {
        delay_amount = {time_units};
    }

    auto symbol_table_t::get_delay_amount() const -> std::optional<expr::symbol_value_t> {
        return delay_amount;
    }

    symbol_table_t operator+(const symbol_table_t &a, const symbol_table_t &b) {
        symbol_table_t r{};
        r += a;
        r += b;
        return r;
    }

    auto operator<<(std::ostream &os, const symbol_value_t &v) -> std::ostream& {
        return std::visit(ya::overload{
                           [&os](const bool &b) -> std::ostream& { return os << std::boolalpha << b << " " << typeid(b).name(); },
                           [&os](const std::string &s) -> std::ostream& { return os << "\"" << s << "\" s"; },
                           [&os](const expr::clock_t &s) -> std::ostream& { return os << "" << s << " c"; },
                           [&os](auto &&v) -> std::ostream& { return os << v << " " << typeid(v).name(); }
                           }, static_cast<const underlying_symbol_value_t &>(v));
    }

    auto operator<<(std::ostream &os, const symbol_table_t &m) -> std::ostream& {
        for (auto &v: m)
            os << v.first << " :-> " << v.second << "\n";
        if(m.get_delay_amount().has_value())
            os << "delay_amount :-> " << m.get_delay_amount().value();
        return os;
    }

    auto operator<<(std::ostream &o, const operator_t &p) -> std::ostream & {
        switch (p.operator_type) {
            case operator_type_t::minus:
                return o << "-";
            case operator_type_t::plus:
                return o << "+";
            case operator_type_t::star:
                return o << "*";
            case operator_type_t::slash:
                return o << "/";
            case operator_type_t::percent:
                return o << "%";
            case operator_type_t::hat:
                return o << "^";
            case operator_type_t::_and:
                return o << "&&";
            case operator_type_t::_or:
                return o << "||";
            case operator_type_t::_xor:
                return o << "xor";
            case operator_type_t::_implies:
                return o << "=>";
            case operator_type_t::_not:
                return o << "!";
            case operator_type_t::gt:
                return o << ">";
            case operator_type_t::ge:
                return o << ">=";
            case operator_type_t::ne:
                return o << "!=";
            case operator_type_t::ee:
                return o << "==";
            case operator_type_t::le:
                return o << "<=";
            case operator_type_t::lt:
                return o << "<";
            case operator_type_t::parentheses:
                return o << "()";
            default:
                return o << "unknown";
        }
    }

    auto operator<<(std::ostream &o, const root_t &r) -> std::ostream & {
        return o << "ROOT";
    }

    auto operator<<(std::ostream& o, const identifier_t& r) -> std::ostream& {
        return o << r.ident;
    }

    auto operator<<(std::ostream& o, const underlying_syntax_node_t& n) -> std::ostream& {
        return std::visit(ya::overload([&o](auto &&x) -> std::ostream& { return o << x; }), n);
    }

    auto operator<<(std::ostream& o, const syntax_tree_t& tree) -> std::ostream& {
        if (tree.children().empty())
            return o << tree.node << " ";
        o << tree.node;
        o << "[";
        for (auto &c: tree.children())
            o << c;
        return o << "]";
    }

    auto operator<<(std::ostream& o, const symbol_table_tree_t& t) -> std::ostream& {
        auto printer_func = [&o](const symbol_table_t& x){
            o << "\t<node>\n" << x;
        };
        t.apply_dfs(printer_func);
        return o;
    }

    auto as_string(const symbol_value_t& v) -> std::string {
        return std::visit(ya::overload{
                [](const std::string &s) -> std::string { return s; },
                [](const expr::clock_t &s) -> std::string { return std::to_string(s.time_units); },
                [](auto &&v) -> std::string { return std::to_string(v); }
        }, static_cast<const underlying_symbol_value_t &>(v));
    }

    auto stob(const char *s) -> bool {
        auto cpy = std::string{s};
        std::transform(cpy.begin(), cpy.end(), cpy.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        bool b;
        std::istringstream(s) >> std::boolalpha >> b;
        return b;
    }
    
    auto stotypename(const std::string& s) -> symbol_type_name_t {
        switch(hash_djb2a(s)) {
            case "auto"_sh: return symbol_type_name_t::_auto;
            case "var"_sh: return symbol_type_name_t::_auto;
            case "int"_sh: return symbol_type_name_t::_int;
            case "long"_sh: return symbol_type_name_t::_int;
            case "float"_sh: return symbol_type_name_t::_float;
            case "double"_sh: return symbol_type_name_t::_float;
            case "string"_sh: return symbol_type_name_t::_string;
            case "bool"_sh: return symbol_type_name_t::_bool;
            case "clock"_sh: return symbol_type_name_t::_clock;
            case "timer"_sh: return symbol_type_name_t::_clock;
            default: throw std::logic_error(std::string{s+" is not a valid type name"}.c_str());
        }
    }

    auto stoaccmod(const std::string& s) -> symbol_access_modifier_t {
        switch(hash_djb2a(s)) {
            case "public"_sh: return symbol_access_modifier_t::_public;
            case "Public"_sh: return symbol_access_modifier_t::_public;
            case "private"_sh: return symbol_access_modifier_t::_private;
            case "Private"_sh: return symbol_access_modifier_t::_private;
            case "protected"_sh: return symbol_access_modifier_t::_protected;
            case "Protected"_sh: return symbol_access_modifier_t::_protected;
            default: throw std::logic_error(std::string{s+" is not a valid access modifier"}.c_str());
        }
    }

    std::ostream& operator<<(std::ostream& out, const symbol_type_name_t& value){
        const char* s = "invalid";
#define PROCESS_VAL(p) case(p): s = #p; break;
        switch(value) {
            PROCESS_VAL(symbol_type_name_t::_int);
            PROCESS_VAL(symbol_type_name_t::_long);
            PROCESS_VAL(symbol_type_name_t::_float);
            PROCESS_VAL(symbol_type_name_t::_double);
            PROCESS_VAL(symbol_type_name_t::_string);
            PROCESS_VAL(symbol_type_name_t::_bool);
            PROCESS_VAL(symbol_type_name_t::_clock);
            PROCESS_VAL(symbol_type_name_t::_auto);
    }
#undef PROCESS_VAL
        return out << s;
    }
    
    std::ostream& operator<<(std::ostream& out, const symbol_access_modifier_t& value){
        const char* s = "invalid";
#define PROCESS_VAL(p) case(p): s = #p; break;
        switch(value) {
            PROCESS_VAL(symbol_access_modifier_t::_private);
            PROCESS_VAL(symbol_access_modifier_t::_public);
            PROCESS_VAL(symbol_access_modifier_t::_protected);
        }
#undef PROCESS_VAL

        return out << s;
    }
}

auto std::hash<expr::symbol_value_t>::operator()(const expr::symbol_value_t& v) const -> size_t {
    return std::visit(ya::overload(
            [](const int& v){ return std::hash<int>{}(v); },
            [](const float& v){ return std::hash<float>{}(v); },
            [](const bool& v){ return std::hash<bool>{}(v); },
            [](const std::string& v){ return std::hash<std::string>{}(v); },
            [](const expr::clock_t& c){ return std::hash<unsigned int>{}(c.time_units); }
    ), static_cast<const expr::underlying_symbol_value_t&>(v));
}

auto std::hash<expr::symbol_table_t>::operator()(const expr::symbol_table_t& v) const -> size_t {
    size_t result{};
    for(auto& symbol : v) {
        result = ya::hash_combine(result, symbol.first);
        result = ya::hash_combine(result, symbol.second);
    }
    if(v.get_delay_amount().has_value())
        result = ya::hash_combine(result, v.get_delay_amount().value());
    return result;
}
