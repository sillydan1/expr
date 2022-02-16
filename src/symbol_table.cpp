#include <string>
#include <functional>
#include "symbol_table.h"

struct symbol_value_printer_t {
    std::string operator()(const int& i) const { return std::to_string(i); }
    std::string operator()(const float& f) const { return std::to_string(f); }
};
struct symbol_value_operator_t {
    symbol_value_t lhs;
    modifier_t<int> int_action;
    modifier_t<float> float_action;
    symbol_value_operator_t() = delete;
    symbol_value_t operator()(const int& i) const { return int_action(std::get<int>(lhs), i); }
    symbol_value_t operator()(const float& i) const { return float_action(std::get<float>(lhs), i); }
};

std::string to_string(const symbol_value_t& s) {
    return std::visit(symbol_value_printer_t{}, s);
}
symbol_value_t add(const symbol_value_t& a, const symbol_value_t& b) {
    return std::visit(symbol_value_operator_t{
            .lhs=a,
            .int_action = std::function<int(int,int)>(std::plus<>()),
            .float_action = std::function<float(float,float)>(std::plus<>()),
    }, b);
}
symbol_value_t operator+(const symbol_value_t& a, const symbol_value_t& b) {return add(a,b);}
symbol_value_t subtract(const symbol_value_t& a, const symbol_value_t& b) {
    return std::visit(symbol_value_operator_t{
            .lhs=a,
            .int_action = std::function<int(int,int)>(std::minus<>()),
            .float_action = std::function<float(float,float)>(std::minus<>()),
    }, b);
}
symbol_value_t operator-(const symbol_value_t& a, const symbol_value_t& b) {return subtract(a,b);}
symbol_value_t multiply(const symbol_value_t& a, const symbol_value_t& b) {
    return std::visit(symbol_value_operator_t{
            .lhs=a,
            .int_action = std::function<int(int,int)>(std::multiplies<>()),
            .float_action = std::function<float(float,float)>(std::multiplies<>()),
    }, b);
}
symbol_value_t operator*(const symbol_value_t& a, const symbol_value_t& b) {return multiply(a,b);}
symbol_value_t divide(const symbol_value_t& a, const symbol_value_t& b) {
    return std::visit(symbol_value_operator_t{
            .lhs=a,
            .int_action = std::function<int(int,int)>(std::divides<>()),
            .float_action = std::function<float(float,float)>(std::divides<>()),
    }, b);
}
symbol_value_t operator/(const symbol_value_t& a, const symbol_value_t& b) {return divide(a,b);}
