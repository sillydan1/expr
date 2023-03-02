#include "ast-factory.h"
#include "symbol_table.h"
#include <cstdlib>

namespace expr {
    auto ast_factory::build_operator(const operator_type_t& op, const syntax_tree_t& child) -> syntax_tree_t {
        return build_operator(op, {child});
    }

    auto ast_factory::build_operator(const operator_type_t& op, const syntax_tree_t& child1, const syntax_tree_t& child2) -> syntax_tree_t {
        return build_operator(op, {child1, child2});
    }

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
   
    auto ast_factory::build_declaration(const std::string &identifier, const syntax_tree_t &tree, const symbol_access_modifier_t& access_modifier) -> syntax_tree_t {
        return build_declaration(identifier, tree, symbol_type_name_t::_auto, access_modifier);
    }

    auto ast_factory::build_declaration(const std::string& identifier, const syntax_tree_t& tree, const symbol_type_name_t& type_name, const symbol_access_modifier_t& access_modifier) -> syntax_tree_t {
        // TODO: add the thing to the thing
        std::cout << identifier << " " << type_name << " " << access_modifier << ": " << tree << std::endl;
        return tree;
    }
}

