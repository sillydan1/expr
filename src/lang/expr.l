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

"-"        { return token::MINUS; }
"+"        { return token::PLUS; }
"*"        { return token::STAR; }
"/"        { return token::SLASH; }
"%"        { return token::PERCENT; }
"^"        { return token::HAT; }
"&&"       { return token::AND; }
"||"       { return token::OR; }
"^^"       { return token::XOR; }
"=>"       { return token::IMPLIES; }
">"        { return token::GT; }
">="       { return token::GE; }
"=="       { return token::EE; }
"!="       { return token::NE; }
"<="       { return token::LE; }
"<"        { return token::LT; }
"!"        { return token::NOT; }
"("        { return token::LPAREN; }
")"        { return token::RPAREN; }
":="       { return token::ASSIGN; }
";"        { return token::TERM; }
{type}     { return token::TYPE; }
{int}      { lval->build<int>(atoi(YYText())); return token::NUMBER; }
{flt}      { lval->build<float>(atof(YYText())); return token::FLOAT; }
{clk}      { lval->build<expr::clock_t>(stoclk(YYText())); return token::CLOCK; }
{bool}     { lval->build<bool>(stob(YYText())); return token::BOOL; }
{str}      { lval->build<std::string>(std::string{YYText()}); return token::STRING; }
{accmod}   { lval->build<std::string>(std::string{YYText()}); return token::ACCESS_MOD; }
{id}       { lval->build<std::string>(std::string{YYText()}); return token::IDENTIFIER; }
<<EOF>>    { return token::YYEOF; }
.          { throw expr::parser::syntax_error(*loc, "invalid character: " + std::string(YYText())); }

%%
/* ================================================== */

