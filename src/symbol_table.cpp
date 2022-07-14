#include "symbol_table.h"
#include <overload>

namespace expr {
    symbol_table_t &symbol_table_t::operator+=(const symbol_table_t &other) {
        for (auto &e: other)
            this->insert_or_assign(e.first, e.second);
        return *this;
    }

    auto symbol_table_t::is_overlapping(const symbol_table_t &other) -> bool {
        return std::any_of(other.begin(), other.end(), [this](const auto& value){ return this->find(value.first) != this->end(); });
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

    auto operator<<(std::ostream &o, const root_t &r) -> std::ostream & { return o << "ROOT"; }

    auto operator<<(std::ostream &o, const symbol_reference_t &r) -> std::ostream & {
        return o << r->first << " :-> " << r->second;
    }

    auto operator<<(std::ostream &o, const c_symbol_reference_t &r) -> std::ostream & {
        return o << r->first << " :-> " << r->second;
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
