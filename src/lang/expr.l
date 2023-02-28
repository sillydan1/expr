%{
#include "expr-parser.hpp"
#include "expr-scanner.hpp"
#undef YY_DECL
#define YY_DECL int expr::scanner::yylex(expr::parser::semantic_type* const lval, expr::parser::location_type* loc)
using token = expr::parser::token;
#define yyterminate() return( token::END )
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option c++ noyywrap nodefault
%option yyclass="expr::scanner"
%option outfile="expr-scanner.cpp"

/* ================================================== */
id     [a-z_A-Z]([.\(\)a-zA-Z_0-9]*[a-zA-Z_0-9]+)?
int    [0-9]+[Ll]?
clk    [0-9]+(_ms)
flt    [0-9]+[.][0-9]+[fd]?
bool   [Ff]alse|[Tt]rue
str    \"(\\.|[^\\"])*\"
blank  [ \t\r]
accmod [Pp](ublic|rivate|rotected)
type   int|long|float|double|string|bool|clock|timer|var|auto

%%

{blank}+   /* nothing */
\n+        /* nothing */

"-"        return token::MINUS;
"+"        return token::PLUS;
"*"        return token::STAR;
"/"        return token::SLASH;
"%"        return token::PERCENT;
"^"        return token::HAT;
"&&"       return token::AND;
"||"       return token::OR;
"^^"       return token::XOR;
"=>"       return token::IMPLIES;
">"        return token::GT;
">="       return token::GE;
"=="       return token::EE;
"!="       return token::NE;
"<="       return token::LE;
"<"        return token::LT;
"!"        return token::NOT;
"("        return token::LPAREN;
")"        return token::RPAREN;
":="       return token::ASSIGN;
";"        return token::TERM;
{accmod}   return token::ACCESS_MOD;
{type}     return token::TYPE;
{int}      return token::NUMBER;
{flt}      return token::FLOAT;
{str}      return token::STRING;
{clk}      return token::CLOCK;
{bool}     return token::BOOL;
{id}       return token::IDENTIFIER;
<<EOF>>    return token::YYEOF;
.          throw expr::parser::syntax_error(*loc, "invalid character: " + std::string(YYText()));

%%
/* ================================================== */

