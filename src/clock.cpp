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
#include "clock.h"
#include <iostream>

namespace expr {
    void clock_t::reset() {
        time_units = 0;
    }
    void clock_t::delay(unsigned int delta) {
        time_units += delta;
    }
    auto clock_t::operator+=(const unsigned int& delta) -> clock_t& {
        delay(delta);
        return *this;
    }
    auto clock_t::operator==(const clock_t& o) const -> bool {
        return time_units == o.time_units;
    }
    auto clock_t::operator!=(const clock_t& o) const -> bool {
        return !(*this == o);
    }

    auto operator"" _ms(unsigned long long val) -> clock_t {
        clock_t v{}; v.time_units = val;
        return v;
    }

    auto operator<<(std::ostream& o, const clock_t& c) -> std::ostream& {
        return o << c.time_units;
    }

    auto stoclk(const char* str) -> clock_t {
        std::string s{str};
        auto loc = s.find( "_ms", 0 );
        if(loc == std::string::npos)
            throw std::invalid_argument("not a clock_t value "+s);
        auto i = std::stoi(s.substr(0, loc));
        clock_t c{};
        c.time_units = i;
        return c;
    }
}
