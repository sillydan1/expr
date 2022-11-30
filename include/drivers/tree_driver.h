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
#ifndef EXPR_TREE_DRIVER_H
#define EXPR_TREE_DRIVER_H
#include "driver.h"

namespace expr {
    struct tree_driver : public driver {
        tree_driver(const symbol_table_tree_t::iterator& it) : driver{}, it{it} {}
        auto find(const std::string& identifier) const -> expr::symbol_table_t::const_iterator override {
            auto* x = &(*it);
            while(x) {
                auto i = x->node.find(identifier);
                if(i != x->node.end())
                    return i;
                if(!x->parent().has_value())
                    return end;
                x = x->parent().value();
            }
            return end;
        }
    private:
        symbol_table_tree_t::iterator it;
    };
}

#endif //EXPR_TREE_DRIVER_H
