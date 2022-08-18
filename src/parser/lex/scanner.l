m4_changequote()
/*
*/

m4_include(skeleton.l)

m4_include(tokens.l)

%{
  // Code run each time a pattern is matched.
  #define YY_USER_ACTION  loc.columns(yyleng);
%}

%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv->location;
  // Code run each time yylex is called.
  loc.step();
%}

m4_include(lexer.l)

%%

m4_include(footer.l)
