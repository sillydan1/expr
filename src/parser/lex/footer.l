m4_changequote()

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

void expr::driver::scan_begin() {
    yy_flex_debug = trace_scanning;
    if(file.empty() || file == "-")
        yyin = stdin;
    else
        yy_scan_string(file.c_str());
}

void expr::driver::scan_end() {
}
