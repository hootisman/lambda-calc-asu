
// ******* 	NAME IS BELOW VVVV  ******* //
//Name: David Zalewski
//ID: 1215324552
//ASURITE: dzalews1
//



#include "reg.h"
#include <iostream>

my_LexicalAnalyzer::my_LexicalAnalyzer(std::vector<struct REG_EXPR> new_list, std::string new_s){
	list = new_list;
	s = new_s;
	p = 0;
	
	epsilonCheck();		//checks if an REG's can output to empty string from start node
	// *** Below runs my_GetToken repeatedly until the entire string is parsed *** //
	while(p < s.length()){	//if p hasn't reached the end of the string yet
		if(isspace(s.at(p))){			//if at the position p of string there is a space, we SKIP the space instead of running my_GetToken 
			if(p + 1 >= s.length()){		//if incrementing position ends the string, we don't increment and instead break
				break;
			}else{
				p++;				//increment position so we are no longer at the space char
			}
		}else{
			std::string result = my_GetToken();		//the result from my_GetToken()
			std::cout << result << std::endl;		//prints regularly
		}
	}
}

std::string my_LexicalAnalyzer::my_GetToken(){	//returns token_name and lexeme in this format: "t1 , "a"" where 't1' is the token_name and 'a' is the lexeme
	std::vector<std::string> match_list;	//new array that stores the longest matching string in the index token is located;
	//ex: "t1 (a)|(b)" is in index 3 of list, match_list stores result of "t1 (a)|(b)"'s match function into index 3 of match_list

	struct REG* r;
	for(int i = 0; i < list.size(); i++){	//for every token and REG in the list
		r = (list.at(i)).my_REG;		//r is the REG at i of the list
		std::string match_result = match(r,s,p);	//result from match method

		match_list.push_back(match_result);			//stores the current REG's longest matching string

	}

	std::string longest_match = match_list.at(list.size() - 1);	//longest matching prefix (TEMPORARILY) , starts at the end of list
	int longest_match_index = list.size() - 1;			//longest matching prefix index
	for(int j = list.size() - 1; j >= 0; j--){		//for loop starts at the end of list
		if(longest_match.length() <= match_list.at(j).length()){		//if the current longest_match length is less than another matching length, the other matching length is new longest
			//std::cout << "AT TOKEN: " << (list.at(j)).token_name << " LONGEST MATCHING IS " << match_list.at(j) << std::endl;
			longest_match = match_list.at(j);
			longest_match_index = j;		//sets index
		}	
	}


	if(longest_match.length() == 0){			//no match was found, so we print "ERROR" and exit
		std::cout << "ERROR" << std::endl;		
		exit(1);
	}else{
		p = p + longest_match.length();		//add the length of longest match to p, so if my_GetToken() is looped the next pass will start further in string;

	}
	
	//ex: s = "aabazef" p = 0, longest_match is "aaba" so add p such that p = 4 and will start at 'z' of "zef"
	
	std::string result = (list.at(longest_match_index)).token_name;		//result = token name (of longest matching prefix's token)
	result += " , \"";
	result += longest_match;
	result += "\"";
	return result;				//final string in format: "token_name , lexeme"
}
void my_LexicalAnalyzer::epsilonCheck(){ 		//iterates through all token REG's to find if any can create an empty string. If yes, prints "EPSILON NOT TOKEN" then exits

	struct REG* r;
	std::vector<std::string> tokens_with_epsilon;	//vector that stores the token_name's that can generate an empty string from their REG start node


	//std::cout << "LOOKING AT TOKEN NAME: " << (list.at(0)).token_name << std::endl;
	for(int i = 0; i < list.size(); i++){	//for every token and REG in the list
		r = (list.at(i)).my_REG;		//r is the REG at i of the list
		std::string match_result = match(r,s,p);	//result from match method
	//	std::cout << "MATCH RESULT : " << match_result << std::endl;
		if(match_result == "EPSILONERROR" ){	//the current token can generate an empty string from its start node

			tokens_with_epsilon.push_back( (list.at(i)).token_name );//adds current token_name to vector of tokens that produce epsilon error

		}
	}

	if(tokens_with_epsilon.size() != 0){	//if the vector contains elements, then it has found tokens that produce an epsilon error
		std::cout << "EPSILON IS NOOOOOOT A TOKEN !!!";
		for(int i = 0;i < tokens_with_epsilon.size(); i++){	//prints out all token names that produce epsilon error
			std::cout << " " << tokens_with_epsilon.at(i);		//prints token_name at index i of the vector, and adds a space character to the end
		}
		std::cout << std::endl;
		exit(1);
	}

}
