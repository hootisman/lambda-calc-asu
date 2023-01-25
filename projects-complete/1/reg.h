
// ******* 	NAME IS BELOW VVVV  ******* //
//Name: David Zalewski
//ID: 1215324552
//ASURITE: dzalews1
//

#ifndef __REG_H__
#define __REG_H__
#include <string>
#include "lexer.h"


// **** ALL USED IN "parser.cc"  **** //
struct REG_NODE{  //REG Node as described in the project PDF
	int label;	//label of node corresponding to when it was created in REG. Ex: 1 means it was first created, 2 means second created, 17 means 17th created
	struct REG_NODE* first_neighbor;
	char first_label;
	struct REG_NODE* second_neighbor;
	char second_label;
};
struct REG{	//REG as described in the project PDF
	struct REG_NODE* start;
	struct REG_NODE* accept;
};
//fully parsed regular expression w/name ex:  {token_name,my_REG} = {"t3",(d)|(e) (in parsed REG)}
struct REG_EXPR{
	std::string token_name;
	struct REG* my_REG;
};
class my_LexicalAnalyzer{	//uses match and 
	public:
		std::vector<struct REG_EXPR> list;	//vector of {token_name,reg_pointer} elements
		std::string s;				//input string
		my_LexicalAnalyzer(std::vector<struct REG_EXPR>, std::string);			//constructor
		std::string my_GetToken();		//returns my_token which contains token_name and lexeme
	private:
		int p;					//current position of string char being processed
		void epsilonCheck();			//if any input REG's can create any empty string from it's start node, exits and prints "EPSILON IS NOT TOKEN"
};
void printREG(struct REG*);	// ***DEBUGGING TOOL*** function for debuging purposes that prints out the contents of a REG
void printMatchOneChar();	// ***DEBUGGING TOOL*** prints a set of nodes result from Match_One_Char method


bool isNodeInSet(std::vector<struct REG_NODE*>, struct REG_NODE*);	//function to check if a node is already in the set of nodes; true if it is in set, false if isn't in set
std::vector<struct REG_NODE*> Match_One_Char(std::vector<struct REG_NODE*>, char);	//for the match method
std::string match(struct REG*,std::string,int);		//returns longest possible lexeme starting from position, p in the string, s that matches the regex of the graph, r
#endif
