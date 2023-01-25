/* name: David Zalewski
 * ID: 1215324552
 * Class: CSE340 MW 12:00-1:15pm
 */ 


#include <string>
#include <iostream>
#include "parser.h"

using namespace std;

vector<string> location_table;	//contains the variables from var_section, mapped to the same index as it's index in mem array

void debug_printMem(){	//prints contents of mem
	for(int i = 0;i < next_available; i++){
		cout << "mem[" << i << "] = " << mem[i];
		cout << "; name associated with = " << location_table.at(i) << endl;
	}	
}
void debug_printInstTypes(struct InstructionNode* inst){	//prints instruction types of each instruction node
	while(inst != nullptr){
		cout << "Type: " << inst->type;
		cout << "; Operand type: " << inst->assign_inst.op << endl;
		inst = inst->next;
	}
}
struct InstructionNode* parse_generate_intermediate_representation(){
	struct InstructionNode* intermediate_R = nullptr;
	
	Parser parser;

	intermediate_R = parser.parse_program();
	//debug_printMem();
	//debug_printInstTypes(intermediate_R);
	return intermediate_R;
}


void Parser::append(struct InstructionNode* addingNodeTo, struct InstructionNode* nodeBeingAdded){
	//appends nodeBeingAdded to the end of addingNodeTo
	while(addingNodeTo->next != nullptr){
		addingNodeTo = addingNodeTo->next;
	}
	addingNodeTo->next = nodeBeingAdded;
}
int location(string name){	//returns index of location associated with string name in mem array
	for(int i = 0; i < location_table.size(); i++){
		if(location_table[i] == name){
			return i;
		}
	}
	return -1;		//variable not found
}
int allocateMemory(string name, int value){	//allocates space in mem and location_table for new variable/constant and sets the value at the memory

	location_table.push_back(name);	//pushes variable name into location table
	mem[next_available] = value;	//sets value at location
	int address = next_available;	//address associated with name
	next_available++;		//increments to allow next allocation
	
	return address;
}
struct InstructionNode* Parser::parse_program(){
	struct InstructionNode* inst = nullptr;

	parse_var_section();
	inst = parse_body();
	parse_inputs();

	lexer.GetToken();	//expect END_OF_FILE

	return inst;
}
void Parser::parse_var_section(){
	parse_id_list();
	lexer.GetToken();	//expect SEMICOLON
}
void Parser::parse_id_list(){
	Token id  = lexer.GetToken();	//expect ID
	string var_name = id.lexeme;	//variable name
	
	allocateMemory(var_name,0);	//creates memory for variable name and sets value at memory to 0

	Token t1 = lexer.peek(1);
	if(t1.token_type == COMMA){
		// id_list -> ID COMMA id_list
		lexer.GetToken();	//expect COMMA
		parse_id_list();
	}else if(t1.token_type == SEMICOLON){
		// id_list -> ID
	}
}
struct InstructionNode* Parser::parse_body(){
	struct InstructionNode* inst = nullptr;

	lexer.GetToken();	//expect LBRACE
	inst = parse_stmt_list();
	lexer.GetToken();	//expect RBRACE

	return inst;
}
struct InstructionNode* Parser::parse_stmt_list(){
	struct InstructionNode* inst = nullptr;
	inst = parse_stmt();
	
	Token t1 = lexer.peek(1);
	if(t1.token_type == ID || t1.token_type == WHILE || t1.token_type == IF
			|| t1.token_type == SWITCH || t1.token_type == FOR
				|| t1.token_type == OUTPUT || t1.token_type == INPUT){	
		// stmt_list -> stmt stmt_list
		struct InstructionNode* inst2 = nullptr;	//node for stmt_list
		inst2 = parse_stmt_list();
		append(inst,inst2);		//appends inst2 to the end of inst
		
	}else if(t1.token_type == RBRACE){
		// stmt_list -> stmt
	}
	return inst;
}
struct InstructionNode* Parser::parse_stmt(){
	struct InstructionNode* inst = nullptr;
	Token t1 = lexer.peek(1);
	switch(t1.token_type){
		case(ID):
			inst = parse_assign_stmt();
			break;
		case(OUTPUT):
			inst = parse_output_stmt();
			break;
		case(INPUT):
			inst = parse_input_stmt();
			break;
		case(IF):
			inst = parse_if_stmt();
			break;
		case(WHILE):
			inst = parse_while_stmt();
			break;
		case(SWITCH):
			inst = parse_switch_stmt();
			break;
		case(FOR):
			inst = parse_for_stmt();
			break;
	}
	return inst;
}
struct InstructionNode* Parser::parse_assign_stmt(){
	Token toAssign = lexer.GetToken();	//ID
	int address_LHS = location(toAssign.lexeme);	//address of variable on LHS

