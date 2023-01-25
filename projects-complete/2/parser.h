
/* Name: David Zalewski
 * ASURITE: dzalews1
 * ID: 1215324552
 * Date: 10/20/21
 *
 */
#include <vector>
#include "lexer.h"
#ifndef _PARSERH_
#define _PARSERH_

typedef struct {		//struct used to create organized grammar rules based on input
	std::string LHS;	//left hand side of rule (has all non-terminals)
	std::vector<std::string> RHS;	//right hand side of rule (has combination of terminals + non terminals)
} Rule;

class GrammarParser{
	public:
		std::vector<Rule*> all_rules;		//end result; all rules parsed from the input
		std::vector<std::string> grammarUniverse;	//contains ALL symbols from grammar including '#' (epsilon) and '$' (EOF)
		void parse_input();			//parses "Grammar -> Rule-List HASH
	private:
		LexicalAnalyzer lex;			//analyzer for GetToken()
		Token expect(TokenType);		
		void syntax_error();			//method to print syntax error

		/* helper functions to parse through input using given grammar */
		void parse_idlist(Rule*);	//takes a Rule* as an argument, appends to the RHS vector an ID 
		void parse_rhs(Rule*);		//right hand side, takes a Rule* with only a LHS as an argument, if no RHS is found, appends # representing epsilon to RHS of Rule*
		void parse_rule();
		void parse_rulelist();
		void parse_grammar();
};
#endif
