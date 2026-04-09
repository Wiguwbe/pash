#ifndef _PASH_AST_INTERNAL_H_
#define _PASH_AST_INTERNAL_H_

#include "ast.h"

ast_node_t *ast_semi_new(ast_node_t *init);
ast_node_t *ast_semi_append(ast_node_t *semi_list, ast_node_t *item);
void ast_semi_free(ast_node_t *init);

ast_node_t *ast_and_or_new(ast_node_t *left, int is_and, ast_node_t *right);
void ast_and_or_free(ast_node_t *node);

ast_node_t *ast_pipeline_new(ast_node_t *init);
ast_node_t * ast_pipeline_append(ast_node_t *pipeline, ast_node_t *item);
void ast_pipeline_free(ast_node_t *pipeline);

ast_node_t *ast_command_new(ast_node_t *assignments, ast_node_t *inner_command);
void ast_command_free(ast_node_t *command);

ast_node_t *ast_if_new(ast_node_t *condition, ast_node_t *then_branch, ast_node_t *else_branch);
void ast_if_free(ast_node_t *node);

ast_node_t *ast_while_new(ast_node_t *condition, ast_node_t *body);
void ast_while_free(ast_node_t *node);

ast_node_t *ast_for_new(const char *ident, ast_node_t *list, ast_node_t *body);
void ast_for_free(ast_node_t *node);

ast_node_t *ast_def_new(const char *ident, ast_node_t *body);
void ast_def_free(ast_node_t *node);

ast_node_t *ast_word_list_new(ast_node_t *init);
ast_node_t *ast_word_list_append(ast_node_t *wlist, ast_node_t *item);
void ast_word_list_free(ast_node_t *node);

ast_node_t *ast_word_new(const char *word);
void ast_word_free(ast_node_t *node);

ast_node_t *ast_var_new(const char *word, ast_node_t *subscripts);
void ast_var_free(ast_node_t *node);

ast_node_t *ast_command_exp_new(ast_node_t *command);
void ast_command_exp_free(ast_node_t *node);

ast_node_t *ast_dict_new(ast_node_t *key_init, ast_node_t *val_init);
ast_node_t *ast_dict_append(ast_node_t *dict, ast_node_t *key, ast_node_t *value);
void ast_dict_free(ast_node_t *dict);

ast_node_t *ast_list_new(ast_node_t *init);
ast_node_t *ast_list_append(ast_node_t *list, ast_node_t *value);
void ast_list_free(ast_node_t *node);

ast_node_t *ast_subscripts_new(ast_node_t *init);
ast_node_t *ast_subscripts_append(ast_node_t *subscripts, ast_node_t *value);
void ast_subscripts_free(ast_node_t *node);

// helper to parse/read a quoted string
char *ast_parse_string(const char *in);

#endif
