m4_changequote()
%skeleton "lalr1.cc"
%require "3.5"

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

// Forward declare the driver (include later)
%code requires {
    #include <string>
    #include <symbol_table.h>
    #include <operations.h>
    namespace expr { class driver; }
}

%param { expr::driver* drv }

// Enable parser location tracking
%locations

// Enable parser tracing and detailed errors
%define parse.trace
%define parse.error verbose
// Enable full lookahead to avoid incorrect error information
// See https://www.gnu.org/software/bison/manual/html_node/LAC.html for details
%define parse.lac full

// Include the driver
%code {
    #include "drivers/driver.h"
    void PARSER_NS ::parser::error (const location_type& l, const std::string& m) {
      std::cerr << l << ": " << m << '\n';
    }
}
