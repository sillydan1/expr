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
#ifndef EXPR_BOOLEAN_H
#define EXPR_BOOLEAN_H
#include "symbol_table.h"
expr::symbol_value_t and_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t or_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t xor_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t not_(const expr::symbol_value_t& a);
expr::symbol_value_t gt_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t ge_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t ee_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t ne_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t le_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t lt_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t implies_(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
// TODO: These operators are ambiguous with symbol_value_t && std::false_type / std::true_type for some reason.
//symbol_value_t operator&&(const symbol_value_t& a, const symbol_value_t& b);
//symbol_value_t operator||(const symbol_value_t& a, const symbol_value_t& b);
//symbol_value_t operator!(const symbol_value_t& a);
#endif //EXPR_BOOLEAN_H
