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
#ifndef EXPR_CLOCK_H
#define EXPR_CLOCK_H
#include <ostream>

namespace expr {
    struct clock_t {
        unsigned int time_units = 0;
        void reset();
        void delay(unsigned int delta);
        void delay(long delta);
        void delay(int delta);
        auto operator+=(const unsigned int& delta) -> clock_t&;
        auto operator==(const clock_t& o) const -> bool;
        auto operator!=(const clock_t& o) const -> bool;
        auto operator=(const clock_t& o) -> clock_t&;
        auto operator=(const int& o) -> clock_t&;
    };
    auto stoclk(const char* str) -> clock_t;
    auto operator<<(std::ostream& o, const expr::clock_t& c) -> std::ostream&;
}
auto operator"" _ms(unsigned long long val) -> expr::clock_t;

#endif //EXPR_CLOCK_H

