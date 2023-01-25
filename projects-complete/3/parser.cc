/* Name: David Zalewski
 * Class: CSE 340
 * ID: 1215324552
 *
 */



/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR !!&%!!\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Parsing

// This function is simply to illustrate the GetToken() function
// you will not need it for your project and you can delete it
// the function also illustrates the use of peek()
void Parser::ConsumeAllInput()
{
    Token token;
    int i = 1;
    
    token = lexer.peek(i);
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        i = i+1;
        token = lexer.peek(i);
        token.Print();
    }
   
    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }

}
void Parser::parse_input(){
    //input -> program inputs
	parse_program();
	parse_inputs();
	expect(END_OF_FILE);	//END OF THE FILE FINALLY
}
// *** program *** //
void Parser::parse_program(){
    //program -> poly_decl_section start
	parse_poly_decl_section();
	parse_start();
}
// ** poly declaration parsing *** //
void Parser::parse_poly_decl_section(){
    //poly_decl_section -> poly_decl
    //poly_decl_section -> poly_decl poly_decl_section
	parse_poly_decl();
	
	Token t1 = lexer.peek(1);
	if(t1.token_type == POLY){
    		//poly_decl_section -> poly_decl poly_decl_section
		parse_poly_decl_section();
	}else if(t1.token_type == START){
    		//poly_decl_section -> poly_decl
	}else syntax_error();
}
void Parser::parse_poly_decl(){
	//poly_decl -> POLY polynomial_header EQUAL polynomial_body SEMICOLON
	expect(POLY);
	parse_polynomial_header();
	expect(EQUAL);
	parse_polynomial_body();
	expect(SEMICOLON);
}
void Parser::parse_polynomial_header(){
    //polynomial_header -> polynomial_name
    //polynomial_header -> polynomial_name LPAREN id_list RPAREN
	parse_polynomial_name();

	Token t1 = lexer.peek(1);
	if(t1.token_type == LPAREN){
    		//polynomial_header -> polynomial_name LPAREN id_list RPAREN
		expect(LPAREN);
		parse_id_list();
		expect(RPAREN);
	}else if(t1.token_type == EQUAL){
    		//polynomial_header -> polynomial_name
	}else syntax_error();
}
void Parser::parse_id_list(){
    //id_list -> ID
    //id_list -> ID COMMA id_list
	expect(ID);

	Token t1 = lexer.peek(1);
	if(t1.token_type == COMMA){
   		//id_list -> ID COMMA id_list
		expect(COMMA);
		parse_id_list();
	}else if(t1.token_type == RPAREN){
    		//id_list -> ID
	}else syntax_error();
}
void Parser::parse_polynomial_name(){
    //polynomial_name -> ID
	expect(ID);
}
void Parser::parse_polynomial_body(){
    //polynomial_body -> term_list
	parse_term_list();
}
void Parser::parse_term_list(){
    //term_list -> term
    //term_list -> term add_operator term_list
	parse_term();

	Token t1 = lexer.peek(1);
	if(t1.token_type == PLUS || t1.token_type == MINUS){
    		//term_list -> term add_operator term_list
		parse_add_operator();
		parse_term_list();
	}else if(t1.token_type == SEMICOLON){
    		//term_list -> term
	}else syntax_error();
	


}
void Parser::parse_term(){
    //term -> monomial_list
    //term -> coefficient monomial_list
    //term -> coefficient
	
	Token t1 = lexer.peek(1);
	if(t1.token_type == NUM){
    		//term -> coefficient monomial_list
    		//term -> coefficient
		parse_coefficient();

		Token t2 = lexer.peek(1);
		if(t2.token_type == ID){
    			//term -> coefficient monomial_list
			parse_monomial_list();
		}else if(t2.token_type == PLUS || t2.token_type == MINUS || t2.token_type == SEMICOLON){
    			//term -> coefficient
		}else syntax_error();
	}else if(t1.token_type == ID){
    		//term -> monomial_list
		parse_monomial_list();
	}else syntax_error();
	
}
void Parser::parse_monomial_list(){
    //monomial_list -> monomial
    //monomial_list -> monomial monomial_list
	parse_monomial();

	Token t1 = lexer.peek(1);
	if(t1.token_type == ID){
    		//monomial_list -> monomial monomial_list
		parse_monomial_list();
	}else if(t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == SEMICOLON){
    		//monomial_list -> monomial
	}else syntax_error();
}
void Parser::parse_monomial(){
    //monomial -> ID
    //monomial -> ID exponent
	expect(ID);

	Token t1 = lexer.peek(1);
	if(t1.token_type == POWER){
    		//monomial -> ID exponent
		parse_exponent();
	}else if(t1.token_type == ID || t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == SEMICOLON){
    		//monomial -> ID
	}else syntax_error();
}
void Parser::parse_exponent(){
    //exponent -> POWER NUM
	expect(POWER);
	expect(NUM);
}
void Parser::parse_add_operator(){
    //add_operator -> PLUS
    //add_operator -> MINUS
	Token t1 = lexer.peek(1);
	if(t1.token_type == PLUS){
    		//add_operator -> PLUS
		expect(PLUS);
	}else if(t1.token_type == MINUS){
    		//add_operator -> MINUS
		expect(MINUS);
	}else syntax_error();
}
void Parser::parse_coefficient(){
    //coefficient -> NUM
	expect(NUM);
}

