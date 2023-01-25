/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */


// ******* 	NAME IS BELOW VVVV  ******* //
//Name: David Zalewski
//ID: 1215324552
//ASURITE: dzalews1
//
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include "reg.h"
#include <vector>

class Parser {
  public:
    void parse_input();
    void readAndPrintAllInput();
  private:
    LexicalAnalyzer lexer;
    void syntax_error();
    Token expect(TokenType expected_type);
    void parse_tokens_section();
    void parse_token_list();
    void parse_token();
    struct REG* parse_expr();
};

#endif

