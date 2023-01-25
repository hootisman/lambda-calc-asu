/* Name: David Zalewski
 * ASURITE: dzalews1
 * ID: 1215324552
 * Date: 10/20/21
 *
 */



/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <algorithm>
#include "parser.h"
using namespace std;

GrammarParser parser;		//global var, contains all_rules vector of Rule* elements
//	!!!*** parser.grammarUniverse contains the universe of ALL symbols in the grammar!! (nonterms, terms, # and $) ***!!! //

// read grammar
void ReadGrammar()
{
	parser.parse_input();		//parses input, syntax error if not valid

}

// **** DEBUGGING TOOL **** //
template <class T>		//template, works for any vector of class T 
void printVector(vector<T> vec){ 	//prints all elements in the vec for debugging purposes
	for(T element : vec){
		cout << element << " ";
	}
}

// **** DEBUGGING TOOL **** //
template <class T>
void printSet(set<T> vec){ 	//prints all elements in vec for debugging purposes
	for(T element : vec){
		cout << element << " ";
	}
}


bool findInVector(vector<string> vec, string str){		//returns true if found str in vec
	bool result = false;
	for(int i = 0;i < vec.size();i++){
		if(vec.at(i) == str){		//if the element at i is the string
			result = true;		//result is true
		}
	}
	return result;
}
int index(string symbol){	//argument is a symbol, returns index of symbol in grammarUniverse
	int counter = 0;
	for(string element : parser.grammarUniverse){	//loops through all elements of grammarUniverse
		if(element == symbol) return counter;
		counter++;
	}
	return -1; //returns -1 if symbol not found
}
// Task 1
void printTerminalsAndNoneTerminals()
{
    vector<string> terminals_inorder;		//vector of terminals in the order which they appear in the rules
    vector<string> nonTerminals_inorder;	//vector of nonterminals in the order which they appear in the rules

    vector<string> listOfNonTerms;		//vector of all total NonTerms that are in the grammar
    
    for(int i = 0;i < parser.all_rules.size();i++){		//loops through all rules and adds all LHS nonterms to listOfNonTerms
	Rule* currentRule = parser.all_rules[i];
	string ruleLHS = currentRule->LHS;

	if(!findInVector(listOfNonTerms,ruleLHS))		//if ruleLHS is NOT in listOfNonTerms
		listOfNonTerms.push_back(ruleLHS);		//add it to the list
	
    }

    for(int i = 0;i < parser.all_rules.size(); i++){		//searches for nonterminals and places in the order in which they appear
	Rule* currentRule = parser.all_rules[i];

	// **** LHS **** //
	string ruleLHS = currentRule->LHS;			//reference to LHS of the current rule
	if(!findInVector(nonTerminals_inorder,ruleLHS))		//if the current rule LHS non terminal is NOT FOUND in the nonTerminals_inorder vector
		nonTerminals_inorder.push_back(ruleLHS);		//append to vector as the latest non terminal found
	

	// **** RHS **** //
	vector<string> ruleRHS = currentRule->RHS;		//reference to RHS of current rule
	for(int j = 0;j < ruleRHS.size(); j++){			//loops through ALL RHS symbols in the vector
		string currentSymbol = ruleRHS[j];		//current selected symbol from RHS side
		if(currentSymbol != "#"){			//if currentsymbol is NOT epsilon
			if(!findInVector(listOfNonTerms,currentSymbol)){			//if currentSymbol is NOT in listOfNonTerms
				
				//currentsymbol == Terminal OR Epsilon
				if(!findInVector(terminals_inorder,currentSymbol)){		//if current symbol is NOT in the list of found terminals AND is NOT epsilon
					terminals_inorder.push_back(currentSymbol);		//append to vector as the latest terminal found
				}
	
			} else {

				//currentSymbol == Non-Terminal
				if(!findInVector(nonTerminals_inorder,currentSymbol)){		//if current symbol is NOt in the list of found non-terminals
					nonTerminals_inorder.push_back(currentSymbol);		//append to vector as the latest non terminal found
				}
	
			}
		}

	}
	
    }//end of first for loop
    
    // prints terminals, then nonterminals
    printVector(terminals_inorder);
    printVector(nonTerminals_inorder);
    cout << endl;
    
    //end
}

