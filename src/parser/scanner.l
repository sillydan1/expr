%{
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring> // strerror
#include <string>
#include <sstream>
#include "parser/driver.h"
#include "parser.hpp"
%}

%{
#if defined __clang__
# define CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#endif

// Clang and ICC like to pretend they are GCC.
#if defined __GNUC__ && !defined __clang__ && !defined __ICC
# define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#endif

// Pacify warnings in yy_init_buffer (observed with Flex 2.6.4)
// and GCC 6.4.0, 7.3.0 with -O3.
#if defined GCC_VERSION && 600 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wnull-dereference"
#endif

// This example uses Flex's C back end, yet compiles it as C++.
// So expect warnings about C style casts and NULL.
#if defined CLANG_VERSION && 500 <= CLANG_VERSION
# pragma clang diagnostic ignored "-Wold-style-cast"
# pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#elif defined GCC_VERSION && 407 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wold-style-cast"
# pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)

// Old versions of Flex (2.5.35) generate an incomplete documentation comment.
//
//  In file included from src/scan-code-c.c:3:
//  src/scan-code.c:2198:21: error: empty paragraph passed to '@param' command
//        [-Werror,-Wdocumentation]
//   * @param line_number
//     ~~~~~~~~~~~~~~~~~^
//  1 error generated.
#if FLEX_VERSION < 206 && defined CLANG_VERSION
# pragma clang diagnostic ignored "-Wdocumentation"
#endif

// Old versions of Flex (2.5.35) use 'register'.  Warnings introduced in
// GCC 7 and Clang 6.
#if FLEX_VERSION < 206
# if defined CLANG_VERSION && 600 <= CLANG_VERSION
#  pragma clang diagnostic ignored "-Wdeprecated-register"
# elif defined GCC_VERSION && 700 <= GCC_VERSION
#  pragma GCC diagnostic ignored "-Wregister"
# endif
#endif

#if FLEX_VERSION < 206
# if defined CLANG_VERSION
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wdocumentation"
#  pragma clang diagnostic ignored "-Wshorten-64-to-32"
#  pragma clang diagnostic ignored "-Wsign-conversion"
# elif defined GCC_VERSION
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
# endif
#endif

// Flex 2.6.4, GCC 9
// warning: useless cast to type 'int' [-Wuseless-cast]
// 1361 |   YY_CURRENT_BUFFER_LVALUE->yy_buf_size = (int) (new_size - 2);
//      |                                                 ^
#if defined GCC_VERSION && 900 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
%}

%option noyywrap nounput noinput batch debug

%{
  // A number symbol corresponding to the value in provided string.
  yy::parser::symbol_type make_NUMBER(const std::string &s, const yy::parser::location_type& loc);
  yy::parser::symbol_type make_FLOAT(const std::string &s, const yy::parser::location_type& loc);
  yy::parser::symbol_type make_STRING(const std::string &s, const yy::parser::location_type& loc);
  yy::parser::symbol_type make_BOOL(std::string s, const yy::parser::location_type& loc);
%}

%{
 // TODO: Remove [ðđ€\(\)]
%}
id     [a-z_A-Z]([.ðđ€\(\)a-zA-Z_0-9]*[a-zA-Z_0-9]+)?
int    [0-9]+
flt    [0-9]+[.][0-9]+f
bool   [Ff]alse|[Tt]rue
str    \"(\\.|[^"\\])*\"
blank  [ \t\r]
accmod [Pp](ublic|rivate|rotected)
type   int|long|float|double|string|bool|var|auto

%{
  // Code run each time a pattern is matched.
  #define YY_USER_ACTION  loc.columns(yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step();
%}
{blank}+   loc.step();
\n+        loc.lines(yyleng); loc.step();

"-"        return yy::parser::make_MINUS  (loc);
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"/"        return yy::parser::make_SLASH  (loc);
"&&"       return yy::parser::make_AND    (loc);
"||"       return yy::parser::make_OR     (loc);
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
%%

yy::parser::symbol_type make_NUMBER(const std::string &s, const yy::parser::location_type& loc) {
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_NUMBER ((int) n, loc);
}

yy::parser::symbol_type make_FLOAT(const std::string &s, const yy::parser::location_type& loc) {
    try {
        double n = std::stod(s.c_str());
        return yy::parser::make_FLOAT((double)n, loc);
    } catch(std::out_of_range& e) {
        throw yy::parser::syntax_error (loc, "double is out of range: " + s);
    }
}

yy::parser::symbol_type make_STRING(const std::string &s, const yy::parser::location_type& loc) {
    return yy::parser::make_STRING(s.substr(1, s.size()-2), loc);
}

yy::parser::symbol_type make_BOOL(std::string s, const yy::parser::location_type& loc) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    bool b;
    std::istringstream(s) >> std::boolalpha >> b;
    return yy::parser::make_BOOL(b, loc);
}

void driver::scan_begin() {
    yy_flex_debug = trace_scanning;
    if(file.empty() || file == "-")
        yyin = stdin;
    else {
        yy_scan_string(file.c_str());
    }
//    else if(!(yyin = fopen(file.c_str(), "r"))) {
//        std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
//        exit(EXIT_FAILURE); // TODO: This is pretty aggressive, maybe just throw exception instead
//    }
}

void driver::scan_end() {
//    fclose(yyin);
}
