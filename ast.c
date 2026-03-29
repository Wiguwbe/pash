
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK_KIND(node, exp_kind)\
	if ((node)->kind != (exp_kind)) {\
		fprintf(stderr, "wrong node kind\n");\
		abort();}

#define CHECK_MEM(ptr)\
	if (!(ptr)) {\
		perror("failed to allocate memory");\
		abort(); }

static void (*freers[])(ast_node_t *) = {
	[AST_SEMI_LIST]	= ast_semi_free,
	[AST_AND_OR]	= ast_and_or_free,
	[AST_PIPELINE]	= ast_pipeline_free,
	[AST_COMMAND]	= ast_command_free,
	[AST_IF]	= ast_if_free,
	[AST_WHILE]	= ast_while_free,
	[AST_FOR]	= ast_for_free,
	[AST_CASE]	= NULL,
	[AST_DEF]	= ast_def_free,
	[AST_WORD_LIST]	= ast_word_list_free,

	[AST_WORD]		= ast_word_free,
	[AST_VAR]		= ast_var_free,
	[AST_COMMAND_EXP]	= ast_command_exp_free,
	[AST_DICT]		= ast_dict_free,
	[AST_LIST]		= ast_list_free,
};

void ast_node_free(ast_node_t *node)
{
	void (*freer)(ast_node_t*) = freers[node->kind];
	if (!freer)
	{
		fprintf(stderr, "unknown kind %d\n", node->kind);
		abort();
	}

	freer(node);
}

ast_node_t *ast_semi_new(ast_node_t *init)
{
	ast_semi_list_t *sl = (ast_semi_list_t*)malloc(sizeof(ast_semi_list_t));
	CHECK_MEM(sl);
	sl->kind = AST_SEMI_LIST;
	sl->count = 0;
	sl->nodes = NULL;

	if (init)
		return ast_semi_append((ast_node_t*)sl, init);

	return (ast_node_t*)sl;
}
ast_node_t *ast_semi_append(ast_node_t *node, ast_node_t *item)
{
	CHECK_KIND(node, AST_SEMI_LIST)
	ast_semi_list_t *sl = (ast_semi_list_t*)node;

	int new_count = sl->count + 1;
	void *nptr = realloc(sl->nodes, new_count * sizeof(ast_node_t*));
	CHECK_MEM(nptr);
	sl->nodes = (ast_node_t**)nptr;
	sl->nodes[sl->count] = item;
	sl->count += 1;

	return (ast_node_t*)sl;
}
void ast_semi_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_SEMI_LIST);
	ast_semi_list_t *sl = (ast_semi_list_t*)node;

	for (int i=0;i<sl->count; i++)
		ast_node_free(sl->nodes[i]);

	free(sl->nodes);
	free(sl);
}

ast_node_t *ast_and_or_new(ast_node_t *left, int is_and, ast_node_t *right)
{
	ast_and_or_t *ao = (ast_and_or_t*)malloc(sizeof(ast_and_or_t));
	CHECK_MEM(ao);

	ao->kind = AST_AND_OR;
	ao->left = left;
	ao->right = right;
	ao->is_and = is_and;

	return (ast_node_t*)ao;
}
void ast_and_or_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_AND_OR);
	ast_and_or_t *ao = (ast_and_or_t*)node;

	ast_node_free(ao->left);
	ast_node_free(ao->right);

	free(ao);
}

ast_node_t *ast_pipeline_new(ast_node_t *init)
{
	ast_pipeline_t *pl = (ast_pipeline_t*)malloc(sizeof(ast_pipeline_t));
	CHECK_MEM(pl);

	pl->kind = AST_PIPELINE;
	pl->count = 0;
	pl->nodes = NULL;

	if (init)
		return ast_pipeline_append((ast_node_t*)pl, init);

	return (ast_node_t*)pl;
}
ast_node_t *ast_pipeline_append(ast_node_t *node, ast_node_t *item)
{
	CHECK_KIND(node, AST_PIPELINE);
	ast_pipeline_t *pl = (ast_pipeline_t*)node;

	int new_count = pl->count + 1;
	void *nptr = realloc(pl->nodes, new_count * sizeof(ast_node_t*));
	CHECK_MEM(nptr);
	pl->nodes = (ast_node_t**)nptr;
	pl->nodes[pl->count] = item;
	pl->count += 1;

	return (ast_node_t*)pl;
}
void ast_pipeline_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_PIPELINE);
	ast_pipeline_t *pl = (ast_pipeline_t*)node;

	for (int i = 0; i< pl->count; i++)
		ast_node_free(pl->nodes[i]);

	free(pl->nodes);
	free(pl);
}

