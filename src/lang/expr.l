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

{blank}+   loc.step();
\n+        { loc.lines(yyleng); loc.step(); }

"-"        return token::MINUS(loc);
"+"        return token::PLUS(loc);
"*"        return token::STAR(loc);
"/"        return token::SLASH(loc);
"%"        return token::PERCENT(loc);
"^"        return token::HAT(loc);
"&&"       return token::AND(loc);
"||"       return token::OR(loc);
"^^"       return token::XOR(loc);
"=>"       return token::IMPLIES(loc);
">"        return token::GT(loc);
">="       return token::GE(loc);
"=="       return token::EE(loc);
"!="       return token::NE(loc);
"<="       return token::LE(loc);
"<"        return token::LT(loc);
"!"        return token::NOT(loc);
"("        return token::LPAREN(loc);
")"        return token::RPAREN(loc);
":="       return token::ASSIGN(loc);
";"        return token::TERM(loc);
{accmod}   return token::ACCESS_MODIFIER(yytext, loc);
{type}     return token::TYPE(loc);
{int}      return token::NUMBER(yytext, loc);
{flt}      return token::FLOAT(yytext, loc);
{str}      return token::STRING(yytext, loc);
{clk}      return token::CLOCK(yytext, loc);
{bool}     return token::BOOL(yytext, loc);
{id}       return token::IDENTIFIER(yytext, loc);
<<EOF>>    return token::YYEOF(loc);
.          throw expr::parser::syntax_error(loc, "invalid character: " + std::string(yytext));

%%
/* ================================================== */