// Task 2
vector<Rule*> FindUselessSymbols() //helper function for RemoveUselessSymbols, returns the vector of rules with generating and reachable symbols 
{
    vector<bool> isGenerating(parser.grammarUniverse.size(),true);	//bool vector of true elements, indices corresponding to indices of grammarUniverse
    
    // ***initialization*** //
    // "#" is TRUE
    isGenerating[1] = false;		//index 1 = "$", false because it is not terminal OR epsilon

    // terminals are TRUE
    // nonterminals are FALSE
    // 
    // The for loop below loops through all terminals and nonterminals and if the symbol is a nonterminal, it's corresponding element at isGenerating = false

    for(int i = 2; i < isGenerating.size(); i++){	//starts at third element of vector (first element is "#", true; second element is "$", false)
	string currentSymbol = parser.grammarUniverse[i];	//current symbol at index i in grammarUniverse; a nonterminal symbol OR terminal symbol

	for(Rule* rule : parser.all_rules){		//for every rule in the parsed grammar
		string nonTerminal = rule->LHS;			//the nonterminal LHS of the current rule
		
		if(currentSymbol == nonTerminal){		//if the currentSymbol is a NonTerminal from the parsed grammar
			isGenerating[i] = false;		//isGenerating at index == false (because it is a nonterminal)
		}
	}
    }
    
    // ***iteration*** ///
	
    // Case 1. Generating Symbols
    // True if generating, false if not generating
    //

    bool changed = true;	//true if generated symbol has changed
    while(changed){
	changed = false;
    
        for(Rule* rule : parser.all_rules){		//for every rule in the grammar		
		bool currentLHSGen = isGenerating[index(rule->LHS)];	//current LHS generating value
		
		if(!currentLHSGen){		//NOT generating, checks RHS to see if LHS is generating
			bool isLHSGenerating = true;		//initially the result is true
			for(string RHS : rule->RHS){	//for each RHS symbol in the rule
				bool currentRHSGen = isGenerating[index(RHS)];	//current RHS generating value

				if(!currentRHSGen){		//if the current rule RHS is NOT generating
					isLHSGenerating = false;
				}
			}
			if(isLHSGenerating){
				changed = true;	//if isLHSGenerating, then the array has changed = true
			}

			isGenerating[index(rule->LHS)] = isLHSGenerating;			//if there is ONE instance of RHS not generating, then equals false; Otherwise, equals true;
		}
        }	//end of for
    } //end of while

    // *** removing non-generating rules *** //
    
    vector<Rule*> RulesGen;	//new rules vector with all generating rules

    for(Rule* rule : parser.all_rules){		//for all rules in grammar
	string ruleLHS = rule->LHS;		//current rule LHS
	bool currentLHSGen = isGenerating[index(ruleLHS)];	//if LHS is generating, true, otherwise false

	if(!currentLHSGen){		//if LHS is NOT generating
		continue;		//continue loop, do NOT add current rule to RulesGen
	}else{
		bool addToRulesGen = true;		//true if current rule will be added to RulesGen, false if will NOT be added to RulesGen
		for(string ruleRHS : rule->RHS){		//for every RHS symbol in current rule
			bool currentRHSGen = isGenerating[index(ruleRHS)];  	//current RHS symbol
			if(!currentRHSGen){		//if the current RHS symbol is NOT generating
				addToRulesGen = false;	//will NOT add current rule to RulesGen
				break;
			}
		}

		if(addToRulesGen){		//true if ALL RHS symbols are generating, false if not
			RulesGen.push_back(rule);	//adds rule to RulesGen	
		}
	}
    }
    
    //  *** iteration *** //
    //Case 2. Calculating reachable symbols
    //true if reachable, false if not

    vector<bool> isReachable(parser.grammarUniverse.size(),false);	//sets ALL symbols to false
    // ****S (starting rule) is always reachable! ****
    
    string startingLHS = parser.all_rules[0]->LHS;		//the LHS of the starting (first) rule (DEBUG: originally RulesGen[0]->LHS)
    isReachable[index(startingLHS)] = true;			//sets LHS symbol of starting rule to true (always reachable)


    changed = true;
    while(changed){
	changed = false;
	for(Rule* rule : RulesGen){		//for each rules in RulesGen
		bool LHSIsReachable = isReachable[index(rule->LHS)];	//true if current LHS is reachable, false if not
		if(LHSIsReachable){		//if LHS is reachable
			for(string RHS : rule->RHS){		//for each symbol in RHS
				bool RHSIsReachable = isReachable[index(RHS)];	// true if current RHS is reachable, false if not
				
				if(!RHSIsReachable){	//if RHSIsReachable is false in the isReachable vector
					isReachable[index(RHS)] = true;		//RHS is reachable
					changed = true;		//isReachable changed
				}
			}
		}
	}//end of for loop
    }//end of while

    // ***removing non reachable rules *** //

    // *** RulesGenAndReach is the FINAL rule vector with no useless symbols!!!!!! *** ///
    vector<Rule*> RulesGenAndReach;	//vector with generating and reachable rules
    
    for(Rule* rule : RulesGen){		//for all rules in generating grammar
	string ruleLHS = rule->LHS;		//current rule LHS
	bool currentLHSGen = isReachable[index(ruleLHS)];	//if LHS is reachable, true, otherwise false

	if(!currentLHSGen){		//if LHS is NOT reachable
		continue;		//continue loop, do NOT add current rule to RulesGenAndReach
	}else{
		bool addToRulesGen = true;		//true if current rule will be added to RulesGenAndReach, false if will NOT be added to RulesGenAndReach
		for(string ruleRHS : rule->RHS){		//for every RHS symbol in current rule
			bool currentRHSGen = isReachable[index(ruleRHS)];  	//current RHS symbol
			if(!currentRHSGen){		//if the current RHS symbol is NOT reachable
				addToRulesGen = false;	//will NOT add current rule to RulesGen
				break;
			}
		}

		if(addToRulesGen){		//true if ALL RHS symbols are generating, false if not
			RulesGenAndReach.push_back(rule);	//adds rule to RulesGen	
		}
	}
    } //end of for

    return RulesGenAndReach;		//returns vector

}

