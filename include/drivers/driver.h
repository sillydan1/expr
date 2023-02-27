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
#ifndef EXPR_DRIVER_H
#define EXPR_DRIVER_H
#include <string>
#include "symbol_table.h"
#include "parser.hpp"
#define YY_DECL yy::parser::symbol_type yylex (expr::driver* drv)
YY_DECL;

namespace expr {
    // TODO: Break this into an interface (functions) and a base abstract class (basic implementation)
    using symbol_table_ref_t = std::reference_wrapper<const expr::symbol_table_t>;
    using symbol_table_ref_collection_t = std::vector<std::reference_wrapper<const expr::symbol_table_t>>;
    struct driver {
        driver(std::initializer_list<symbol_table_ref_t> environments)
         : trace_parsing(false), trace_scanning(false), environments{environments}, buffer{} {}
        virtual ~driver() = default;

        virtual int parse(const std::string &f) = 0;
        virtual auto get_symbol(const std::string &identifier) -> syntax_tree_t = 0;
        virtual void add_tree(const syntax_tree_t& tree) = 0;
        virtual void add_tree(const std::string& identifier, const syntax_tree_t& tree) = 0;
        virtual void add_tree(const std::string& access_modifier, const std::string& identifier, const syntax_tree_t& tree) = 0;
        virtual auto contains(const std::string& identifier) const -> bool {
            return find(identifier) != end;
        }
        virtual auto find(const std::string& identifier) const -> expr::symbol_table_t::const_iterator {
            // TODO: Tree of environments
            for(auto& env : environments) {
                auto env_it = env.get().find(identifier);
                if(env_it != env.get().end())
                    return env_it;
            }
            return end;
        }

        void scan_begin();
        void scan_end();

        std::string error;
        std::string file;
        bool trace_parsing;
        bool trace_scanning;
        yy::location location;
    protected:
        expr::symbol_table_t::const_iterator end{};
        symbol_table_ref_collection_t environments;
        yy_buffer_state* buffer;
    };
}

#endif //EXPR_DRIVER_H
