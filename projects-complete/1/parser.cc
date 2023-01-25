/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */

// ******* 	NAME IS BELOW VVVV  ******* //
//Name: David Zalewski
//ID: 1215324552
//ASURITE: dzalews1
//
// **************************************** //

#include <iostream>
#include <cstdlib>
#include <vector>
#include "parser.h"
#include "reg.h"

using namespace std;

vector<struct REG_EXPR> regex_list;	//global list for storing respective REG's and Token names
string input_text;			//INPUT_TEXT from the parser
bool epsilon_error = false;		//true if the match method results in an "Epislon is not a token!!!" case 
void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
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

//	******************* 			TO REMEMBER:		 ***************	//
//	(enum) TokenType = { END_OF_FILE = 0,LPAREN, RPAREN, HASH, ID, COMMA, DOT, STAR, OR, UNDERSCORE, SYMBOL, CHAR, INPUT_TEXT, ERROR} 	//

/*  FOR REFERENCE, DELETE LATER!
void Parser::parse_expr() //parses regular expression and adds to a REG graph
{
 	//expr -> CHAR
	//expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
	//expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
	//expr -> LPAREN expr RPAREN STAR
	//expr -> UNDERSCORE
	Token t = lexer.peek(1);
	if(t.token_type == CHAR){
		parsed_tokens.push_back(expect(CHAR));
	}else if(t.token_type == LPAREN){
		parsed_tokens.push_back(expect(LPAREN));	//LPAREN to parsed_tokens
		parse_expr();
		parsed_tokens.push_back(expect(RPAREN));	//RPAREN to parsed_tokens
		Token r = lexer.peek(1);	//another token to peek and see if the next token is DOT, OR, or STAR
		if(r.token_type == DOT || r.token_type == OR){
			parsed_tokens.push_back(expect(r.token_type));	//expects r.token_type (either DOT or OR) and adds to parsed_tokens vector
			parsed_tokens.push_back(expect(LPAREN));	//LPAREN to parsed_tokens
			parse_expr();
			parsed_tokens.push_back(expect(RPAREN));	//RPAREN to parsed_tokens
		}else if(r.token_type == STAR){
			parsed_tokens.push_back(expect(STAR));
		}else syntax_error();
	}
}
*/