void RemoveUselessSymbols(){	 
    vector<Rule*> RulesNoUseless = FindUselessSymbols();	//returns vector of rules without useless symbols

    // *** print vector *** //
    for(Rule* rule : RulesNoUseless){
	cout << rule->LHS << " ->";	//prints LHS and arrow
	for(string RHS : rule->RHS){	//for each RHS in rule->RHS
		cout << " " << RHS;
	}
	cout << endl;
    }
}


bool isInSet(set<string>* set,string input){	//returns true if input is in set, false if not
	return (*set).count(input) == 1;	//1 if one element is found, 0 if no element is found
}
bool addToSet(set<string>* set, string input){	//adds an input to the set, true if successful
	bool boolResult = false;
	if(!isInSet(set,input)){	//if input is NOT in set
		(*set).insert(input);	//add to set
		boolResult = true;
	}
	return boolResult;
}
bool addFirstSets(set<string>* set_addTo,set<string> set_addFrom){	
	// *** set_addFrom IS A COPIED ARRAY ( pass by value ) ! UNLIKE set_addTo WHICH IS pass by reference !
	bool isElementNotFound = false;		//true if atleast 1 element from set_addFrom was NOT in set_addTo, and was then added
	set_addFrom.erase("#");			//FIRST(...) - {#}

	for(string symbol_addFrom : set_addFrom){
		if(!isInSet(set_addTo,symbol_addFrom)){		//if the current symbol from set_addFrom is NOT in set_addTo
			isElementNotFound = true;		//element was NOT found, so true
		}

	}
	if(isElementNotFound){	//if an element in set_addFrom was NOT found in set_addTo
		(*set_addTo).insert(set_addFrom.begin(),
						set_addFrom.end());		//insert all elements from set_addFrom to set_addTo
	}
	/*
	for(string symbol_addFrom : set_addFrom){		//for every symbol in set_addFrom
		bool hasAdded = addToSet(set_addTo, symbol_addFrom);		//hasAdded is true if SOMETHING was added to set
		if(!boolResult && hasAdded) boolResult = true;	//if boolResult is FALSE and hasAdded is TRUE
	}
	*/
	return isElementNotFound;

}
vector<string> findNTInOrder(set<string> listOfNonTerms_unordered){		//returns the vector with ALL nonterminals in the grammar, in order
	vector<string> listOfNonTerms;		//vector with all nonterminals in order for which they appear in the grammar
	for(Rule* rule : parser.all_rules){
		string LHSSymbol = rule->LHS;
		vector<string> RHS = rule->RHS;
		if(!findInVector(listOfNonTerms,LHSSymbol)){	//if nonterminal is NOT in ordered vector
			listOfNonTerms.push_back(LHSSymbol);
		}
		for(string RHSSymbol : RHS){	//for each element in RHS
			//if the RHS symbol is a non terminal (is in the set of unordered non terminals) AND is NOT in the ordered vector
			if(isInSet(&listOfNonTerms_unordered,RHSSymbol) && !findInVector(listOfNonTerms,RHSSymbol)){	
				//add to the RHS symbol to the ordered vector
				//1. it IS a non terminal
				//2. it is NOT in the vector yet
				listOfNonTerms.push_back(RHSSymbol);
			}
		}
	}

	return listOfNonTerms;
}
vector<string> findFirstInOrder(set<string> firstset_unordered){	//returns vector<string> of the inputed firstset but in the order it appears in the grammar
	vector<string> firstvector;	//vector of current first set sorted

	if(isInSet(&firstset_unordered,"#")){	//if the unordered set contains epsilon
		firstvector.push_back("#");	//add epsilon to the front of the vector
	}	
	for(Rule* rule : parser.all_rules){	
		// **elements of a firstset are just terminals, so no checking rule->LHS**
		for(string RHSSymbol : rule->RHS){
			if(RHSSymbol != "#" && !findInVector(firstvector,RHSSymbol) && isInSet(&firstset_unordered,RHSSymbol)){
				//if:
				//1. The current RHS symbol is NOT epsilon
				//2. AND the current RHS symbol is NOT in the sorted first vector
				//3. AND the current RHS symbol is IN the unordered first set

				firstvector.push_back(RHSSymbol);	//add the symbol to the first vector
			}
		}
	}
	return firstvector;
	
}
void printFirstSets(vector<set<string>>* allFirstSets,set<string>* listOfNonTerms){	//passes vector with ALL first sets and set with ALL non terminals, and prints first sets of non terminals
	vector<string> listOfNonTerms_ordered = findNTInOrder((*listOfNonTerms));	
	//passes set of non terminals and returns the vector of nonterminals in the ORDER for which they appear in the grammar
	
	for(string nonTerminal : listOfNonTerms_ordered){	//for each of the non terminals
		int nonTermIndex = index(nonTerminal);	//index of the non terminal in grammarUniverse
		set<string> firstSet = (*allFirstSets)[nonTermIndex];
		vector<string> firstVector = findFirstInOrder(firstSet);	//returns first set sorted correctly to print
		
		cout << "FIRST(" << nonTerminal << ") = {";
		bool isFirstElement = true;	//true if is first element of the set
		for(string setSymbol : firstVector){	//for each symbol in the set
			if(isFirstElement){
				isFirstElement = false;	
			}else{
				cout << ",";
			}
			cout << " " << setSymbol;
		}
		cout << " }" << endl;
	}
}
// Task 3
vector<set<string>> findFirstSets(){		//returns all first sets of the nonterminals in the grammar
	


	//  *** initialization *** //
	set<string> listOfNonTerms;	//list of ALL non terminals
	for(Rule* rule : parser.all_rules){
		addToSet(&listOfNonTerms,rule->LHS);		//adds LHS to set of nonterminals
	}
	
	vector<set<string>> allFirstSets(parser.grammarUniverse.size());	//vector of size grammarUniverse.size()
	//each index corresponds to a symbol at the same index in grammarUniverse
	
	for(string grammarSymbol : parser.grammarUniverse){	//for each symbol in the grammar

		int symbolIndex = index(grammarSymbol);		//symbol's index in the grammarUniverse and allFirstSets
		if(!isInSet(&listOfNonTerms,grammarSymbol)){		//if grammar symbol is NOT in list of non terms (is NOT a non terminal)
			addToSet(&allFirstSets[symbolIndex],grammarSymbol);	//adds grammar symbol to it's first set
		}
	}


	// *** implementation *** //
	
	bool changed = true;
	while(changed){
		changed = false;
		
		for(string nonTerminal : listOfNonTerms){	//iterates through ALL non terminals (LHS symbolS)
			int NTIndex = index(nonTerminal);	//index of allFirstSets that contains the setSymbol's firstset
			set<string>* NTFirstSet =  &allFirstSets[NTIndex];		//first set of the current nonterminal

			vector<Rule*> symbolRules;		//empty set that contains all rules that has setSymbol as the LHS

			// *** symbolRules initialization *** //
			for(Rule* rule : parser.all_rules){
				if(nonTerminal == rule->LHS){		//if the current set symbol is the rule LHS
					symbolRules.push_back(rule);	//adds the rule to symbolRules
				
					/*	
					cout << rule->LHS << " --> ";	//DEBUG TOOL
					printVector(rule->RHS);		//DEBUG TOOL
					cout << endl;			//DEBUG TOOL
					*/
				}
			}
			



			// *** Rule 3 *** //
			for(Rule* rule : symbolRules){		//for each rule that contains set symbol as the LHS
				string RHSSymbol = rule->RHS[0];	//symbol of the first symbol in rule RHS
				int RHSIndex = index(RHSSymbol);	//index of the first symbol in rule RHS vector
				set<string> RHSFirstSet = allFirstSets[RHSIndex];	//FIRST(RHSSymbol)	(first set of first symbol of RHS)
				
				/*			
				cout << "Symbol: " << RHSSymbol << " ";		//DEBUG TOOL
				printSet(RHSFirstSet);
				cout << endl;

				*/
				if(!isInSet(NTFirstSet,"#") && rule->RHS[0] == "#"){	//if one rule only has "#" (epsilon) on it's RHS, add it to the current nonterminal firstset
					addToSet(NTFirstSet,"#");
					changed = true;
				}
				//adds elements of RHSSet - {#} to NTFirstSet, returns true if ATLEAST 1 element was in RHSFirstSet that was NOT in NTFirstSet
				if( addFirstSets(NTFirstSet, RHSFirstSet)){		//add first sets, if successful, changed = true
					changed = true;
				}
				
				
			}	

			// *** Rule 4 & 5*** //
			for(Rule* rule : symbolRules){		//for each rule that contains the current nonterminal as the LHS
				if(isInSet(&(allFirstSets[index(rule->RHS[0])]), "#")){		//if the first set of the first symbol in rule->RHS has an epsilon, then
					// *** Check for rule 4 & 5 *** //

					bool isRuleFive = true;	//true if the current rule follows Rule 5
					for(string RHSSymbol : rule->RHS){		//for each symbol in the current rule's RHS
						int RHSIndex = index(RHSSymbol);		//index of the current symbol in rule RHS
						set<string> RHSFirstSet = allFirstSets[RHSIndex];	//FIRST(RHSSymbol) 	(first set of first symbol of RHS)
					
						if(!isInSet(&RHSFirstSet, "#")){	//if the current set doesn't contain epsilon then it is Rule 4
							isRuleFive = false;
							if(addFirstSets(NTFirstSet, RHSFirstSet)){	//add first sets, if successful, changed = true
								changed = true;
							}
							break;
						}else if(isInSet(&RHSFirstSet,"#")){		//else if epsilon is in first set
							if(addFirstSets(NTFirstSet, RHSFirstSet)){	//add first sets, if successful, changed = true
								changed = true;
							}
						}
					}	
					//if reached this point and isRuleFive is still true, then adds epislon to the current nonterminal first set
					if(isRuleFive && addToSet(NTFirstSet,"#")){
						changed = true;
					}
				}
			}
			

		}//end of for


	}//end of while

	return allFirstSets;		//returns the complete list of ALL first sets of non terminals

}
// Task 3
void CalculateFirstSets(){

	vector<set<string>> allFirstSets = findFirstSets();	//finds first sets

	set<string> listOfNonTerms;	//list of ALL non terminals

	for(Rule* rule : parser.all_rules){
		addToSet(&listOfNonTerms,rule->LHS);		//adds all nonterminals to the set of non terminals
	}
	printFirstSets(&allFirstSets,&listOfNonTerms);		//prints first sets in correct order

}


