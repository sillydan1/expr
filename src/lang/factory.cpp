#include "factory.h"
#include "symbol_table.h"
#include <cstdlib>

namespace expr {
    auto ast_factory::build_operator(const operator_type_t& op, const std::vector<syntax_tree_t>& children) -> syntax_tree_t {
        syntax_tree_t result{operator_t{op}};
        for(auto& child : children)
            result.concat(child);
        return result;
    }

    auto ast_factory::build_literal(const symbol_value_t& value) -> syntax_tree_t {
        return syntax_tree_t{value};
    }

    auto ast_factory::build_identifier(const std::string& identifier) -> syntax_tree_t {
        return syntax_tree_t{identifier_t{identifier}};
    }

    auto ast_factory::build_root(const syntax_tree_t& child) -> syntax_tree_t {
        return syntax_tree_t{}.concat(child);
    }

    auto ast_factory::build_declaration(const std::string& identifier, const syntax_tree_t& tree) -> syntax_tree_t {
        // TODO: Add identifier to thingy
        std::cout << "default" << " " << identifier << tree << "\n";
        return tree;
    }

    auto ast_factory::build_declaration(const std::string& identifier, const std::string& access_modifier, const syntax_tree_t& tree) -> syntax_tree_t {
        // TODO: add identifier to thingy
        std::cout << access_modifier << " " << identifier << tree << "\n";
        return tree;
    }
}