struct REG* Parser::parse_expr() //parses regular expression and adds to a REG graph
{
 	//expr -> CHAR
	//expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
	//expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
	//expr -> LPAREN expr RPAREN STAR
	//expr -> UNDERSCORE
	Token t = lexer.peek(1);
	if(t.token_type == CHAR){		//base case
		Token tok = expect(CHAR);	//CHAR  token
		//	creating REG >S--'a'-->A
		 // 	where S is the starting node, A is the end node, and 'a' is the arrow label
		struct REG* my_REG = new struct REG();
		struct REG_NODE* start = new struct REG_NODE();		//new starting node
		struct REG_NODE* accept = new struct REG_NODE();	//new accepting node

		start->first_label = tok.lexeme[0];	//sets arrow label
		start->first_neighbor = accept;	//start points an arrow to node 'accept';	start--->accept

		//	sets my_REG start and accept node pointers
		my_REG->start = start;	
		my_REG->accept = accept;
		return my_REG;
	}else if(t.token_type == UNDERSCORE){
		Token tok = expect(UNDERSCORE);	//UNDERSCORE token
		//	creating REG >S--'_'-->A
		 // 	where S is the starting node, A is the end node, and '_' is the arrow label
		struct REG* my_REG = new struct REG();
		struct REG_NODE* start = new struct REG_NODE();		//new starting node
		struct REG_NODE* accept = new struct REG_NODE();	//new accepting node

		start->first_label = '_';	//sets arrow label
		start->first_neighbor = accept;	//start points an arrow to node 'accept';	start--->accept

		//	sets my_REG start and accept node pointers
		my_REG->start = start;	
		my_REG->accept = accept;
		return my_REG;
	
	
	
	}else if(t.token_type == LPAREN){
		expect(LPAREN);	//LPAREN to parsed_tokens
		struct REG* R1 = parse_expr();			//parses expr inside parentheses
		expect(RPAREN);	//RPAREN to parsed_tokens

		Token r = lexer.peek(1);	//another token to peek and see if the next token is DOT, OR, or STAR
		if(r.token_type == DOT){
			//using parsed REG R1, parses R2 so that  R1--'_'-->R2  where R1 is a REG, '_' is the arrow label, R2 is a REG;
			//The arrow start's from the accept state of R1 nad and ends at the start state of R2
			//returns new REG as my_REG
			expect(DOT);	//expects DOT token_type

			expect(LPAREN);	//LPAREN expect
			struct REG* R2 = parse_expr();			//parsing of R2
			expect(RPAREN);	//RPAREN expect
			
			struct REG* my_REG = new struct REG();
			my_REG->start = R1->start;	//start state is the start of R1
			my_REG->accept = R2->accept;	//accept state is accept of R2

			R1->accept->first_neighbor = R2->start;		//creates arrow from R1 accept state to R2 start state
			R1->accept->first_label = '_';			//sets arrow label to epsilon ('_')
			return my_REG;
		}else if(r.token_type == OR){
			//using parsed REG R1, parses R2 so that  S--'_'-->R1-'_'-->A (and) S--'_'-->R2--'_'-->A   
			//where S is the starting state, R1 is a REG, R2 is a REG, '_' are the arrow labels (epsilon), and A is the accepting state
			//The arrow start's from a new start state and goes to the start state of R1 and R2 (with label epsilon). 
			//Another arrow starts from the accept state of both R1 and R2 and ends to a single, new accept state
			//returns new REG as my_REG
			expect(OR);	//expects OR token_type

			expect(LPAREN);	//LPAREN expect
			struct REG* R2 = parse_expr();			//parsing of R2
			expect(RPAREN);	//RPAREN expect
			
			struct REG* my_REG = new struct REG();
			//	new start and accept states for my_REG		//
			struct REG_NODE* start = new struct REG_NODE();
			struct REG_NODE* accept = new struct REG_NODE();
			start->first_neighbor = R1->start;	//first arrow goes to R1's start state
			start->first_label = '_';		//first arrow label
			start->second_neighbor = R2->start;	//second arrow goes to R2's start state
			start->second_label = '_';		
			
			//R1 and R2 accept states have arrows that go to the new accept state
			R1->accept->first_neighbor = accept;	//first arrow goes to accept state
			R1->accept->first_label = '_';
			R2->accept->first_neighbor = accept;	//first arrow goes to accept state
			R2->accept->first_label = '_';
			
			//	sets my_REG start and accept states	// 
			my_REG->start = start;	
			my_REG->accept = accept;	
			return my_REG;
		}else if(r.token_type == STAR){
			//using only the parsed R1 and a new start and accept state , creates my_REG with the following arrows:
			// S--'_'-->R1_S
			// R1_A--'_'-->R1_S
			// R1_A--'_'-->A
			// S--'_'-->A
			// where S is the new start state, A is the new accept state, R1_S is the start state of R1, and R1_A is the accept state of R1
			expect(STAR);	//expect STAR token_type
			struct REG* my_REG = new struct REG();		//final REG to return

			//	new start and accept states for my_REG		//
			struct REG_NODE* start = new struct REG_NODE();
			struct REG_NODE* accept = new struct REG_NODE();

			// S--'_'-->R1_S
			start->first_neighbor = R1->start;
			start->first_label = '_';

			// R1_A--'_'-->A
			R1->accept->first_neighbor = accept;
			R1->accept->first_label = '_';

			// R1_A--'_'-->R1_S
			R1->accept->second_neighbor = R1->start;
			R1->accept->second_label = '_';

			// S--'_'-->A
			start->second_neighbor = accept;
			start->second_label = '_';

			//	sets my_REG start and accept states	// 
			my_REG->start = start;	
			my_REG->accept = accept;	
			return my_REG;
		}else syntax_error();
	}else syntax_error();
}


void Parser::parse_token()
{ 
	//token -> ID expr
	string name = expect(ID).lexeme;	//parsed name of token
	struct REG* new_REG = parse_expr();	//parsed regex into REG
//	printREG(new_REG);	//DEBUGGING TOOL! prints a sample string of a REG

	struct REG_EXPR reg_expr = { .token_name = name,			//adds struct with token_name and my_REG elements into a global list
					.my_REG = new_REG};
	regex_list.push_back(reg_expr);		//pushes reg_expr to back of list
	
}

void Parser::parse_token_list()
{
	//token_list -> token
	//token_list -> token COMMA token_list
	parse_token();
	Token t = lexer.peek(1);
	if(t.token_type == COMMA){
		expect(COMMA);		//expects comma 
		parse_token_list();
	}else if(t.token_type == HASH){
		//if next token is hash, reached the end of token_list, return
	}else syntax_error();
}

void Parser::parse_tokens_section()
{
	//tokens_section -> token_list HASH
	parse_token_list();		//parses token_list
	Token t = lexer.peek(1);	
	expect(HASH);		//expects HASH 
}

void Parser::parse_input()
{
	//input -> tokens_section INPUT_TEXT
	parse_tokens_section();		//parses tokens_section (non terminal)
	string input_text_with_quotes = expect(INPUT_TEXT).lexeme;	//expects INPUT_TEXT token, then stores its lexeme; (INPUT_TEXT has quotes, must remove!) ex: input_text_with_quotes = "aaabbcaaaa"

	input_text = input_text_with_quotes.substr(1,input_text_with_quotes.length() - 2);		// INPUT_TEXT lexeme but without quotes ex: input_text = aaabbcaaaa; stores into global var input_text
	expect(END_OF_FILE);		//expects EOF
}

