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
#include <overload>

namespace expr {
    auto symbol_table_t::operator+=(const symbol_table_t &other) -> symbol_table_t& {
        return put(other);
    }

    auto symbol_table_t::put(const symbol_table_t &other) -> symbol_table_t & {
        for (auto &e: other)
            this->insert_or_assign(e.first, e.second);
        return *this;
    }

    auto symbol_table_t::operator*=(const symbol_table_t &other) -> symbol_table_t & {
        return overwrite_elements(other);
    }

    auto symbol_table_t::overwrite_elements(const symbol_table_t &other) -> symbol_table_t & {
        for (auto &e: other)
            if(contains(e.first))
                this->insert_or_assign(e.first, e.second);
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

    void symbol_table_t::delay(unsigned int time_units) {
        for(auto& e : *this)
            std::visit(ya::overload(
                    [&time_units](clock_t& v){ v.delay(time_units); },
                    [](auto&&){}), e.second);
    }

    symbol_table_t operator+(const symbol_table_t &a, const symbol_table_t &b) {
        symbol_table_t r{};
        r += a;
        r += b;
        return r;
    }

    std::ostream &operator<<(std::ostream &os, const symbol_value_t &v) {
        std::visit(ya::overload{
                           [&os](const bool &b) { os << std::boolalpha << b << " " << typeid(b).name(); },
                           [&os](const std::string &s) { os << "\"" << s << "\" s"; },
                           [&os](const expr::clock_t &s) { os << "\"" << s << "\" c"; },
                           [&os](auto &&v) { os << v << " " << typeid(v).name(); }},
                   static_cast<const underlying_symbol_value_t &>(v));
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const symbol_table_t &m) {
        for (auto &v: m)
            os << v.first << " :-> " << v.second << "\n";
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

    auto operator<<(std::ostream &o, const underlying_syntax_node_t &n) -> std::ostream & {
        std::visit(ya::overload(
                [&o](auto &&x) { o << x; }
        ), n);
        return o;
    }

    auto operator<<(std::ostream &o, const syntax_tree_t &tree) -> std::ostream & {
        if (tree.children.empty())
            return o << tree.node << " ";
        o << tree.node;
        o << "[";
        for (auto &c: tree.children)
            o << c;
        return o << "]";
    }
}

auto std::hash<expr::symbol_value_t>::operator()(const expr::symbol_value_t& v) const -> size_t {
    size_t result{};
    std::visit(ya::overload(
            [&result](const int& v){result = std::hash<int>{}(v);},
            [&result](const float& v){result = std::hash<float>{}(v);},
            [&result](const bool& v){result = std::hash<bool>{}(v);},
            [&result](const std::string& v){result = std::hash<std::string>{}(v);},
            [&result](const expr::clock_t& c){result = std::hash<unsigned int>{}(c.time_units);}
    ), static_cast<const expr::underlying_symbol_value_t&>(v));
    return result;
}

auto std::hash<expr::symbol_table_t>::operator()(const expr::symbol_table_t& v) const -> size_t {
    size_t result{};
    for(auto& symbol : v) {
        result = ya::hash_combine(result, symbol.first);
        result = ya::hash_combine(result, symbol.second);
    }
    return result;
}
