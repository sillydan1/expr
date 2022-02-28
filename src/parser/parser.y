%skeleton "lalr1.cc"
%require "3.8"
%header

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

// Forward declare the driver (include later)
%code requires {
    #include <string>
    #include <symbol_table.h>
    #include <operations/operations.h>
    class driver;
}

%param { driver& drv }

// Enable parser location tracking
%locations

// Enable parser tracing and detailed errors
%define parse.trace
%define parse.error detailed
// Enable full lookahead to avoid incorrect error information
// See https://www.gnu.org/software/bison/manual/html_node/LAC.html for details
%define parse.lac full

// Include the driver
%code {
    #include "parser/driver.h"
}

%define api.token.prefix {TOK_}
%token
  ASSIGN  ":="
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  AND     "&&"
  OR      "||"
  NOT     "!"
  LPAREN  "("
  RPAREN  ")"
  TERM    ";"
;

// Identifiers are strings
%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <float> FLOAT "float"
%token <bool> BOOL "bool"
%token <std::string> STRING "string"
%nterm <symbol_value_t> exp
%printer { yyo << $$; } <*>;

%%
%start unit;
unit: assignments { };

assignments:
  %empty                     {}
| assignment                 {}
| assignment ";" assignments {}
;

assignment:
  "identifier" ":=" exp { drv.result[$1] = $3; };

%left "+" "-";
%left "*" "/";
exp:
  "number"      { $$ = $1; }
| "float"       { $$ = $1; }
| "string"      { $$ = $1; }
| "bool"        { $$ = $1; }
| "identifier"  { $$ = drv.environment.at($1); }
| exp "+" exp   { $$ = $1 + $3; }
| exp "-" exp   { $$ = $1 - $3; }
| exp "*" exp   { $$ = $1 * $3; }
| exp "/" exp   { $$ = $1 / $3; }
| exp "||" exp  { $$ = or_($1,$3); }
| exp "&&" exp  { $$ = and_($1,$3); }
| "!" exp       { $$ = not_($2); }
| "(" exp ")"   { $$ = $2; }
%%

void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
