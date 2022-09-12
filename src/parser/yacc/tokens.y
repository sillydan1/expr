m4_changequote()

%define api.token.prefix {TOK_}
%token YYEOF 0
%token
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  PERCENT "%"
  HAT     "^"
  AND     "&&"
  OR      "||"
  XOR     "^^"
  IMPLIES "=>"
  GT      ">"
  GE      ">="
  EE      "=="
  NE      "!="
  LE      "<="
  LT      "<"
  NOT     "!"
  LPAREN  "("
  RPAREN  ")"
  ASSIGN  ":="
  ACCMOD  "access_modifier"
  TYPE    "type"
  TERM    ";"
;

// Identifiers are strings
%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <float> FLOAT "float"
%token <bool> BOOL "bool"
%token <expr::clock_t> CLOCK "clk"
%token <std::string> STRING "string"
%printer { yyo << $$; } <*>;

%left XOR
%left OR
%left AND
%left GT GE EE NE LE LT
%left PLUS MINUS STAR SLASH PERCENT HAT
%left IMPLIES
%precedence LPAREN NOT
