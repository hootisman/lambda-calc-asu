
/* name: David Zalewski
 * ID: 1215324552
 * Class: CSE340 MW 12:00-1:15pm
 */
#include "lexer.h"
#include "compiler.h"
#ifndef _PARSER_H
#define _PARSER_H

class Parser{

	public:
		LexicalAnalyzer lexer;
		void append(struct InstructionNode*,struct InstructionNode*);	//appends the second argument to the first argument
		struct InstructionNode* parse_program();
	private:
		void parse_var_section();
		void parse_id_list();
		struct InstructionNode* parse_body();
		struct InstructionNode* parse_stmt_list();
		struct InstructionNode* parse_stmt();
		struct InstructionNode* parse_assign_stmt();
		void parse_expr(struct InstructionNode*);
		int parse_primary();
		void parse_op(struct InstructionNode*);
		struct InstructionNode* parse_output_stmt();
		struct InstructionNode* parse_input_stmt();
		struct InstructionNode* parse_while_stmt();
		struct InstructionNode* parse_if_stmt();
		void parse_condition(struct InstructionNode*);
		ConditionalOperatorType parse_relop();
		struct InstructionNode* parse_switch_stmt();
		struct InstructionNode* parse_for_stmt();
		struct InstructionNode* parse_case_list(int,struct InstructionNode*);
		struct InstructionNode* parse_case(int,struct InstructionNode*);
		struct InstructionNode* parse_default_case();
		struct InstructionNode* parse_inputs();
		struct InstructionNode* parse_num_list();
};

#endif
