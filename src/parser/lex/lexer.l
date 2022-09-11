m4_changequote()

{blank}+   loc.step();
\n+        { loc.lines(yyleng); loc.step(); }

"-"        return PARSER_NS ::parser::make_MINUS  (loc);
"+"        return PARSER_NS ::parser::make_PLUS   (loc);
"*"        return PARSER_NS ::parser::make_STAR   (loc);
"/"        return PARSER_NS ::parser::make_SLASH  (loc);
"%"        return PARSER_NS ::parser::make_PERCENT(loc);
"^"        return PARSER_NS ::parser::make_HAT    (loc);
"&&"       return PARSER_NS ::parser::make_AND    (loc);
"||"       return PARSER_NS ::parser::make_OR     (loc);
"^^"       return PARSER_NS ::parser::make_XOR    (loc);
"=>"       return PARSER_NS ::parser::make_IMPLIES(loc);
">"        return PARSER_NS ::parser::make_GT     (loc);
">="       return PARSER_NS ::parser::make_GE     (loc);
"=="       return PARSER_NS ::parser::make_EE     (loc);
"!="       return PARSER_NS ::parser::make_NE     (loc);
"<="       return PARSER_NS ::parser::make_LE     (loc);
"<"        return PARSER_NS ::parser::make_LT     (loc);
"!"        return PARSER_NS ::parser::make_NOT    (loc);
"("        return PARSER_NS ::parser::make_LPAREN (loc);
")"        return PARSER_NS ::parser::make_RPAREN (loc);
":="       return PARSER_NS ::parser::make_ASSIGN (loc);
";"        return PARSER_NS ::parser::make_TERM   (loc);

{accmod}   return PARSER_NS ::parser::make_ACCMOD(loc);
{type}     return PARSER_NS ::parser::make_TYPE(loc);

{int}      return make_NUMBER(yytext, loc);
{flt}      return make_FLOAT(yytext, loc);
{str}      return make_STRING(yytext, loc);
{bool}     return make_BOOL(yytext, loc);
{id}       return PARSER_NS ::parser::make_IDENTIFIER (yytext, loc);
.          { throw PARSER_NS ::parser::syntax_error(loc, "invalid character: " + std::string(yytext)); }
<<EOF>>    return PARSER_NS ::parser::make_YYEOF (loc);
