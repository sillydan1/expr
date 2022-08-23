m4_changequote()

exp:
  lit                   { $$ = $1; }
| bin_op                { $$ = $1; }
| mono_op               { $$ = $1; }
;

bin_op:
  exp PLUS exp          { $$ = BINOP_CTOR(plus,$1,$3);     }
| exp MINUS exp         { $$ = BINOP_CTOR(minus,$1,$3);    }
| exp STAR exp          { $$ = BINOP_CTOR(star,$1,$3);     }
| exp SLASH exp         { $$ = BINOP_CTOR(slash,$1,$3);    }
| exp PERCENT exp       { $$ = BINOP_CTOR(percent,$1,$3);  }
| exp HAT exp           { $$ = BINOP_CTOR(hat,$1,$3);      }
| exp GT  exp           { $$ = BINOP_CTOR(gt,$1,$3);       }
| exp GE exp            { $$ = BINOP_CTOR(ge,$1,$3);       }
| exp EE exp            { $$ = BINOP_CTOR(ee,$1,$3);       }
| exp NE exp            { $$ = BINOP_CTOR(ne,$1,$3);       }
| exp LE exp            { $$ = BINOP_CTOR(le,$1,$3);       }
| exp LT  exp           { $$ = BINOP_CTOR(lt,$1,$3);       }
| exp OR exp            { $$ = BINOP_CTOR(_or,$1,$3);      }
| exp XOR exp           { $$ = BINOP_CTOR(_xor,$1,$3);     }
| exp IMPLIES exp       { $$ = BINOP_CTOR(_implies,$1,$3); }
| exp AND exp           { $$ = BINOP_CTOR(_and,$1,$3);     }
;

mono_op:
  NOT exp               { $$ = MONOOP_CTOR(_not,$2);        }
| LPAREN exp RPAREN     { $$ = MONOOP_CTOR(parentheses,$2); }
;

lit:
  "number"              { $$ = LIT_CTOR($1);   }
| MINUS "number"        { $$ = LIT_CTOR(-$2);  }
| "float"               { $$ = LIT_CTOR($1);   }
| MINUS "float"         { $$ = LIT_CTOR(-$2);  }
| "string"              { $$ = LIT_CTOR($1);   }
| "bool"                { $$ = LIT_CTOR($1);   }
| "identifier"          { $$ = IDENT_CTOR($1); }
;