ast_node_t *ast_command_new(ast_node_t *assignment, ast_node_t *inner_command)
{
	ast_command_t *cmd = (ast_command_t*)malloc(sizeof(ast_command_t));
	CHECK_MEM(cmd);

	cmd->kind = AST_COMMAND;
	cmd->assignments = assignment;
	cmd->inner_command = inner_command;

	return (ast_node_t*)cmd;
}

void ast_command_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_COMMAND);
	ast_command_t *cmd = (ast_command_t*)node;

	if (cmd->assignments)
		free(cmd->assignments);
	if (cmd->inner_command)
		free(cmd->inner_command);

	free(cmd);
}

ast_node_t *ast_if_new(ast_node_t *cond, ast_node_t *then, ast_node_t *else_)
{
	ast_if_t *if_n = (ast_if_t*)malloc(sizeof(ast_if_t));
	CHECK_MEM(if_n);

	if_n->kind = AST_IF;
	if_n->condition = cond;
	if_n->then_branch = then;
	if_n->else_branch = else_;

	return (ast_node_t*)if_n;
}
void ast_if_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_IF);
	ast_if_t *if_n = (ast_if_t*)node;

	ast_node_free(if_n->condition);
	ast_node_free(if_n->then_branch);
	if (if_n->else_branch)
		ast_node_free(if_n->else_branch);

	free(if_n);
}

ast_node_t *ast_while_new(ast_node_t *cond, ast_node_t *body)
{
	ast_while_t *while_n = (ast_while_t*)malloc(sizeof(ast_while_t));
	CHECK_MEM(while_n);

	while_n->kind = AST_WHILE;
	while_n->condition = cond;
	while_n->body = body;

	return (ast_node_t*)while_n;
}
void ast_while_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_WHILE);
	ast_while_t *while_n = (ast_while_t*)node;

	ast_node_free(while_n->condition);
	ast_node_free(while_n->body);

	free(while_n);
}

ast_node_t *ast_for_new(const char *ident, ast_node_t *list, ast_node_t *body)
{
	ast_for_t *for_n = (ast_for_t*)malloc(sizeof(ast_for_t));
	CHECK_MEM(for_n);

	for_n->kind = AST_FOR;
	for_n->ident = strdup(ident);
	for_n->list = list;
	for_n->body = body;

	return (ast_node_t*)for_n;
}
void ast_for_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_FOR);
	ast_for_t *for_n = (ast_for_t*)node;

	free(for_n->ident);
	ast_node_free(for_n->list);
	ast_node_free(for_n->body);

	free(for_n);
}

ast_node_t *ast_def_new(const char *ident, ast_node_t *body)
{
	ast_def_t *def = (ast_def_t*)malloc(sizeof(ast_def_t*));
	CHECK_MEM(def);

	def->kind = AST_DEF;
	def->name = strdup(ident);
	def->body = body;

	return (ast_node_t*)def;
}
void ast_def_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_DEF);
	ast_def_t *def = (ast_def_t*)node;

	free(def->name);
	ast_node_free(def->body);

	free(def);
}

ast_node_t *ast_word_list_new(ast_node_t *init)
{
	ast_word_list_t *wl = (ast_word_list_t*)malloc(sizeof(ast_word_list_t));
	CHECK_MEM(wl);

	wl->kind = AST_WORD_LIST;
	wl->count = 0;
	wl->words = NULL;

	if (init)
		return ast_word_list_append((ast_node_t*)wl, init);

	return (ast_node_t*)wl;
}
ast_node_t *ast_word_list_append(ast_node_t *node, ast_node_t *item)
{
	CHECK_KIND(node, AST_WORD_LIST);
	ast_word_list_t *wl = (ast_word_list_t*)node;

	int new_count = wl->count + 1;
	void *nptr = realloc(wl->words, new_count * sizeof(ast_node_t*));
	CHECK_MEM(nptr);

	wl->words = (ast_node_t**)nptr;
	wl->words[wl->count] = item;
	wl->count += 1;

	return (ast_node_t*)wl;
}
void ast_word_list_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_WORD_LIST);
	ast_word_list_t *wl = (ast_word_list_t*)node;

	for (int i =0;i<wl->count; i++)
		ast_node_free(wl->words[i]);

	free(wl->words);
	free(wl);
}

ast_node_t *ast_word_new(const char *word)
{
	ast_word_t *w = (ast_word_t*)malloc(sizeof(ast_word_t));
	CHECK_MEM(w);

	w->kind = AST_WORD;
	w->word = strdup(word); // caller should free it

	return (ast_node_t*)w;
}
void ast_word_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_WORD);
	ast_word_t *w = (ast_word_t*)node;

	free(w->word);
	free(w);
}

