
/* Name: David Zalewski
 * ASURITE: dzalews1
 * ID: 1215324552
 * Date: 10/20/21
 *
 */
#include "parser.h"
#include <iostream>
#include <algorithm>
Token GrammarParser::expect(TokenType expectedResult){		//method for parsing a terminal character
	Token tok = lex.GetToken();
	if(tok.token_type != expectedResult){
		syntax_error();
	}else return tok;
}

void GrammarParser::syntax_error(){
	std::cout << "SYNTAX ERROR !!!\n";
	exit(0);
}
void GrammarParser::parse_input(){
	parse_grammar();
	expect(END_OF_FILE);
}
void GrammarParser::parse_grammar(){
	parse_rulelist();
	expect(HASH);
}
void GrammarParser::parse_rulelist(){
	parse_rule();
	Token tok = lex.peek(1);
	if(tok.token_type == ID){	//peeks to next token, if it is an ID then there is another Rule-List
		parse_rulelist();
	}else if(tok.token_type == HASH){	//peeks to next token, if it is a HASH then it has reached all the rules and the program can end
		//do nothing, returns to parse_grammar();
	}else syntax_error();
}
void GrammarParser::parse_idlist(Rule* new_rule){
	Token RHS_token = expect(ID);
	new_rule->RHS.push_back(RHS_token.lexeme);	//appends expected ID into RHS of new_rule
	
	if(std::find(grammarUniverse.begin(), grammarUniverse.end(), RHS_token.lexeme) == grammarUniverse.end()){	// if terminal is NOT in grammar universe
		grammarUniverse.push_back(RHS_token.lexeme);	//adds terminal to grammarUniverse
	}

	Token tok = lex.peek(1);
	if(tok.token_type == ID){	//peeking, if next token is an ID then there is more idlist, parse another idlist
		parse_idlist(new_rule);
	}else if(tok.token_type == STAR){	//peeking, if next token is STAR, reached end of current rule, do nothing
		//do nothing, goes back to parse_rhs()
	}
}
void GrammarParser::parse_rule(){
	Rule* new_rule = new Rule();		//creates a new rule
	Token LHS_token = expect(ID);		//gets LHS token
	new_rule->LHS = LHS_token.lexeme;	//adds LHS to the new rule

	if(std::find(grammarUniverse.begin(), grammarUniverse.end(), LHS_token.lexeme) == grammarUniverse.end()){	// if non terminal is NOT in grammar universe
		grammarUniverse.push_back(LHS_token.lexeme);	//adds non-terminal to grammarUnivese
	}

	expect(ARROW);

	parse_rhs(new_rule);	//parses RHS and adds RHS to the new rule
	expect(STAR);
}
void GrammarParser::parse_rhs(Rule* new_rule){
	Token tok = lex.peek(1);
	if(tok.token_type == ID){	//peeking, if next token is ID then there is an ID list, so parse it
		parse_idlist(new_rule);
	}else if(tok.token_type == STAR){	//peeking, if next token is STAR, token is epsilon, so append # for epsilon to RHS vector
		new_rule->RHS.push_back("#");	//adds epsilon to RHS
	}else syntax_error();

	all_rules.push_back(new_rule);		//adds the Rule* to the all_rules public vector
}
