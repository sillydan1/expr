/* MIT License
 *
 * Copyright (c) 2022 Asger Gitz-Johansen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef EXPR_SCANNER_HPP
#define EXPR_SCANNER_HPP
#include "expr-lang/ast-factory.h"
#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "expr-lang/expr-parser.hpp"
#include "expr-lang/location.hh"

namespace expr {
    class scanner : public yyFlexLexer {
    public:
        scanner(std::istream& arg_yyin, std::ostream& arg_yyout, ast_factory* fct) : yyFlexLexer{arg_yyin, arg_yyout}, fct(fct) {}
        scanner(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr, ast_factory* fct = nullptr) : yyFlexLexer{arg_yyin, arg_yyout}, fct(fct) {}
        virtual ~scanner() = default;
        virtual int yylex(parser::semantic_type* const lval, parser::location_type* location);
    private:
        ast_factory* fct = nullptr;
    };
}

#endif

