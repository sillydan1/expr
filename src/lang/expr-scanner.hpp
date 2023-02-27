#ifndef EXPR_SCANNER_HPP
#define EXPR_SCANNER_HPP
#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "expr-parser.hpp"
#include "location.hh"

namespace expr {
    class scanner : public yyFlexLexer {
    public:
        scanner(std::istream& arg_yyin, std::ostream& arg_yyout) : yyFlexLexer{arg_yyin, arg_yyout} {}
        scanner(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr) : yyFlexLexer{arg_yyin, arg_yyout} {}
        virtual ~scanner() = default;
        virtual int yylex(parser::semantic_type* const lval, parser::location_type* location);
    private:
        parser::semantic_type* yylval = nullptr;    
    };
}

#endif

