m4_changequote()
/*
m4_include(mit.license)
*/

m4_include(skeleton.y)

m4_include(tokens.y)

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
  "identifier" ASSIGN exp                          { drv->add_tree($1, expr::syntax_tree_t{}.concat($3)); }
| "type" "identifier" ASSIGN exp                   { drv->add_tree($2, expr::syntax_tree_t{}.concat($4)); }
| "access_modifier" "type" "identifier" ASSIGN exp { drv->add_tree($3, expr::syntax_tree_t{}.concat($5)); }
| statement TERM                                   { }
;

m4_include(exp.y)

%%
