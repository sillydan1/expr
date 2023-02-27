%{
#include "expr-parser.hpp"
#include "expr-scanner.hpp"
#undef YY_DECL
#define YY_DECL int expr::scanner::yylex(expt::parser::semantic_type* const lval, expr::parser::location_type* loc)
using token = expr::parser::token;
#define yyterminate() return( token::END )
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->stop(); loc->columns(yyleng);
%}

%option c++ noyywrap nodefault
%option yyclass="expr::scanner"
%option outfile="expr-scanner.cpp"

/* ================================================== */
%%


%%
/* ================================================== */

