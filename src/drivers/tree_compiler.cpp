#include <drivers/tree_compiler.h>

namespace expr {
    int tree_compiler::parse(const std::string& f) {
        if (f.empty()) {
#ifdef DEFAULT_EXPRESSION_VALUE
            if(!std::string(DEFAULT_EXPRESSION_VALUE).empty())
                return parse(DEFAULT_EXPRESSION_VALUE);
#endif
            error = "empty expression and no DEFAULT_EXPRESSION_VALUE";
            return 1;
        }
        file = f;
        location.initialize(&file);
        scan_begin();
        yy::parser parse(this);
        parse.set_debug_level(trace_parsing);
        try {
            int res = parse();
            scan_end();
            return res;
        } catch (std::exception &e) {
            error = e.what();
            return 1;
        }
    }
    auto tree_compiler::get_symbol(const std::string& identifier) -> syntax_tree_t {
        if (!contains(identifier))
            throw std::out_of_range(identifier + " not found");
        return syntax_tree_t{identifier_t{identifier}};
    }
    void tree_compiler::add_tree(const syntax_tree_t& tree) {
        trees["expression_result"] = (tree);
    }
    void tree_compiler::add_tree(const std::string& identifier, const syntax_tree_t& tree) {
        trees[identifier] = tree;
    }
    void tree_compiler::add_tree(const std::string& access_modifier, const std::string& identifier, const expr::syntax_tree_t& tree) {
        add_tree(identifier, tree);
    }
}