bool addFollowSets(set<string>* set_addTo,set<string> set_addFrom){	
	bool isElementNotFound = false;		//true if atleast 1 element from set_addFrom was NOT in set_addTo, and was then added

	for(string symbol_addFrom : set_addFrom){
		if(!isInSet(set_addTo,symbol_addFrom)){		//if the current symbol from set_addFrom is NOT in set_addTo
			isElementNotFound = true;		//element was NOT found, so true
		}

	}
	if(isElementNotFound){	//if an element in set_addFrom was NOT found in set_addTo
		(*set_addTo).insert(set_addFrom.begin(),
						set_addFrom.end());		//insert all elements from set_addFrom to set_addTo
	}
	/*
	for(string symbol_addFrom : set_addFrom){		//for every symbol in set_addFrom
		bool hasAdded = addToSet(set_addTo, symbol_addFrom);		//hasAdded is true if SOMETHING was added to set
		if(!boolResult && hasAdded) boolResult = true;	//if boolResult is FALSE and hasAdded is TRUE
	}
	*/
	return isElementNotFound;

}

vector<string> findFollowInOrder(set<string> followset_unordered){	//returns vector<string> of the inputed followset but in the order it appears in the grammar
	vector<string> followvector;	//vector of current first set sorted

	if(isInSet(&followset_unordered,"$")){	//if the unordered set contains eOF
		followvector.push_back("$");	//add epsilon to the front of the vector
	}	
	for(Rule* rule : parser.all_rules){	
		// **elements of a followset are just terminals, so no checking rule->LHS**
		for(string RHSSymbol : rule->RHS){
			if(RHSSymbol != "$" && !findInVector(followvector,RHSSymbol) && isInSet(&followset_unordered,RHSSymbol)){
				//if:
				//1. The current RHS symbol is NOT eof
				//2. AND the current RHS symbol is NOT in the sorted follow vector
				//3. AND the current RHS symbol is IN the unordered follow set

				followvector.push_back(RHSSymbol);	//add the symbol to the first vector
			}
		}
	}
	return followvector;
	
}
void printFollowSets(vector<set<string>>* allFollowSets,set<string>* listOfNonTerms){	//passes vector with ALL first sets and set with ALL non terminals, and prints first sets of non terminals
	vector<string> listOfNonTerms_ordered = findNTInOrder((*listOfNonTerms));	
	//passes set of non terminals and returns the vector of nonterminals in the ORDER for which they appear in the grammar
	
	for(string nonTerminal : listOfNonTerms_ordered){	//for each of the non terminals
		int nonTermIndex = index(nonTerminal);	//index of the non terminal in grammarUniverse
		set<string> followSet = (*allFollowSets)[nonTermIndex];
		vector<string> followVector = findFollowInOrder(followSet);	//returns first set sorted correctly to print
		
		cout << "FOLLOW(" << nonTerminal << ") = {";
		bool isFirstElement = true;	//true if is first element of the set
		for(string setSymbol : followVector){	//for each symbol in the set
			if(isFirstElement){
				isFirstElement = false;	
			}else{
				cout << ",";
			}
			cout << " " << setSymbol;
		}
		cout << " }" << endl;
	}
}
// Task 4

