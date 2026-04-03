#ifndef _PASH_AST_H_
#define _PASH_AST_H_

typedef enum ast_kind {
	// just to have it here
	AST_NONE = 0,
	AST_SEMI_LIST,
	AST_AND_OR,
	AST_PIPELINE,

	AST_COMMAND,
	AST_IF,
	AST_WHILE,
	AST_FOR,
	AST_CASE, // not implemented
	AST_DEF,
	AST_WORD_LIST,

	AST_WORD = 20,
	AST_VAR,
	AST_COMMAND_EXP,
	AST_DICT,
	AST_LIST,

	AST__MAX,
} ast_kind_t;

typedef struct ast_node {
	ast_kind_t kind;
} ast_node_t;

typedef struct ast_semi_list {
	ast_kind_t kind;
	// do we need a list?
	int count;
	ast_node_t **nodes;
} ast_semi_list_t;

typedef struct ast_and_or {
	ast_kind_t kind;
	int is_and; // 1 == AND, 0 == OR
	ast_node_t *left;
	ast_node_t *right;
} ast_and_or_t;

typedef struct ast_pipeline {
	ast_kind_t kind;
	int count;
	ast_node_t **nodes;
} ast_pipeline_t;

typedef struct ast_command {
	ast_kind_t kind;
	ast_node_t *assignments;
	ast_node_t *inner_command;
} ast_command_t;

typedef struct ast_if {
	ast_kind_t kind;
	ast_node_t *condition;
	ast_node_t *then_branch;
	ast_node_t *else_branch;
} ast_if_t;

typedef struct ast_while {
	ast_kind_t kind;
	ast_node_t *condition;
	ast_node_t *body;
} ast_while_t;

typedef struct ast_for {
	ast_kind_t kind;
	char *ident;
	ast_node_t *list;
	ast_node_t *body;
} ast_for_t;

typedef struct ast_def {
	ast_kind_t kind;
	char *name;
	ast_node_t *body;
} ast_def_t;

typedef struct ast_word_list {
	ast_kind_t kind;
	int count;
	ast_node_t **words;
} ast_word_list_t;

/*
 * words
 */

typedef struct ast_word {
	ast_kind_t kind;
	char *word;
} ast_word_t;

typedef struct ast_var {
	ast_kind_t kind;
	char *var_name;
} ast_var_t;

typedef struct ast_commpand_exp {
	ast_kind_t kind;
	ast_node_t *command;
} ast_command_exp_t;

typedef struct ast_dict {
	ast_kind_t kind;
	int count;  // items is double that?
	ast_node_t **items;
} ast_dict_t;

typedef struct ast_list {
	ast_kind_t kind;
	int count;
	ast_node_t **items;
} ast_list_t;

// instead of returning an empty list at empty line, return this:
extern const ast_node_t *empty_line;

// generic destructor
void ast_node_free(ast_node_t *node);

#endif
