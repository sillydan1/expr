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
    #include "parser/driver.h"
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
%nterm <syntax_tree_t> exp bin_op mono_op
%nterm <symbol_value_t> lit
%printer { yyo << $$; } <*>;

%left OR
%left AND
%left GT GE EE NE LE LT
%left PLUS MINUS STAR SLASH PERCENT HAT
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
  "identifier" ASSIGN exp                          { drv->add_tree($1, syntax_tree_t{}.concat($3)); }
| "type" "identifier" ASSIGN exp                   { drv->add_tree($2, syntax_tree_t{}.concat($4)); }
| "access_modifier" "type" "identifier" ASSIGN exp { drv->add_tree($3, syntax_tree_t{}.concat($5)); }
| statement TERM                                   { }
;

exp:
  lit                   { $$ = syntax_tree_t{$1}; }
| bin_op                { $$ = $1; }
| mono_op               { $$ = $1; }
| "identifier"          { $$ = drv->get_symbol($1); }
;

bin_op:
  exp PLUS exp          { $$ = syntax_tree_t{operator_t{operator_type_t::plus}}.concat($1).concat($3); }
| exp MINUS exp         { $$ = syntax_tree_t{operator_t{operator_type_t::minus}}.concat($1).concat($3); }
| exp STAR exp          { $$ = syntax_tree_t{operator_t{operator_type_t::star}}.concat($1).concat($3); }
| exp SLASH exp         { $$ = syntax_tree_t{operator_t{operator_type_t::slash}}.concat($1).concat($3); }
| exp PERCENT exp       { $$ = syntax_tree_t{operator_t{operator_type_t::percent}}.concat($1).concat($3); }
| exp HAT exp           { $$ = syntax_tree_t{operator_t{operator_type_t::hat}}.concat($1).concat($3); }
| exp GT  exp           { $$ = syntax_tree_t{operator_t{operator_type_t::gt}}.concat($1).concat($3); }
| exp GE exp            { $$ = syntax_tree_t{operator_t{operator_type_t::ge}}.concat($1).concat($3); }
| exp EE exp            { $$ = syntax_tree_t{operator_t{operator_type_t::ee}}.concat($1).concat($3); }
| exp NE exp            { $$ = syntax_tree_t{operator_t{operator_type_t::ne}}.concat($1).concat($3); }
| exp LE exp            { $$ = syntax_tree_t{operator_t{operator_type_t::le}}.concat($1).concat($3); }
| exp LT  exp           { $$ = syntax_tree_t{operator_t{operator_type_t::lt}}.concat($1).concat($3); }
| exp OR exp            { $$ = syntax_tree_t{operator_t{operator_type_t::_or}}.concat($1).concat($3); }
| exp AND exp           { $$ = syntax_tree_t{operator_t{operator_type_t::_and}}.concat($1).concat($3); }
;

mono_op:
  NOT exp               { $$ = syntax_tree_t{operator_t{operator_type_t::_not}}.concat($2); }
| LPAREN exp RPAREN     { $$ = syntax_tree_t{operator_t{operator_type_t::parentheses}}.concat($2); }
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
