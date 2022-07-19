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
%skeleton "lalr1.cc"
%require "3.5"

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

// Forward declare the driver (include later)
%code requires {
    #include <string>
    #include <symbol_table.h>
    #include <operations.h>
    namespace expr { class driver; }
}

%param { expr::driver* drv }

// Enable parser location tracking
%locations

// Enable parser tracing and detailed errors
%define parse.trace
%define parse.error verbose
// Enable full lookahead to avoid incorrect error information
// See https://www.gnu.org/software/bison/manual/html_node/LAC.html for details
%define parse.lac full

// Include the driver
%code {
    #include "drivers/driver.h"
}

%define api.token.prefix {TOK_}
%token YYEOF 0
%token
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  PERCENT "%"
  HAT     "^"
  AND     "&&"
  OR      "||"
  XOR     "^^"
  IMPLIES "=>"
  GT      ">"
  GE      ">="
  EE      "=="
  NE      "!="
  LE      "<="
  LT      "<"
  NOT     "!"
  LPAREN  "("
  RPAREN  ")"
  ASSIGN  ":="
  ACCMOD  "access_modifier"
  TYPE    "type"
  TERM    ";"
;

// Identifiers are strings
%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <float> FLOAT "float"
%token <bool> BOOL "bool"
%token <std::string> STRING "string"
%nterm <expr::syntax_tree_t> exp bin_op mono_op
%nterm <expr::symbol_value_t> lit
%printer { yyo << $$; } <*>;

%left XOR
%left OR
%left AND
%left GT GE EE NE LE LT
%left PLUS MINUS STAR SLASH PERCENT HAT
%left IMPLIES
%precedence LPAREN NOT
%%
%start unit;
unit:
  statements    { }
| exp           { drv->add_tree($1); }
;

statements:
  %empty               {}
| statement statements {}
;

statement:
  "identifier" ASSIGN exp                          { drv->add_tree($1, expr::syntax_tree_t{}.concat($3)); }
| "type" "identifier" ASSIGN exp                   { drv->add_tree($2, expr::syntax_tree_t{}.concat($4)); }
| "access_modifier" "type" "identifier" ASSIGN exp { drv->add_tree($3, expr::syntax_tree_t{}.concat($5)); }
| statement TERM                                   { }
;

exp:
  lit                   { $$ = expr::syntax_tree_t{$1}; }
| bin_op                { $$ = $1; }
| mono_op               { $$ = $1; }
| "identifier"          { $$ = drv->get_symbol($1); }
;

bin_op:
  exp PLUS exp          { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::plus}}.concat($1).concat($3); }
| exp MINUS exp         { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::minus}}.concat($1).concat($3); }
| exp STAR exp          { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::star}}.concat($1).concat($3); }
| exp SLASH exp         { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::slash}}.concat($1).concat($3); }
| exp PERCENT exp       { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::percent}}.concat($1).concat($3); }
| exp HAT exp           { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::hat}}.concat($1).concat($3); }
| exp GT  exp           { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::gt}}.concat($1).concat($3); }
| exp GE exp            { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::ge}}.concat($1).concat($3); }
| exp EE exp            { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::ee}}.concat($1).concat($3); }
| exp NE exp            { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::ne}}.concat($1).concat($3); }
| exp LE exp            { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::le}}.concat($1).concat($3); }
| exp LT  exp           { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::lt}}.concat($1).concat($3); }
| exp OR exp            { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::_or}}.concat($1).concat($3); }
| exp XOR exp           { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::_xor}}.concat($1).concat($3); }
| exp IMPLIES exp       { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::_implies}}.concat($1).concat($3); }
| exp AND exp           { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::_and}}.concat($1).concat($3); }
;

mono_op:
  NOT exp               { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::_not}}.concat($2); }
| LPAREN exp RPAREN     { $$ = expr::syntax_tree_t{expr::operator_t{expr::operator_type_t::parentheses}}.concat($2); }
;

lit:
  "number"       { $$ = $1; }
| MINUS "number" { $$ = -$2; }
| "float"        { $$ = $1; }
| MINUS "float"  { $$ = -$2; }
| "string"       { $$ = $1; }
| "bool"         { $$ = $1; }
;
%%

void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