vector<set<string>> findFollowSets(){		//returns all follow sets of the nonterminals in the grammar

	//  *** initialization *** //
	
	set<string> listOfNonTerms;	//list of ALL non terminals

	for(Rule* rule : parser.all_rules){
		addToSet(&listOfNonTerms,rule->LHS);		//adds all nonterminals to the set of non terminals
	}
	vector<set<string>> allFirstSets = findFirstSets();	//finds first sets
	vector<set<string>> allFollowSets(parser.grammarUniverse.size());	//all Follow sets for the grammar
	
	// *** Rule 1 add "$" to FOLLOW(starting rule symbol) *** //
	string startRuleSymbol = parser.all_rules[0]->LHS;	//the LHS of the starting rule symbol of the grammar
	allFollowSets[index(startRuleSymbol)].insert("$");	//at the FOLLOW set of the starting rule symbol, insert "$"
	
	/*
	cout << "Before" << endl;
	allFollowSets[0].insert("in the [0] index");
			for(int i =0;i < parser.grammarUniverse.size();i++){
				cout << "Follow of " << parser.grammarUniverse[i] << " : " ;
				printSet(allFollowSets[i]);
				cout << "\n" << endl;
			}
	
	cout << "after" << endl;
	addFollowSets(&allFollowSets[1],allFollowSets[0]);
			for(int i =0;i < parser.grammarUniverse.size();i++){
				cout << "Follow of " << parser.grammarUniverse[i] << " : " ;
				printSet(allFollowSets[i]);
				cout << "\n" << endl;
			}
	*/


	bool changed = true;
	while(changed){
		changed = false;

		for(string nonTerminal : listOfNonTerms){	//for each non terminal
			int NTIndex = index(nonTerminal);	//index of the non terminal
			set<string>* NTFollowSet = &allFollowSets[NTIndex];	//FOLLOW set of the current nonterminal

			vector<Rule*> symbolRules;		//empty set that contains all rules that has setSymbol as the LHS

			// *** symbolRules initialization *** //
			for(Rule* rule : parser.all_rules){
				if(nonTerminal == rule->LHS){		//if the current set symbol is the rule LHS
					symbolRules.push_back(rule);	//adds the rule to symbolRules
				
					/*	
					cout << rule->LHS << " --> ";	//DEBUG TOOL
					printVector(rule->RHS);		//DEBUG TOOL
					cout << endl;			//DEBUG TOOL
					*/
				}
			}
			
			// *** Rule 2 *** //
			for(Rule* rule : symbolRules){
				int endIndex = rule->RHS.size() - 1;	//index of the final element of rule->RHS
				string endSymbol = rule->RHS[endIndex];	//final symbol of rule->RHS

				set<string>* endFollowSet = &allFollowSets[index(endSymbol)];	//FOLLOW set of final element of rule->RHS
				if(addFollowSets(endFollowSet,*NTFollowSet)){		//Rule 2: add FOLLOW(A) to FOLLOW(B)
					changed = true;
				}	
			}
		
			// *** Rule 4 *** //
			for(Rule* rule : symbolRules){
				int finalIndex = rule->RHS.size() - 1;	//index of the final element of rule->RHS

				for(int indexOfB = 0;indexOfB < rule->RHS.size(); indexOfB++){	//indexOfB = index of current symbol being looked at
					//Iterates through all symbols in RHS of current rule
					//ex: A --> ... B A1 A2 .. Ak
					//indexOfB is index of B
					//indexOfA1 is index of A1
					//for loops iterates through ALL symbols of RHS, so ALL symbols act as B and A1
					
					int indexOfA1 = indexOfB + 1;		

					// *** add FIRST(A1) - {#} to FOLLOW(B) *** //


				
					if(indexOfA1 <= finalIndex){
						string symbolB = rule->RHS[indexOfB];				//symbol of B
						string symbolA1 = rule->RHS[indexOfA1];				//symbol of A1
						set<string>* followSetB = &allFollowSets[index(symbolB)];	//FOLLOW set of B
						set<string> firstSetA1 = allFirstSets[index(symbolA1)];		//FIRST set of A1

						if(addFirstSets(followSetB,firstSetA1)){		//if successfully added FIRST(A1) - {#} to FOLLOW(B)
							changed = true;
						}
					}
					
				}
			}
			// *** Rule 3 and 5
			for(Rule* rule : symbolRules){
				int finalIndex = rule->RHS.size() - 1;	//index of the final element of rule->RHS


				for(int indexOfB = 0; indexOfB < rule->RHS.size(); indexOfB++){
					int indexOfNonEps = -1;	

					for(int indexOfA = indexOfB + 1; indexOfA < rule->RHS.size(); indexOfA++){
						string symbolA = rule->RHS[indexOfA];
						set<string> firstSetA = allFirstSets[index(symbolA)];

						if(!isInSet(&firstSetA,"#")){	//# is NOT in set
							indexOfNonEps = indexOfA;	
							break;
						}
					}
					if(indexOfNonEps == -1){	//Rule 3; did not find a rule without epsilon,

						//FOLLOW(A) to FOLLOW(B)
						string symbolB = rule->RHS[indexOfB];		
						set<string>* followSetB = &allFollowSets[index(symbolB)];
						if(addFollowSets(followSetB,*NTFollowSet)){		//Rule 2: add FOLLOW(A) to FOLLOW(B)
							changed = true;
						}	
							
					}
					else{			//Rule 5; indexOfNonEps contains an index with epsilon 	
							//FIRST(Ai+1) - {#} to FOLLOW(B)
						string symbolB = rule->RHS[indexOfB];
						set<string>* followSetB = &allFollowSets[index(symbolB)];		//FOLLOW(B)
						string symbolA = rule->RHS[indexOfNonEps];
						set<string> firstSetA = allFirstSets[index(symbolA)];

						if(addFirstSets(followSetB,firstSetA)){		//Rule 2: add FOLLOW(A) to FOLLOW(B)
							changed = true;
						}	
					}
				}
			}
		}//end of for	
	}//end of while
	return allFollowSets;
}
void CalculateFollowSets(){
	vector<set<string>> allFollowSets = findFollowSets();	//all follow sets of non terminals in grammar

	set<string> listOfNonTerms;	//list of ALL non terminals

	for(Rule* rule : parser.all_rules){
		addToSet(&listOfNonTerms,rule->LHS);		//adds all nonterminals to the set of non terminals
	}

	printFollowSets(&allFollowSets,&listOfNonTerms);
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
	vector<Rule*> RulesNoUseless = FindUselessSymbols();	//vector with useless symbols removed
	vector<set<string>> allFirstSets = findFirstSets();	//all first sets in the grammar
	vector<set<string>> allFollowSets = findFollowSets();	//all follow sets in the grammar


	string result = "YES";
	if(RulesNoUseless != parser.all_rules){			//if RulesNoUseless HAD useless symbols and is not the same as the original grammar (useless symbols removed in RuleNoUseless)
		result = "NO"; 		//grammar has NO predictive parser
	}
	for(Rule* rule : RulesNoUseless){		//for each rule
		string nonTerminal = rule->LHS;
		set<string> nonTerminalFirstSet = allFirstSets[index(nonTerminal)];	//FIRST set of the nonterminal
		if(isInSet(&nonTerminalFirstSet, "#")){	//if the first set contains epsilon
			set<string> nonTerminalFollowSet = allFollowSets[index(nonTerminal)];
			
			vector<string> resultVector(parser.grammarUniverse.size());
			vector<string>::iterator resultIter;
			resultIter = set_intersection(nonTerminalFirstSet.begin(),nonTerminalFirstSet.end(),			//first
											nonTerminalFollowSet.begin(),nonTerminalFollowSet.end(),	//follow
															resultVector.begin());		//result placed in resultVector
			resultVector.resize(resultIter - resultVector.begin());		//MUST resize to get proper size
			if(resultVector.size() != 0){	//if the size is not zero, is NOT a disjoint
				//is NOT predictive parser, return NO
				result = "NO";
			}
		}
	}

	cout << result << endl;		//print result
}
    
int main (int argc, char* argv[])
{
    parser.grammarUniverse.push_back("#");	//adds '#' to universe
    parser.grammarUniverse.push_back("$");	//adds '$' to universe

    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);
    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
            break;

        case 3: CalculateFirstSets();
            break;

        case 4: CalculateFollowSets();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

