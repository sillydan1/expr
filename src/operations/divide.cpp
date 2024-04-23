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
#include "operations/divide.h"
#include "operations/util.h"
#include <cstdint>
#include <sstream>
using namespace expr;

template<typename T1, typename T2>
auto t_divide(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to divide type " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<> auto t_divide(const int& x, const int& y) {
    if(y == 0 || (x == INT32_MIN && y == -1))
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<> auto t_divide(const float& x, const int& y) {
    if(y == 0)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<> auto t_divide(const int& x, const float& y) {
    if(y == 0.0f)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<> auto t_divide(const float& x, const float& y) {
    if(y == 0.0f)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y;
}
template<> auto t_divide(const expr::clock_t& x, const expr::clock_t& y) {
    if(y.time_units == 0)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return expr::clock_t{x.time_units / y.time_units};
}
template<> auto t_divide(const int& x, const expr::clock_t& y) {
    if(y.time_units == 0)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return (int)(x / y.time_units);
}
template<> auto t_divide(const expr::clock_t& x, const int& y) {
    if(y == 0)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return expr::clock_t{x.time_units / y};
}
template<> auto t_divide(const float& x, const expr::clock_t& y) {
    if(y.time_units == 0)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x / y.time_units;
}
template<> auto t_divide(const expr::clock_t& x, const float& y) {
    if(y == 0.0f)
        throw std::domain_error("Cannot divide with zero or INT_MIN / -1");
    return x.time_units / y;
}

symbol_value_t divide(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_divide, b, res);
    return res;
}
symbol_value_t operator/(const symbol_value_t& a, const symbol_value_t& b) {
    return divide(a,b);
}
