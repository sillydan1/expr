m4_changequote()

{blank}+   loc.step();
\n+        { loc.lines(yyleng); loc.step(); }

"-"        return yy::parser::make_MINUS  (loc);
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"/"        return yy::parser::make_SLASH  (loc);
"%"        return yy::parser::make_PERCENT(loc);
"^"        return yy::parser::make_HAT    (loc);
"&&"       return yy::parser::make_AND    (loc);
"||"       return yy::parser::make_OR     (loc);
"^^"       return yy::parser::make_XOR    (loc);
"=>"       return yy::parser::make_IMPLIES(loc);
">"        return yy::parser::make_GT     (loc);
">="       return yy::parser::make_GE     (loc);
"=="       return yy::parser::make_EE     (loc);
"!="       return yy::parser::make_NE     (loc);
"<="       return yy::parser::make_LE     (loc);
"<"        return yy::parser::make_LT     (loc);
"!"        return yy::parser::make_NOT    (loc);
"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
":="       return yy::parser::make_ASSIGN (loc);
";"        return yy::parser::make_TERM   (loc);

{accmod}   return yy::parser::make_ACCMOD(loc);
{type}     return yy::parser::make_TYPE(loc);

{int}      return make_NUMBER(yytext, loc);
{flt}      return make_FLOAT(yytext, loc);
{str}      return make_STRING(yytext, loc);
{bool}     return make_BOOL(yytext, loc);
{id}       return yy::parser::make_IDENTIFIER (yytext, loc);
.          { throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext)); }
<<EOF>>    return yy::parser::make_YYEOF (loc);