// This function simply reads and prints all tokens
// I included it as an example. You should compile the provided code
// as it is and then run ./a.out < tests/test0.txt to see what this function does

void Parser::readAndPrintAllInput()
{
    Token t;

    // get a token
    t = lexer.GetToken();

    // while end of input is not reached
    while (t.token_type != END_OF_FILE) 
    {
        t.Print();         	// pringt token
        t = lexer.GetToken();	// and get another one
    }    
    // note that you should use END_OF_FILE and not EOF
}
// *** function for DEBUGGING! ***
// Prints out the contents of a REG using recursion
// currentNode = node printREG is currently trying to print contents of
void printREG(struct REG* my_REG){
	struct REG_NODE* currentNode = my_REG->start;

	cout << "Creating a string (first neighbor prioritized)" << endl;

	//first neighbor prioritized ex: a node has a first and second neighbor, take the first neighbor path
	for(int i = 0; i < 20; i++){
		if(currentNode->first_neighbor != NULL){
			cout<< currentNode->first_label << " ";
			currentNode = currentNode->first_neighbor;	
		}else if(currentNode->second_neighbor != NULL){
			cout<< currentNode->second_label << " ";
			currentNode = currentNode->second_neighbor;	
		}
	}
	cout <<endl;

	currentNode = my_REG->start;

	cout << "Creating a string (second neighbor prioritized)" << endl;

	//second neighbor prioritized ex: a node has a first neighbor and a second neighbor, take the second neighbor path
	for(int i = 0; i < 20; i++){
		if(currentNode->second_neighbor != NULL){
			cout<< currentNode->second_label << " ";
			currentNode = currentNode->second_neighbor;	
		}else if(currentNode->first_neighbor != NULL){
			cout<< currentNode->first_label << " ";
			currentNode = currentNode->first_neighbor;	
		}
	}
	cout <<endl;
}

//	***DEBUGGING TOOL***
void printMatchOneChar(){	//prints a set of nodes result from Match_One_Char method
	vector<struct REG_NODE*> S;
	
	struct REG* my_reg = regex_list.front().my_REG;
	S.push_back(my_reg->start);
	S = Match_One_Char(S,'_');
	S.push_back(my_reg->start);
	cout << "Vector printing out labels: (size: " << S.size() << " )" << endl;
	for(int i = 0; i < S.size();i++){
		cout << "From a node: " << endl;
		if(S.at(i)->first_neighbor != NULL){
			cout << "N -- " << S.at(i)->first_label << "-->N" << endl;
		}
		if(S.at(i)->second_neighbor != NULL){
			cout << "N -- " << S.at(i)->second_label << "-->N" << endl;
		}
	}
	cout << endl;
	S = Match_One_Char(S,'a');
	cout << "Vector printing out labels: (size: " << S.size() << " )" << endl;
	for(int i = 0; i < S.size();i++){
		cout << "From a node: " << endl;
		if(S.at(i)->first_neighbor != NULL){
			cout << "N -- " << S.at(i)->first_label << "-->N" << endl;
		}
		if(S.at(i)->second_neighbor != NULL){
			cout << "N -- " << S.at(i)->second_label << "-->N" << endl;
		}
	}

}

//checks to see if node is in set S; true if in set, false if not in set
bool isNodeInSet(vector<struct REG_NODE*> set, struct REG_NODE* node){
	vector<struct REG_NODE*>::iterator set_iter = set.begin();	//iterator that starts at beginning of vector set
	bool foundNode = false;	//true if found node, false if not
	
	for(set_iter;set_iter != set.end();set_iter++){			//for every node in set
		if(*(set_iter) == node){			//if the node at the iterators current position is equal to the input node
			foundNode = true;		//foundNode is true
		}
	}
	return foundNode;
}