ast_node_t *ast_var_new(const char *var)
{
	ast_var_t *v = (ast_var_t*)malloc(sizeof(ast_var_t));
	CHECK_MEM(v);

	v->kind = AST_VAR;
	v->var_name = strdup(var);

	return (ast_node_t*)v;
}
void ast_var_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_VAR);
	ast_var_t *v = (ast_var_t*)node;

	free(v->var_name);
	free(v);
}

ast_node_t *ast_command_exp_new(ast_node_t *command)
{
	ast_command_exp_t *ce = (ast_command_exp_t*)malloc(sizeof(ast_command_exp_t));
	CHECK_MEM(ce);

	ce->kind = AST_COMMAND_EXP;
	ce->command = command;

	return (ast_node_t*)ce;
}
void ast_command_exp_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_COMMAND_EXP);
	ast_command_exp_t *ce = (ast_command_exp_t*)node;

	ast_node_free(ce->command);
	free(ce);
}

ast_node_t *ast_dict_new(ast_node_t *key_init, ast_node_t *val_init)
{
	ast_dict_t *dict = (ast_dict_t*)malloc(sizeof(ast_dict_t));
	CHECK_MEM(dict);

	dict->kind = AST_DICT;
	dict->count = 0;
	dict->items = NULL;

	if (key_init && val_init)
		return ast_dict_append((ast_node_t*)dict, key_init, val_init);

	return (ast_node_t*)dict;
}
ast_node_t *ast_dict_append(ast_node_t *node, ast_node_t *key, ast_node_t *val)
{
	CHECK_KIND(node, AST_DICT);
	ast_dict_t *dict = (ast_dict_t*)node;

	register int new_count = dict->count + 2;
	void *nptr = realloc(dict->items, new_count * sizeof(ast_node_t*));
	CHECK_MEM(nptr);
	dict->items = (ast_node_t**)nptr;
	dict->items[dict->count ++] = key;
	dict->items[dict->count ++] = val;

	return (ast_node_t*)dict;
}
void ast_dict_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_DICT);
	ast_dict_t *dict = (ast_dict_t*)node;

	for (int i =0 ;i <dict->count; i++)
		ast_node_free(dict->items[i]);

	free(dict->items);
	free(dict);
}

ast_node_t *ast_list_new(ast_node_t *init)
{
	ast_list_t *list = (ast_list_t*)malloc(sizeof(ast_list_t));
	CHECK_MEM(list);

	list->kind = AST_LIST;
	list->count = 0;
	list->items = NULL;

	if (init)
		return ast_list_append((ast_node_t*)list, init);

	return (ast_node_t*)list;
}
ast_node_t *ast_list_append(ast_node_t *node, ast_node_t *item)
{
	CHECK_KIND(node, AST_LIST);
	ast_list_t *list = (ast_list_t*)node;

	register int new_count = list->count + 1;
	register void *nptr = realloc(list->items, new_count * sizeof(ast_node_t*));
	CHECK_MEM(nptr);
	list->items = (ast_node_t**)nptr;
	list->items[list->count ++] = item;

	return (ast_node_t*)list;
}
void ast_list_free(ast_node_t *node)
{
	CHECK_KIND(node, AST_LIST);
	ast_list_t *list = (ast_list_t*)node;

	for (int i = 0; i < list->count; i++)
		ast_node_free(list->items[i]);

	free(list->items);
	free(list);
}

static const ast_node_t _empty_line = {AST_NONE};
const ast_node_t *empty_line = &_empty_line;

char *ast_parse_string(const char *in)
{
	size_t len = strlen(in);
	// it's probably less than the original
	char *out = (char*)malloc(len + 1);
	CHECK_MEM(out);

	char *o_ptr = out;
	char *i_ptr = (char*)in;
	char end = 0;
	char quote = *i_ptr;
	int quoted = quote == '"' || quote == '\'';
	if (quoted)
	{
		// start later
		i_ptr++;
		end = quote;
	}

	// if not quoted:
	//	escape spaces and newlines
	// if quoted:
	//	escape quotes only
	// always:
	//	escape backslashes
	while(*i_ptr && *i_ptr != end)
	{
		char next;
		if (*i_ptr == '\\')
		{
			next = i_ptr[1];
			if (next == '\\')
				goto _unescape;
			if (quoted && next == quote)
				goto _unescape;
			if (!quoted)
			{
				if (next == ' ')
					goto _unescape;
				if (next == '\n')
				{
					// ignore
					i_ptr+=2;
					continue;
				}
			}
			// else, fallthrough
		}
		// else, fallthrough

	_copy:
		*o_ptr = *i_ptr;
		goto _next;
	_unescape:
		*o_ptr = next;
		i_ptr++;
	_next:
		o_ptr ++;
		i_ptr ++;
	}

	*o_ptr = 0;

	return out;
}
