m4_changequote()

void expr::driver::scan_begin() {
    yy_flex_debug = trace_scanning;
    if(file.empty() || file == "-")
        yyin = stdin;
    else
        yy_scan_string(file.c_str());
}

void expr::driver::scan_end() {
}
