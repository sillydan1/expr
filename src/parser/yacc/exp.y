m4_changequote()

exp:
  lit                   { $$ = $1; }
| bin_op                { $$ = $1; }
| mono_op               { $$ = $1; }
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
  "number"              { $$ = expr::syntax_tree_t{$1}; }
| MINUS "number"        { $$ = expr::syntax_tree_t{-$2}; }
| "float"               { $$ = expr::syntax_tree_t{$1}; }
| MINUS "float"         { $$ = expr::syntax_tree_t{-$2}; }
| "string"              { $$ = expr::syntax_tree_t{$1}; }
| "bool"                { $$ = expr::syntax_tree_t{$1}; }
| "identifier"          { $$ = drv->get_symbol($1); }
;