// ** start parsing ** //
void Parser::parse_start(){
    //start -> START statement_list
	expect(START);
	parse_statement_list();
}
// ** inputs parsing ** //
void Parser::parse_inputs(){
    //inputs -> NUM
    //inputs -> NUM inputs
	expect(NUM);

	Token t1 = lexer.peek(1);
	if(t1.token_type == NUM){
    		//inputs -> NUM inputs
		parse_inputs();
	}else if(t1.token_type == END_OF_FILE){
    		//inputs -> NUM
	}else syntax_error();
}
void Parser::parse_statement_list(){
    //statement_list -> statement
    //statement_list -> statement statement_list
	parse_statement();

	Token t1 = lexer.peek(1);
	if(t1.token_type == ID || t1.token_type == INPUT){
    		//statement_list -> statement statement_list
		parse_statement_list();
	}else if(t1.token_type == NUM){
    		//statement_list -> statement
	}else syntax_error();
}
void Parser::parse_statement(){
    //statement -> input_statement
    //statement -> poly_evaluation_statement
	
	Token t1 = lexer.peek(1);
	if(t1.token_type == INPUT){
    		//statement -> input_statement
		parse_input_statement();
	}else if(t1.token_type == ID){
    		//statement -> poly_evaluation_statement
		parse_poly_evaluation_statement();
	}else syntax_error();
}
void Parser::parse_poly_evaluation_statement(){
    //poly_evaluation_statement -> polynomial_evaluation SEMICOLON
	parse_polynomial_evaluation();
	expect(SEMICOLON);
}
void Parser::parse_input_statement(){
    //input_statement -> INPUT ID SEMICOLON
	expect(INPUT);
	expect(ID);
	expect(SEMICOLON);
}
void Parser::parse_polynomial_evaluation(){
    //polynomial_evaluation -> polynomial_name LPAREN argument_list RPAREN
	parse_polynomial_name();
	expect(LPAREN);
	parse_argument_list();
	expect(RPAREN);
}
void Parser::parse_argument_list(){
    //argument_list -> argument
    //argument_list -> argument COMMA argument_list
	parse_argument();

	Token t1 = lexer.peek(1);
	if(t1.token_type == COMMA){
    		//argument_list -> argument COMMA argument_list
		expect(COMMA);
		parse_argument_list();
	}else if(t1.token_type == RPAREN){
    		//argument_list -> argument
	}else syntax_error();
}
void Parser::parse_argument(){
    //argument -> ID
    //argument -> NUM
    //argument -> polynomial_evaluation
    
	Token t1 = lexer.peek(1);
	Token t2 = lexer.peek(2);
	if((t1.token_type == ID && t2.token_type == COMMA) 
			|| (t1.token_type == ID && t2.token_type == RPAREN)){
    		//argument -> ID
		expect(ID);
	}else if(t1.token_type == ID && t2.token_type == LPAREN){
    		//argument -> polynomial_evaluation
		parse_polynomial_evaluation();
	}else if(t1.token_type == NUM){
    		//argument -> NUM
		expect(NUM);
	}else syntax_error();
}
int main()
{
	// note: the parser class has a lexer object instantiated in it. You should not be declaring
    // a separate lexer object. You can access the lexer object in the parser functions as shown in the
    // example method Parser::ConsumeAllInput
    // If you declare another lexer object, lexical analysis will not work correctly
    	Parser parser;
	parser.parse_input();
}