	lexer.GetToken();	//expect EQUAL

	// **creating instruction node** //
	struct InstructionNode* inst = new InstructionNode();
	inst->type = ASSIGN;	//sets instruction to ASSIGN mode
	inst->assign_inst.left_hand_side_index = address_LHS;
	inst->next = nullptr;

	Token t1 = lexer.peek(2);	//check op token, if is an op then parse expr, if semicolon then parse primary

	if(t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == MULT || t1.token_type == DIV){
		//assign_stmt -> ID EQUAL expr SEMICOLON
		parse_expr(inst);
	}else if(t1.token_type == SEMICOLON){
		//assign_stmt -> ID EQUAL primary SEMICOLON
		// ex: a = b
		inst->assign_inst.op = OPERATOR_NONE;		//no operator
		inst->assign_inst.operand1_index = parse_primary();	//address of variable/constant
	}

	lexer.GetToken();	//expect SEMICOLON
	
	return inst;
}
void Parser::parse_expr(struct InstructionNode* inst){
	inst->assign_inst.operand1_index = parse_primary();

	parse_op(inst);

	inst->assign_inst.operand2_index = parse_primary();
}
int Parser::parse_primary(){	//returns address of location associated with primary
	Token primary = lexer.GetToken();	//expect ID || NUM
	int address = location(primary.lexeme);	//address of ID or NUM in location table
	if(primary.token_type == NUM && address == -1){	//address not found and is a NUM
		address = allocateMemory(primary.lexeme,stoi(primary.lexeme));	//allocates new constant for NUM
	}

	return address;
}
void Parser::parse_op(struct InstructionNode* inst){
	Token op = lexer.GetToken();	//expect PLUS || MINUS || MULT || DIV
	switch(op.token_type){
		case(PLUS):
			inst->assign_inst.op = OPERATOR_PLUS;
			break;
		case(MINUS):
			inst->assign_inst.op = OPERATOR_MINUS;
			break;
		case(MULT):
			inst->assign_inst.op = OPERATOR_MULT;
			break;
		case(DIV):
			inst->assign_inst.op = OPERATOR_DIV;
			break;

	}

}
struct InstructionNode* Parser::parse_output_stmt(){
	lexer.GetToken();	//expect OUTPUT 
	Token toOutput = lexer.GetToken();	//expect ID

	struct InstructionNode* inst = new InstructionNode();
	inst->type = OUT;
	inst->output_inst.var_index = location(toOutput.lexeme); //output value of variable
	inst->next = nullptr;

	lexer.GetToken();	//expect SEMICOLON
	return inst;
}
struct InstructionNode* Parser::parse_input_stmt(){
	lexer.GetToken();	//expect INPUT
	Token toInput = lexer.GetToken();	//expect ID

	struct InstructionNode* inst = new InstructionNode();
	inst->type = IN;
	inst->input_inst.var_index = location(toInput.lexeme);	//input value into variable
	inst->next = nullptr;

	lexer.GetToken();	//expect SEMICOLON
	return inst;
}
struct InstructionNode* Parser::parse_while_stmt(){
	struct InstructionNode* inst = new InstructionNode();
	inst->type = CJMP;

	lexer.GetToken();	//expect WHILE
	parse_condition(inst);
	inst->next = parse_body();

	// ** JMP node ** //
	struct InstructionNode* inst_jmp = new InstructionNode();	//JMP node
	inst_jmp->type = JMP;
	inst_jmp->jmp_inst.target = inst;	//inst_jmp --(jumps to)--> inst
	append(inst,inst_jmp);	//appends JMP node to end of WHILE instruction body

	// ** NOOP node ** //
	struct InstructionNode* inst_noop = new InstructionNode();	//NOOP node
	inst_noop->type = NOOP;
	inst_noop->next = nullptr;

