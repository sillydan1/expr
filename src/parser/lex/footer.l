m4_changequote()

PARSER_NS ::parser::symbol_type make_NUMBER(const std::string &s, const PARSER_NS ::parser::location_type& loc) {
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw PARSER_NS ::parser::syntax_error (loc, "integer is out of range: " + s);
  return PARSER_NS ::parser::make_NUMBER ((int) n, loc);
}

PARSER_NS ::parser::symbol_type make_FLOAT(const std::string &s, const PARSER_NS ::parser::location_type& loc) {
    try {
        double n = std::stod(s.c_str());
        return PARSER_NS ::parser::make_FLOAT((double)n, loc);
    } catch(std::out_of_range& e) {
        throw PARSER_NS ::parser::syntax_error (loc, "double is out of range: " + s);
    }
}

PARSER_NS ::parser::symbol_type make_STRING(const std::string &s, const PARSER_NS ::parser::location_type& loc) {
    return PARSER_NS ::parser::make_STRING(s.substr(1, s.size()-2), loc);
}

PARSER_NS ::parser::symbol_type make_BOOL(std::string s, const PARSER_NS ::parser::location_type& loc) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    bool b;
    std::istringstream(s) >> std::boolalpha >> b;
    return PARSER_NS ::parser::make_BOOL(b, loc);
}

PARSER_NS ::parser::symbol_type make_CLOCK(const std::string &s, const PARSER_NS ::parser::location_type& loc) {
    try {
        auto c = expr::stoclk(s.c_str());
        return PARSER_NS ::parser::make_CLOCK(c, loc);
    } catch(std::out_of_range& e) {
        throw PARSER_NS ::parser::syntax_error (loc, "clock value is out of range: " + s);
    }
}
