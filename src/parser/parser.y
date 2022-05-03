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
  ASSIGN  ":="
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
%left PLUS MINUS STAR SLASH PERCENT HAT
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
  "identifier" ASSIGN exp                          { drv->set_symbol($1, $3); }
| "type" "identifier" ASSIGN exp                   { drv->set_symbol($2, $4); }
| "access_modifier" "type" "identifier" ASSIGN exp { drv->set_symbol($3, $5); }
| statement TERM                                   { }
;

exp:
  lit                   { $$ = $1; }
| exp PLUS exp          { $$ = drv->add($1,$3); }
| exp MINUS exp         { $$ = drv->sub($1,$3); }
| exp STAR exp          { $$ = drv->mul($1,$3); }
| exp SLASH exp         { $$ = drv->div($1,$3); }
| exp PERCENT exp       { $$ = drv->mod($1,$3); }
| exp HAT exp           { $$ = drv->pow($1,$3); }
| exp GT  exp           { $$ = drv->gt($1,$3); }
| exp GE exp            { $$ = drv->ge($1,$3); }
| exp EE exp            { $$ = drv->ee($1,$3); }
| exp NE exp            { $$ = drv->ne($1,$3); }
| exp LE exp            { $$ = drv->le($1,$3); }
| exp LT  exp           { $$ = drv->lt($1,$3); }
| exp OR exp            { $$ = drv->_or($1,$3); }
| exp AND exp           { $$ = drv->_and($1,$3); }
| NOT exp               { $$ = drv->_not($2); }
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