	inst->cjmp_inst.target = inst_noop;	//target node of WHILE instruction is the NOOP node
	inst_jmp->next = inst_noop;	//JMP node->next = NOOP node

	return inst;
}
struct InstructionNode* Parser::parse_if_stmt(){
	struct InstructionNode* inst = new InstructionNode();
	inst->type = CJMP;

	lexer.GetToken();	//expect IF
	parse_condition(inst);
	inst->next = parse_body();

	struct InstructionNode* inst_noop = new InstructionNode();	//NOOP node
	inst_noop->type = NOOP;
	inst_noop->next = nullptr;
	append(inst,inst_noop);		//appends NOOP node  to the end of IF instruction node body

	inst->cjmp_inst.target = inst_noop;	//target node of IF instruction is the NOOP node
	return inst;

}
void Parser::parse_condition(struct InstructionNode* inst){
	int address_1 = parse_primary();
	ConditionalOperatorType op = parse_relop();	//returns operator type of relop token
	int address_2 = parse_primary();

	inst->cjmp_inst.condition_op = op;
	inst->cjmp_inst.operand1_index = address_1;
	inst->cjmp_inst.operand2_index = address_2;
}
ConditionalOperatorType Parser::parse_relop(){
	Token t1 = lexer.GetToken();
	ConditionalOperatorType op;
	if(t1.token_type == GREATER){
		op = CONDITION_GREATER;
	}else if(t1.token_type == LESS){
		op = CONDITION_LESS;
	}else if(t1.token_type == NOTEQUAL){
		op = CONDITION_NOTEQUAL;
	}
	return op;
}
struct InstructionNode* Parser::parse_switch_stmt(){

	lexer.GetToken();	//expect SWITCH
	Token operand1 = lexer.GetToken();	//expect ID
	lexer.GetToken();	//expect LBRACE

	// ** NOOP node ** //
	// NOOP node at very end of switch statement
	// JMP nodes will jump to the end if one case body is successfully executed
	struct InstructionNode* inst_noop = new InstructionNode();
	inst_noop->type = NOOP;
	inst_noop->next = nullptr;
	//inst_noop targeted by ALL JMP nodes at the end of each case node

	struct InstructionNode* inst = parse_case_list(location(operand1.lexeme),inst_noop);	
	//determines if operand1 == operand2 if true then runs code in CASE body
	


	Token t1 = lexer.peek(1);
	if(t1.token_type == RBRACE){
		//switch_stmt -> SWITCH ID LBRACE case_list RBRACE
	}else if(t1.token_type == DEFAULT){
		//switch_stmt -> SWITCH ID LBRACE case_list default_case RBRACE
		struct InstructionNode* inst2 = parse_default_case();
		append(inst,inst2);	//append inst2 to the end of inst
	}
	lexer.GetToken();	//expect RBRACE

	append(inst,inst_noop);	//appends inst_noop to end of inst

	return inst;
}
struct InstructionNode* Parser::parse_for_stmt(){
	lexer.GetToken();	//expect FOR
	lexer.GetToken();	//expect LPAREN

	struct InstructionNode* inst_assign1 = parse_assign_stmt();
	struct InstructionNode* inst_condition = new InstructionNode();
	inst_condition->type = CJMP;
	parse_condition(inst_condition);

	lexer.GetToken();	//expect SEMICOLON

	struct InstructionNode* inst_assign2 = parse_assign_stmt();

	lexer.GetToken();	//expect RPAREN

	struct InstructionNode* inst_body = parse_body();
	
	/**** JMP and NOOP nodes ****/
	//JMP node
	struct InstructionNode* inst_jmp = new InstructionNode();
	inst_jmp->type = JMP;
	inst_jmp->jmp_inst.target = nullptr;
	inst_jmp->next = nullptr;

	//NOOP node
	struct InstructionNode* inst_noop = new InstructionNode();
	inst_noop->type = NOOP;
	inst_noop->next = nullptr;

	/**** Appending list ****/
	append(inst_assign1,inst_condition);	//appends inst_condition to end of inst_assign1
	append(inst_assign1,inst_body);		//appends inst_body to end of inst_assign1
	append(inst_assign1,inst_assign2);	//appends inst_assign2 to end of inst_assign1
	append(inst_assign1,inst_jmp);		//appends inst_jmp to end of inst_assign1
	append(inst_assign1,inst_noop);		//appends inst_noop to end of inst_assign1

