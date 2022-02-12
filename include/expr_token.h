#ifndef EXPR_EXPR_TOKEN_H
#define EXPR_EXPR_TOKEN_H
#include <string>

struct expr_token_t {
    expr_token_t(const std::string& token);
};
expr_token_t& operator+(const expr_token_t&);
expr_token_t& operator-(const expr_token_t&);
expr_token_t& operator*(const expr_token_t&);
expr_token_t& operator/(const expr_token_t&);
expr_token_t& operator^(const expr_token_t&);

#endif //EXPR_EXPR_TOKEN_H
