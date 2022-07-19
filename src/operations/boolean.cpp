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
#include "operations/boolean.h"
#include "operations/util.h"
#include <sstream>
using namespace expr;

template<typename T1, typename T2>
auto t_and(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to AND types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_or(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to OR types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_xor(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to XOR types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1>
auto t_not(const T1&) {
    std::ostringstream ss{};
    ss << "Unable to NOT type " << typeid(T1).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_gt(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to compare (>) types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_ge(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to compare (>=) types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_ee(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to compare (==) types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_ne(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to compare (!=) types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_le(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to compare (<=) types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}
template<typename T1, typename T2>
auto t_lt(const T1&, const T2&) {
    std::ostringstream ss{};
    ss << "Unable to compare (<) types " << typeid(T1).name() << " and " << typeid(T2).name();
    throw std::domain_error(ss.str());
    return nullptr; // Must return something
}

template<>
auto t_and(const bool& a, const bool& b) {
    return a && b;
}
template<>
auto t_or(const bool& a, const bool& b) {
    return a || b;
}
template<>
auto t_xor(const bool& a, const bool& b) {
    return a != b;
}
template<>
auto t_not(const bool& a) {
    return !a;
}

auto t_gt(const int& a, const int& b) {return a > b;}
auto t_gt(const int& a, const float& b) {return a > b;}
auto t_gt(const float& a, const int& b) {return a > b;}
auto t_gt(const float& a, const float& b) {return a > b;}

auto t_ge(const int& a, const int& b) {return a >= b;}
auto t_ge(const int& a, const float& b) {return a >= b;}
auto t_ge(const float& a, const int& b) {return a >= b;}
auto t_ge(const float& a, const float& b) {return a >= b;}

auto t_ee(const bool& a, const bool& b) {return a == b;}
auto t_ee(const int& a, const int& b) {return a == b;}
auto t_ee(const int& a, const float& b) {return a == b;}
auto t_ee(const float& a, const int& b) {return a == b;}
auto t_ee(const float& a, const float& b) {return a == b;}
auto t_ee(const std::string& a, const std::string& b) {return a == b;}

auto t_ne(const bool& a, const bool& b) {return a != b;}
auto t_ne(const int& a, const int& b) {return a != b;}
auto t_ne(const int& a, const float& b) {return a != b;}
auto t_ne(const float& a, const int& b) {return a != b;}
auto t_ne(const float& a, const float& b) {return a != b;}
auto t_ne(const std::string& a, const std::string& b) {return a != b;}

auto t_lt(const int& a, const int& b) {return a < b;}
auto t_lt(const int& a, const float& b) {return a < b;}
auto t_lt(const float& a, const int& b) {return a < b;}
auto t_lt(const float& a, const float& b) {return a < b;}

auto t_le(const int& a, const int& b) {return a <= b;}
auto t_le(const int& a, const float& b) {return a <= b;}
auto t_le(const float& a, const int& b) {return a <= b;}
auto t_le(const float& a, const float& b) {return a <= b;}

symbol_value_t and_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_and, b, res);
    return res;
}
symbol_value_t operator&&(const symbol_value_t& a, const symbol_value_t& b) {
    return and_(a,b);
}
symbol_value_t or_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_or, b, res);
    return res;
}
symbol_value_t operator||(const symbol_value_t& a, const symbol_value_t& b) {
    return and_(a,b);
}
symbol_value_t xor_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_xor, b, res);
    return res;
}
symbol_value_t not_(const symbol_value_t& a) {
    symbol_value_t res{};
    MONOFUNC_IMPL(a, t_not, res);
    return res;
}
symbol_value_t operator!(const symbol_value_t& a) {
    return not_(a);
}
symbol_value_t gt_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_gt, b, res);
    return res;
}
symbol_value_t ge_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_ge, b, res);
    return res;
}
symbol_value_t ee_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_ee, b, res);
    return res;
}
symbol_value_t ne_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_ne, b, res);
    return res;
}
symbol_value_t le_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_le, b, res);
    return res;
}
symbol_value_t lt_(const symbol_value_t& a, const symbol_value_t& b) {
    symbol_value_t res{};
    FUNC_IMPL(a, t_lt, b, res);
    return res;
}
