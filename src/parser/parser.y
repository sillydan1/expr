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
    class driver;
}

%param { driver* drv }

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
  ASSIGN  ":="
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  PERCENT "%"
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
%nterm <symbol_value_t> lit exp
%printer { yyo << $$; } <*>;

%left OR
%left AND
%left GT GE EE NE LE LT
%left PLUS MINUS STAR SLASH PERCENT
%precedence LPAREN NOT
%%
%start unit;
unit:
  statements    { }
| exp           { drv->expression_result = $1; }
;

statements:
  %empty               {}
| statement statements {}
;

statement:
  "identifier" ASSIGN exp                          { drv->result[$1] = $3; }
| "type" "identifier" ASSIGN exp                   { drv->result[$2] = $4; }
| "access_modifier" "type" "identifier" ASSIGN exp { drv->result[$3] = $5; }
| statement TERM                                   { }
;

exp:
  lit                   { $$ = $1; }
| exp PLUS exp          { $$ = $1 + $3; }
| exp MINUS exp         { $$ = $1 - $3; }
| exp STAR exp          { $$ = $1 * $3; }
| exp SLASH exp         { $$ = $1 / $3; }
| exp PERCENT exp       { $$ = $1 % $3; }
| exp GT  exp           { $$ = gt_($1,$3); }
| exp GE exp            { $$ = ge_($1,$3); }
| exp EE exp            { $$ = ee_($1,$3); }
| exp NE exp            { $$ = ne_($1,$3); }
| exp LE exp            { $$ = le_($1,$3); }
| exp LT  exp           { $$ = lt_($1,$3); }
| exp OR exp            { $$ = or_($1,$3); }
| exp AND exp           { $$ = and_($1,$3); }
| NOT exp               { $$ = not_($2); }
| LPAREN exp RPAREN     { $$ = $2; }
;

lit:
  "number"       { $$ = $1; }
| MINUS "number" { $$ = -$2; }
| "float"        { $$ = $1; }
| MINUS "float"  { $$ = -$2; }
| "string"       { $$ = $1; }
| "bool"         { $$ = $1; }
| "identifier"   { $$ = drv->get_symbol($1); }
;
%%

void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
