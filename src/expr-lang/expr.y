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
%require "3.2"
%defines "expr-parser.hpp"
%output "expr-parser.cpp"
%define api.parser.class { parser }
%define api.value.automove true
%define api.namespace { expr }
%define api.value.type variant
%define parse.assert
%define parse.lac full
%define parse.trace true
%define parse.error detailed
%locations

%code requires {
    #include <memory>
    #include "symbol/symbol_table.h"
    #include "ast-factory.h"
    #include "language-builder.h"

    namespace expr {
        class scanner;
        struct parser_args {
            std::optional<std::string> expression; // used for debugging / error message purposes
            scanner* scn;
            ast_factory* fct;
            language_builder* builder;
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
%token MINUS PLUS STAR SLASH PERCENT HAT AND OR XOR IMPLIES GT GE EE NE LE LT NOT LPAREN RPAREN ASSIGN TERM
%token <std::string> IDENTIFIER
%token <symbol_access_modifier_t> ACCESS_MOD
%token <symbol_type_name_t> TYPE
%token <int> NUMBER
%token <float> FLOAT
%token <bool> BOOL
%token <std::string> STRING 
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
| exp           { args.builder->add_expression(args.fct->build_root($1)); }
;

statements:
  %empty               { }
| statements statement { }
;

statement:
  IDENTIFIER ASSIGN exp                 { args.builder->add_declaration($1, args.fct->build_root($3)        ); }
| TYPE IDENTIFIER ASSIGN exp            { args.builder->add_declaration($2, args.fct->build_root($4), $1    ); }
| ACCESS_MOD IDENTIFIER ASSIGN exp      { args.builder->add_declaration($2, args.fct->build_root($4), $1    ); }
| ACCESS_MOD TYPE IDENTIFIER ASSIGN exp { args.builder->add_declaration($3, args.fct->build_root($5), $2, $1); }
| statement TERM                        { }
;

exp:
  lit                   { $$ = $1; }
| bin_op                { $$ = $1; }
| mono_op               { $$ = $1; }
;

bin_op:
  exp PLUS exp          { $$ = args.fct->build_operator (expr::operator_type_t::plus,$1,$3); }
| exp MINUS exp         { $$ = args.fct->build_operator (expr::operator_type_t::minus,$1,$3); }
| exp STAR exp          { $$ = args.fct->build_operator (expr::operator_type_t::star,$1,$3); }
| exp SLASH exp         { $$ = args.fct->build_operator (expr::operator_type_t::slash,$1,$3); }
| exp PERCENT exp       { $$ = args.fct->build_operator (expr::operator_type_t::percent,$1,$3); }
| exp HAT exp           { $$ = args.fct->build_operator (expr::operator_type_t::hat,$1,$3); }
| exp GT  exp           { $$ = args.fct->build_operator (expr::operator_type_t::gt,$1,$3); }
| exp GE exp            { $$ = args.fct->build_operator (expr::operator_type_t::ge,$1,$3); }
| exp EE exp            { $$ = args.fct->build_operator (expr::operator_type_t::ee,$1,$3); }
| exp NE exp            { $$ = args.fct->build_operator (expr::operator_type_t::ne,$1,$3); }
| exp LE exp            { $$ = args.fct->build_operator (expr::operator_type_t::le,$1,$3); }
| exp LT  exp           { $$ = args.fct->build_operator (expr::operator_type_t::lt,$1,$3); }
| exp OR exp            { $$ = args.fct->build_operator (expr::operator_type_t::_or,$1,$3); }
| exp XOR exp           { $$ = args.fct->build_operator (expr::operator_type_t::_xor,$1,$3); }
| exp IMPLIES exp       { $$ = args.fct->build_operator (expr::operator_type_t::_implies,$1,$3); }
| exp AND exp           { $$ = args.fct->build_operator (expr::operator_type_t::_and,$1,$3); }
;

mono_op:
  NOT exp               { $$ = args.fct->build_operator (expr::operator_type_t::_not,$2); }
| LPAREN exp RPAREN     { $$ = args.fct->build_operator (expr::operator_type_t::parentheses,$2); }
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
    // TODO: We should print to the injected stream instead of directly to cerr
    // TODO: This should provide a WINDOW to peek into rather than print the entire file/expression
    // TODO: This is a mess... I can't use yylineno and l.begin/end.line is always just '1' so this insane code will suffice for now
    if(args.expression) {
        std::cerr << msg << " between column " << l.begin.column << " and " << l.end.column << ":\n";
        auto& e = args.expression.value();
        int i = 0; int offset = 0;
        for(; i < l.begin.column - 1; i++, offset++) {
            if(e[i] == '\n')
                offset = 0;
            std::cerr << e[i];
        }
        for(; i < l.end.column - 1; i++)
            std::cerr << e[i];
        for(; i < e.size() - 1; i++) {
            if(e[i] == '\n')
                break;
            std::cerr << e[i];
        }
        std::cerr << "\n";
        for(int j = 0; j < offset-1; j++)
            std::cerr << "~";
        for(int j = l.begin.column - 1; j < l.end.column-1; j++)
            std::cerr << "^";
        std::cerr << "\n";
        for(i++; i < e.size() - 1; i++)
            std::cerr << e[i];
        std::cerr << "\n";
    } else
        std::cerr << msg << " at " << l << "\n"; // boring
}

