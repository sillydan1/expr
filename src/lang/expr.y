%skeleton "lalr1.cc"
%require "3.2"
%defines "expr-parser.hpp"
%output "expr-parser.cpp"
%define api.parser.class { parser }
%define api.namespace { expr }
%define api.value.type variant
%define parse.assert
%locations

%code requires {
    #include <memory>
    #include "symbol_table.h"
    #include "factory.h"
    namespace expr {
        class scanner;
        struct parser_args {
            std::shared_ptr<scanner> scn;
            std::shared_ptr<ast_factory> fct;
        };
    }
}

%parse-param { parser_args& args }

%code {
    #include <iostream>
    #include "expr-scanner.hpp"
    #undef yylex
    #define yylex args.scn->yylex
}

/* ================================================== */
%token YYEOF 0
%token MINUS PLUS STAR SLASH PERCENT HAT AND OR XOR IMPLIES GT GE EE NE LE LT NOT LPAREN RPAREN ASSIGN TYPE TERM
%token <std::string> IDENTIFIER STRING ACCESS_MOD
%token <int> NUMBER
%token <float> FLOAT
%token <bool> BOOL
%token <clock_t> CLOCK
%nterm <syntax_tree_t> exp bin_op mono_op lit

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
| exp           { args.fct->build_root($1); } /* TODO: something something "expression_result" */
;

statements:
  %empty               { }
| statement statements { }
;

statement:
  IDENTIFIER ASSIGN exp                 { args.fct->build_declaration($1,     args.fct->build_root($3)); }
| TYPE IDENTIFIER ASSIGN exp            { args.fct->build_declaration($2,     args.fct->build_root($4)); }
| ACCESS_MOD IDENTIFIER ASSIGN exp      { args.fct->build_declaration($2, $1, args.fct->build_root($4)); }
| ACCESS_MOD TYPE IDENTIFIER ASSIGN exp { args.fct->build_declaration($3, $1, args.fct->build_root($5)); }
| statement TERM                        { }
;

exp:
  lit                   { $$ = $1; }
| bin_op                { $$ = $1; }
| mono_op               { $$ = $1; }
;

bin_op:
  exp PLUS exp          { $$ = args.fct->build_operator (expr::operator_type_t::plus,{$1,$3}); }
| exp MINUS exp         { $$ = args.fct->build_operator (expr::operator_type_t::minus,{$1,$3}); }
| exp STAR exp          { $$ = args.fct->build_operator (expr::operator_type_t::star,{$1,$3}); }
| exp SLASH exp         { $$ = args.fct->build_operator (expr::operator_type_t::slash,{$1,$3}); }
| exp PERCENT exp       { $$ = args.fct->build_operator (expr::operator_type_t::percent,{$1,$3}); }
| exp HAT exp           { $$ = args.fct->build_operator (expr::operator_type_t::hat,{$1,$3}); }
| exp GT  exp           { $$ = args.fct->build_operator (expr::operator_type_t::gt,{$1,$3}); }
| exp GE exp            { $$ = args.fct->build_operator (expr::operator_type_t::ge,{$1,$3}); }
| exp EE exp            { $$ = args.fct->build_operator (expr::operator_type_t::ee,{$1,$3}); }
| exp NE exp            { $$ = args.fct->build_operator (expr::operator_type_t::ne,{$1,$3}); }
| exp LE exp            { $$ = args.fct->build_operator (expr::operator_type_t::le,{$1,$3}); }
| exp LT  exp           { $$ = args.fct->build_operator (expr::operator_type_t::lt,{$1,$3}); }
| exp OR exp            { $$ = args.fct->build_operator (expr::operator_type_t::_or,{$1,$3}); }
| exp XOR exp           { $$ = args.fct->build_operator (expr::operator_type_t::_xor,{$1,$3}); }
| exp IMPLIES exp       { $$ = args.fct->build_operator (expr::operator_type_t::_implies,{$1,$3}); }
| exp AND exp           { $$ = args.fct->build_operator (expr::operator_type_t::_and,{$1,$3}); }
;

mono_op:
  NOT exp               { $$ = args.fct->build_operator (expr::operator_type_t::_not,{$2}); }
| LPAREN exp RPAREN     { $$ = args.fct->build_operator (expr::operator_type_t::parentheses,{$2}); }
;

lit:
  NUMBER                { $$ = args.fct->build_literal ($1); }
| MINUS NUMBER          { $$ = args.fct->build_literal (-$2); }
| FLOAT                 { $$ = args.fct->build_literal ($1); }
| MINUS FLOAT           { $$ = args.fct->build_literal (-$2); }
| STRING                { $$ = args.fct->build_literal ($1); }
| BOOL                  { $$ = args.fct->build_literal ($1); }
| CLOCK                 { $$ = args.fct->build_literal ($1); }
| IDENTIFIER            { $$ = args.fct->build_identifier ($1); }
;

%%
/* ================================================== */

void expr::parser::error(const location_type& l, const std::string& msg) {
    std::cerr << msg << " at " << l << "\n";
}