	/**** Targeting nodes ****/
	inst_condition->cjmp_inst.target = inst_noop;	//cjmp node targets the noop node
	inst_jmp->jmp_inst.target = inst_condition;	//jmp node targets the cjmp node

	return inst_assign1;

}
struct InstructionNode* Parser::parse_case_list(int address_operand1,struct InstructionNode* inst_end){
	struct InstructionNode* inst = parse_case(address_operand1,inst_end);

	Token t1 = lexer.peek(1);
	if(t1.token_type == CASE){
		//case_list -> case case_list
		struct InstructionNode* inst2 = parse_case_list(address_operand1,inst_end);
		append(inst,inst2);	//append inst2 to the end of inst
	}else if(t1.token_type == DEFAULT || t1.token_type == RBRACE){
		//case_list -> case
	}
	return inst;
}
struct InstructionNode* Parser::parse_case(int address_operand1,struct InstructionNode* inst_end){
	//structure of nodes:
	// CJMP  -> ...(case body)... -> JMP -> NOOP ->...
	/*** parsing tokens ***/
	lexer.GetToken();	//expect CASE
	Token operand2 = lexer.GetToken();	//expect NUM
	lexer.GetToken();	//expect COLON
	
	int address_operand2 = location(operand2.lexeme);
	if(address_operand2 == -1){	//address not found 
		address_operand2 = allocateMemory(operand2.lexeme,stoi(operand2.lexeme));	//allocates new constant for NUM
	}

	/*** CJMP node ***/ 
	// If operands are NOT equal, moves to next (jmp node) which jumps to the next case (next CJMP node)
	// If operands ARE equal, moves to target (NOOP node) which then executes the case body. Ends on last JMP node which jumps to the end of switch statement
	struct InstructionNode* inst_cjmp = new InstructionNode();
	inst_cjmp->type = CJMP;
	inst_cjmp->cjmp_inst.condition_op = CONDITION_NOTEQUAL;	   //jumps to target if operands ARE EQUAL
	inst_cjmp->cjmp_inst.operand1_index = address_operand1;
	inst_cjmp->cjmp_inst.operand2_index = address_operand2;
	inst_cjmp->cjmp_inst.target = nullptr;
	inst_cjmp->next = nullptr;

	/*** Case BODY ***/
	struct InstructionNode* inst_body = parse_body();

	/*** JMP node ***/
	struct InstructionNode* inst_jmp = new InstructionNode();
	inst_jmp->type = JMP;
	inst_jmp->jmp_inst.target = nullptr;
	inst_jmp->next = nullptr;

	/*** NOOP NODE ***/
	struct InstructionNode* inst_noop = new InstructionNode();
	inst_noop->type = NOOP;
	inst_noop->next = nullptr;

	/*** setting next nodes ***/
	inst_cjmp->next = inst_noop;	//if op1 != op2, skips body
	append(inst_body,inst_jmp);	//appends JMP node to end of body
	inst_jmp->next = inst_noop;	//JMP next is NOOP node


	/*** setting targets ***/
	// CJMP  -> ...(case body)... -> JMP -> NOOP ->...
	inst_cjmp->cjmp_inst.target = inst_body;	//if op1 == op2, proceeds to target (body list)
	inst_jmp->jmp_inst.target = inst_end;		//JMP node at end of body proceeds to end node (NOOP) of switch statement

	return inst_cjmp;
}
struct InstructionNode* Parser::parse_default_case(){
	lexer.GetToken();	//expect DEFAULT
	lexer.GetToken();	//expect COLON
	struct InstructionNode* inst = parse_body();
	return inst;
}
struct InstructionNode* Parser::parse_inputs(){
	parse_num_list();
}
struct InstructionNode* Parser::parse_num_list(){
	Token newInput = lexer.GetToken();	//expect NUM
	
	inputs.push_back(stoi(newInput.lexeme));	//adds NUM to the inputs vector

	Token t1 = lexer.peek(1);
	if(t1.token_type == NUM){
		//num_list -> NUM num_list
		parse_num_list();
	}else if(t1.token_type == END_OF_FILE){
		//num_list -> NUM
	}
}
