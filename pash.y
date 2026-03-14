
%define api.value.type union
%define api.pure full
%define api.push-pull push

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
%token T_COLON
%token T_COMMA

%token T_DOLLAR
%token <char *> T_VAR
%token T_DOLLAR_LPAREN
%token T_DOLLAR_LBRACE

%token T_IF
%token T_THEN
%token T_ELIF
%token T_ELSE
%token T_FI
%token T_FOR
%token T_DO
%token T_DONE
%token T_WHILE
%token T_CASE
%token T_ESAC
%token T_DEF
%token T_IN

%type <ast_node_t*> full_command semi_list and_or pipeline command if_command
%type <ast_node_t*> else_branch while_command for_command compound_command
%type <ast_node_t*> def_command simple_command word_list word nl_list
%type <ast_node_t*> command_substitution object dict dict_items literal_list
%type <ast_node_t*> list_items obj_list word_or_object

%parse-param { ast_node_t **out }

%start full_command

%%

full_command
	: T_NEWLINE	{ *out = ast_semi_new(NULL); /* make empty list */ YYACCEPT; }
	/* semi_list may be empty */
	| semi_list T_NEWLINE	{ *out = $semi_list; YYACCEPT; }
	| semi_list YYEOF	{ *out = $semi_list; YYACCEPT; }
	/* TODO allow semi_list + YYEOF? */
	| YYEOF	{ *out = NULL; }

semi_list
	/* TODO pass straight if single */
	: and_or
	  {
	  	/* TODO pass it straight */
	  	$$ = ast_semi_new($and_or);
	  }
	| semi_list[list] T_SEMI and_or
	  {
	  	/* TODO is `list` is not yet a list, make it */
	  	$$ = ast_semi_append($list, $and_or);
	  }

nl_list
	/* TODO same as above */
	: and_or
	  { $$ = ast_semi_new($and_or); }
	| nl_list[list] separator and_or
	  { $$ = ast_semi_append($list, $and_or); }

and_or
	: pipeline	{ $$ = $1; }
	| and_or[l] T_AND nl_opt pipeline[r]
	  { $$ = ast_and_or_new($l, 1, $r); }
	| and_or[l] T_OR nl_opt pipeline[r]
	  { $$ = ast_and_or_new($l, 0, $r); }

pipeline
	/* TODO same as semi/nl list: passthrough if single */
	: command
	  { $$ = ast_pipeline_new($command); }
	| pipeline[list] T_PIPE nl_opt command
	  { $$ = ast_pipeline_append($list, $command); }

command
	: if_command
	  { $$ = $1; }
	| while_command
	  { $$ = $1; }
	| for_command
	  { $$ = $1; }
	/*| case_command /* not implemented yet */
	| compound_command
	  { $$ = $1; }
	| def_command
	  { $$ = $1; }
	| simple_command
	  { $$ = $1; }

if_command
	: T_IF
	  nl_opt
	  nl_list[condition]
	  separator
	  T_THEN nl_opt
	  nl_list[then_branch]
	  separator
	  else_branch
	  /* separators ?? */
	  T_FI
	  { $$ = ast_if_new($condition, $then_branch, $else_branch); }

else_branch
	: %empty { $$ = NULL; }
	| T_ELSE
	  nl_opt
	  nl_list[body]
	  separator
	  { $$ = $body; }
	  /* only else, parse as compound command or something */
	| T_ELIF
	  nl_opt
	  nl_list[condition]
	  separator
	  T_THEN
	  nl_opt
	  nl_list[then_branch]
	  separator
	  else_branch[else_b]
	  /* this is considered another if command */
	  { $$ = ast_if_new($condition, $then_branch, $else_b); }

while_command
	: T_WHILE
	  nl_opt
	  nl_list[condition]
	  separator
	  T_DO
	  nl_opt
	  nl_list[body]
	  separator
	  T_DONE
	  { $$ = ast_while_new($condition, $body); }

for_command
	: T_FOR
	  T_IDENT[ident]
	  T_IN
	  obj_list[list] /* object list actually */
	  separator
	  T_DO
	  nl_opt
	  nl_list[body]
	  separator
	  T_DONE
	  { $$ = ast_for_new($ident, $list, $body); }

compound_command
	: T_LBRACE
	  nl_opt
	  nl_list[body]
	  separator
	  T_RBRACE
	  { $$ = $body; }

def_command
	: T_DEF
	  T_IDENT[name]
	  T_LPAREN T_RPAREN
	  compound_command[body]
	  { $$ = ast_def_new($name, $body); }

simple_command
	: word_list
	  { $$ = $1; }

obj_list
	: word_or_object
	  { $$ = ast_word_list_new($word_or_object); }
	/* does this need an explicit separator? */
	| obj_list[list] word_or_object
	  { $$ = ast_word_list_append($list, $word_or_object); }

word_list
	: word
	  { $$ = ast_word_list_new($word); }
	/* does this need an explicit whitespace separator? */
	| word_list word_or_object
	  { $$ = ast_word_list_append($1, $2); }

word
	: T_WORD		{ $$ = ast_word_new($1); }
	| T_IDENT		{ $$ = ast_word_new($1); }
	| T_VAR			{ $$ = ast_var_new($1); }
	| command_substitution	{ $$ = $1; }
	/*| var_expansion /* not implemented yet */

word_or_object
	: word		{ $$ = $1; }
	| object	{ $$ = $1; }

command_substitution
	: T_DOLLAR_LPAREN
	  nl_opt
	  nl_list
	  nl_opt
	  T_RPAREN
	  { $$ = ast_command_exp_new($nl_list); }

object
	: dict
	  { $$ = $1; }
	| literal_list
	  { $$ = $1; }

dict
	/* TODO add a bunch of optional newlines here */
	: T_LBRACE
	  dict_items
	  /* optional comma? */
	  T_RBRACE
	  { $$ = $dict_items; }
	| T_LBRACE
	  nl_opt
	  /* empty dict */
	  T_RBRACE
	  { $$ = ast_dict_new(NULL, NULL); }

dict_items
	: nl_opt
	  word[key]
	  nl_opt
	  T_COLON
	  nl_opt
	  word_or_object[val]
	  nl_opt
	  { $$ = ast_dict_new($key, $val); }
	| dict_items[dict]
	  T_COMMA
	  nl_opt
	  word[key]
	  nl_opt
	  T_COLON
	  nl_opt
	  word_or_object[val]
	  nl_opt
	  { $$ = ast_dict_append($dict, $key, $val); }

literal_list
	: T_LBRACK
	  list_items
	  /* optional comma? */
	  T_RBRACK
	  {$$ = $list_items; }
	| T_LBRACK
	  /* empty list */
	  T_RBRACK
	  { $$ = ast_list_new(NULL); }

list_items
	: nl_opt
	  word_or_object[val]
	  nl_opt
	  { $$ = ast_list_new($val); }
	| list_items[list]
	  T_COMMA
	  nl_opt
	  word_or_object[val]
	  nl_opt
	  { $$ = ast_list_append($list, $val); }

separator
	: T_SEMI
	| T_NEWLINE
	| separator T_NEWLINE

nl_opt
	: %empty
	| nl_opt T_NEWLINE

%%