vector<struct REG_NODE*> Match_One_Char(vector<struct REG_NODE*> S, char c){
	// S is a set of input nodes
	// c is a char to consume & match
	//
	// ***1. Finding all nodes that can be reached from S by consuming c***
	
	vector<struct REG_NODE*> S_new;	//set of nodes that can be reached from S by consuming c
	
	vector<struct REG_NODE*>::iterator S_iter = S.begin();	//iterator that starts at beginning of vector S
	for(S_iter;S_iter != S.end(); S_iter++){	// for every node in S
		struct REG_NODE* n = *S_iter;		//node n at current position of the iterator
		struct REG_NODE* m;		
		if(n->first_neighbor != NULL){ //n has a first_neighbor (an arrow at another node)
			m = n->first_neighbor;		//set m to be first_neighbor
			if(n->first_label == c && !isNodeInSet(S_new, m)){	//if the label from arrow n-->m is equal to the input char c , AND m is not in S' 
				S_new.push_back(m);	//add m to S'
			}	
		}
		if(n->second_neighbor != NULL){ //n has a second_neighbor (an arrow at another node)
			m = n->second_neighbor;		//set m to be second_neighbor
			if(n->second_label == c && !isNodeInSet(S_new, m)){	//if the label from arrow n-->m is equal to the input char c , AND m is not in S' 
				S_new.push_back(m);	//add m to S'
			}	
		}
	}
	if(S_new.empty()){
		return S_new;	//if S' is empty, return the empty set
	}

	//S_new is NOT empty, continue:
	// ***2. Finding all nodes that can be reached from S' by consuming NO input (epsilon)***
	
	bool changed = true;	
	vector<struct REG_NODE*> S_new_new;	//new empty set, S''
	while(changed){
		changed = false;	//once something has changed, changed = true
		vector<struct REG_NODE*>::iterator S_iter = S_new.begin();	//iterator that starts at beginning of vector S' (the result set from step 1)
		for(S_iter;S_iter != S_new.end(); S_iter++){	// for every node in S''
			struct REG_NODE* n = *S_iter;		//node n at current position of the iterator

			if(!isNodeInSet(S_new_new,n)){		//if n isn't already in S''
				S_new_new.push_back(n);			//adds n to S''
			}

			struct REG_NODE* m;
			// *for every neighbor m of n*
			if(n->first_neighbor != NULL){ //n has a first_neighbor (an arrow at another node)
				m = n->first_neighbor;		//set m to be first_neighbor
				bool isInNode = isNodeInSet(S_new_new,m);
				if(n->first_label == '_'  && !isInNode){	//if the label from arrow n-->m is equal to '_' , AND m is not in S'' 
					S_new_new.push_back(m);	//add m to S''
				}	
			}
			if(n->second_neighbor != NULL){ //n has a second_neighbor (an arrow at another node)
				m = n->second_neighbor;		//set m to be second_neighbor
				bool isInNode = isNodeInSet(S_new_new,m);
				if(n->second_label == '_' && !isInNode){	//if the label from arrow n-->m is equal to '_' , AND m is not in S'' 
					S_new_new.push_back(m);	//add m to S''
				}	
			}
		}//for loop end
		
		if(S_new != S_new_new){		//S' != S'', 
			changed = true;		//changed, continue loop until S_new == S_new_new
			S_new = S_new_new;
			S_new_new.clear();
		}



	}//while loop end
	return S_new;

}//method end

string match(struct REG* r, string s, int p){

	vector<struct REG_NODE*> set_of_nodes;	//set of nodes

	//	***creating set of nodes that can be reached w/o input***	///
	set_of_nodes.push_back(r->start);	//adds start node to set of nodes
	//for(int i = 0;i<set_of_nodes.size();i++){
	//	cout << "\"" <<  set_of_nodes.at(i)->first_neighbor->first_label << "\""<< endl;
	//}
	set_of_nodes = Match_One_Char(set_of_nodes,'_');	//first pass of Match_One_Char, results in set of nodes that can be reached from starting node of r by consuming no input (epsilon)
	
	set_of_nodes.push_back(r->start);	//adds start node back to set of nodes
	if(isNodeInSet(set_of_nodes,r->accept)){		//if set of nodes contains the accept node, an empty string can be created, so print "EPISLON NOT TOKEN!!" the tokens that are causing error, and exit
		return "EPSILONERROR";	//returns epsilon error
	}
	//	*** set_of_nodes = nodes that can be reached w/o input ***	///


	string longestMatchingPrefix = "";	//the longest matching prefix found; Increases if a character matches
	string viablePrefix = "";		//string that did not find r accept node, but is still viable

	while(!set_of_nodes.empty() &&  p < s.length()){	//while set_of_nodes isn't empty AND p is less than the size of the string
		char charAtP = s.at(p);		//the current char at pa
		viablePrefix += charAtP;	//viableprefix increases; 
		set_of_nodes = Match_One_Char(set_of_nodes, charAtP);	//match call

		if(isNodeInSet(set_of_nodes,r->accept)){		//if one of the nodes in set_of_nodes is the REG accept node, then the char is MATCHING
			if(viablePrefix != longestMatchingPrefix){	//the set of nodes HAS the accept node so it is MATCHING, but viable prefix has char's that aren't currently in longest matching
				longestMatchingPrefix = viablePrefix;		//adds missing chars that were viable before
				
			}else{
				longestMatchingPrefix += charAtP;	//adds char to the end of the longestMatchingPrefix
			}
		}
		p++;
	}
	return longestMatchingPrefix;
}



int main()
{
    	Parser parser;
	parser.parse_input();

	my_LexicalAnalyzer lex = {regex_list, input_text};

}
