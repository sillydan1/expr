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
    namespace expr {
        class scanner;
        class factory;
    }
}

%parse-param { scanner& scn, factory& fct }

%code {
    #include <iostream>
    #include "expr-scanner.hpp"
    #undef yylex
    #define yylex scn.yylex
}

/* ================================================== */
%token YYEOF 0
%token MINUS PLUS STAR SLASH PERCENT HAT AND OR XOR IMPLIES GT GE EE NE LE LT NOT LPAREN RPAREN ASSIGN TYPE TERM
%token <std::string> IDENTIFIER STRING ACCESS_MOD
%token <int> NUMBER
%token <float> FLOAT
%token <bool> BOOL
%token <expr::clock_t> CLOCK
%nterm <expr::syntax_tree_t> exp bin_op mono_op lit

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
  %empty               { }
| statement statements { }
;

statement:
  IDENTIFIER ASSIGN exp                 { fct.add_tree($1, expr::syntax_tree_t{}.concat($3)); }
| TYPE IDENTIFIER ASSIGN exp            { fct.add_tree($2, expr::syntax_tree_t{}.concat($4)); }
| ACCESS_MOD IDENTIFIER ASSIGN exp      { fct.add_tree($1, $2, expr::syntax_tree_t{}.concat($4)); }
| ACCESS_MOD TYPE IDENTIFIER ASSIGN exp { fct.add_tree($1, $3, expr::syntax_tree_t{}.concat($5)); }
| statement TERM                        { }
;

exp:
  lit                   { $$ = $1; }
| bin_op                { $$ = $1; }
| mono_op               { $$ = $1; }
;

bin_op:
  exp PLUS exp          { $$ = fct.op (plus,$1,$3);     }
| exp MINUS exp         { $$ = fct.op (minus,$1,$3);    }
| exp STAR exp          { $$ = fct.op (star,$1,$3);     }
| exp SLASH exp         { $$ = fct.op (slash,$1,$3);    }
| exp PERCENT exp       { $$ = fct.op (percent,$1,$3);  }
| exp HAT exp           { $$ = fct.op (hat,$1,$3);      }
| exp GT  exp           { $$ = fct.op (gt,$1,$3);       }
| exp GE exp            { $$ = fct.op (ge,$1,$3);       }
| exp EE exp            { $$ = fct.op (ee,$1,$3);       }
| exp NE exp            { $$ = fct.op (ne,$1,$3);       }
| exp LE exp            { $$ = fct.op (le,$1,$3);       }
| exp LT  exp           { $$ = fct.op (lt,$1,$3);       }
| exp OR exp            { $$ = fct.op (_or,$1,$3);      }
| exp XOR exp           { $$ = fct.op (_xor,$1,$3);     }
| exp IMPLIES exp       { $$ = fct.op (_implies,$1,$3); }
| exp AND exp           { $$ = fct.op (_and,$1,$3);     }
;

mono_op:
  NOT exp               { $$ = fct.op (_not,$2);        }
| LPAREN exp RPAREN     { $$ = fct.op (parentheses,$2); }
;

lit:
  NUMBER                { $$ = fct.lit ($1);   }
| MINUS NUMBER          { $$ = fct.lit (-$2);  }
| FLOAT                 { $$ = fct.lit ($1);   }
| MINUS FLOAT           { $$ = fct.lit (-$2);  }
| STRING                { $$ = fct.lit ($1);   }
| BOOL                  { $$ = fct.lit ($1);   }
| CLOCK                 { $$ = fct.lit ($1);   }
| IDENTIFIER            { $$ = fct.ident ($1); }
;

%%
/* ================================================== */

void expr::parser::error(const location_type& l, const std::string& msg) {
    std::cerr << msg << " at " << l << "\n";
}

