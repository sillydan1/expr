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
#include "operations/multiply.h"
#include "operations/util.h"
#include <sstream>
using namespace expr;

template<typename T1, typename T2>
auto t_modulo(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to modulo type " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<> auto t_modulo(const int& x, const int& y) {
    return x % y;
}
template<> auto t_modulo(const expr::clock_t& x, const int& y) {
    return (int)(x.time_units % y);
}
template<> auto t_modulo(const expr::clock_t& x, const expr::clock_t& y) {
    return expr::clock_t{x.time_units % y.time_units};
}
template<> auto t_modulo(const int& x, const expr::clock_t& y) {
    return expr::clock_t{x % y.time_units};
}

symbol_value_t modulo(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_modulo, b, res);
    return res;
}
symbol_value_t operator%(const symbol_value_t& a, const symbol_value_t& b) {
    return modulo(a,b);
}
