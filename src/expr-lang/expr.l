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
%option prefix="expr"

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
{type}     { 
                lval->build<expr::symbol_type_name_t>(stotypename(std::string{YYText()}));
                return token::TYPE; 
           }
{int}      { 
                lval->build<int>(atoi(YYText())); 
                return token::NUMBER; 
           }
{flt}      { 
                lval->build<float>(atof(YYText())); 
                return token::FLOAT; 
           }
{clk}      { 
                lval->build<expr::clock_t>(stoclk(YYText())); 
                return token::CLOCK; 
           }
{bool}     { 
                lval->build<bool>(stob(YYText())); 
                return token::BOOL; 
           }
{str}      { 
                std::string s{YYText()}; 
                lval->build<std::string>(s.substr(1, s.size()-2)); 
                return token::STRING; 
           }
{accmod}   { 
                lval->build<expr::symbol_access_modifier_t>(stoaccmod(std::string{YYText()}));
                return token::ACCESS_MOD; 
           }
{id}       { 
                lval->build<std::string>(std::string{YYText()}); 
                return token::IDENTIFIER; 
           }
<<EOF>>    return token::YYEOF;
.          throw expr::parser::syntax_error(*loc, "invalid character: " + std::string(YYText()));

%%
/* ================================================== */

namespace expr {
    scanner::scanner(std::istream& arg_yyin, std::ostream& arg_yyout, ast_factory* fct) : exprFlexLexer{arg_yyin, arg_yyout}, fct(fct) {}
    scanner::scanner(std::istream* arg_yyin, std::ostream* arg_yyout, ast_factory* fct) : exprFlexLexer{arg_yyin, arg_yyout}, fct(fct) {}
    scanner::~scanner() {}
}

