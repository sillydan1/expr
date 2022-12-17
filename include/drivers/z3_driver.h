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
#ifndef EXPR_Z3_DRIVER_H
#ifdef ENABLE_Z3
#define EXPR_Z3_DRIVER_H
#include "operations.h"
#include "drivers/driver.h"
#include <memory>

namespace expr {
    struct z3_driver : public driver {
        z3_driver(const symbol_table_t& known_env, const symbol_table_t& unknown_env);
        ~z3_driver() override;

        auto parse(const std::string &f) -> int override;
        auto get_symbol(const std::string& identifier) -> syntax_tree_t override;
        void add_tree(const syntax_tree_t& tree) override;
        void add_tree(const std::string& identifier, const syntax_tree_t& tree) override;
        void add_tree(const std::string& access_modifier, const std::string& identifier, const syntax_tree_t& tree) override;

        symbol_table_t result{};
    protected:
        struct impl;
        std::unique_ptr<impl> pimpl;
        std::string delay_identifier;
        const symbol_table_t& known;
        const symbol_table_t& unknown;
        void solve();
    };
}

#endif //ENABLE_Z3
#endif //EXPR_Z3_DRIVER_H
