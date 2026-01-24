
%define api.value.type union

%code requires {
#include "ast.h"
#include <stddef.h>
}

%token T_NEWLINE
%token <char *> T_WORD
%token <char *> T_IDENT

%token T_AND
%token T_OR
%token T_PIPE
%token T_LPAREN
%token T_RPAREN
%token T_LBRACE
%token T_RBRACE
%token T_LBRACK
%token T_RBRACK
%token T_SEMI

%token T_DOLLAR
%token <char *> T_VAR
%token T_DOLLAR_LPAREN
%token T_DOLLAR_LBRACE

%token T_IF
%token T_THEN
%token T_ELIF
%token T_FI
%token T_FOR
%token T_DO
%token T_DONE
%token T_WHILE
%token T_CASE
%token T_ESAC

%type <ast_node_t*> word_list simple_command
%type <ast_node_t*> word

%parse-param { ast_node_t **out }

%start command

%%

command
	: simple_command T_NEWLINE { *out = $1; YYACCEPT; }
	| YYEOF { *out = NULL; }

simple_command: word_list { $$ = $1; }

word_list
	: word
	{
		$$ = ast_node_generic(AST_WORD_LIST, $1, NULL);
	}
	| word_list word
	{
		$$ = ast_node_generic(AST_WORD_LIST, $1, $2);
	}

word
	: T_WORD		{ $$ = ast_word($1); }
/*	| T_VAR			{ $$ = $1; } */

%%

