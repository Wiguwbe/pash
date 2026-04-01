
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// entrypoint
void print_ast_pretty(ast_node_t *node);

static int indent;

static void print_semi_list(ast_node_t *node);
static void print_and_or(ast_node_t *node);
static void print_pipeline(ast_node_t *node);
static void print_command(ast_node_t *node);
static void print_if(ast_node_t *node);
static void print_while(ast_node_t *node);
static void print_for(ast_node_t *node);
static void print_case(ast_node_t *node);
static void print_def(ast_node_t *node);
static void print_word_list(ast_node_t *node);
static void print_word(ast_node_t *node);
static void print_var(ast_node_t *node);
static void print_command_exp(ast_node_t *node);
static void print_dict(ast_node_t *node);
static void print_list(ast_node_t *node);

static void (*printers[])(ast_node_t *node) = {
	[AST_SEMI_LIST]	= print_semi_list,
	[AST_AND_OR]	= print_and_or,
	[AST_PIPELINE]	= print_pipeline,
	[AST_COMMAND]	= print_command,
	[AST_IF]	= print_if,
	[AST_WHILE]	= print_while,
	[AST_FOR]	= print_for,
	[AST_CASE]	= print_case,
	[AST_DEF]	= print_def,
	[AST_WORD_LIST]	= print_word_list,

	[AST_WORD]		= print_word,
	[AST_VAR]		= print_var,
	[AST_COMMAND_EXP]	= print_command_exp,
	[AST_DICT]		= print_dict,
	[AST_LIST]		= print_list,
};

static void print(ast_node_t *node)
{
	void (*printer)(ast_node_t*) = printers[node->kind];
	if (!printer)
	{
		fprintf(stderr, "unknown field %d\n", node->kind);
		abort();
	}

	printer(node);
}

void print_ast_pretty(ast_node_t *node)
{
	indent = 0;
	print(node);
}

static void iprintf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	printf("%*s", indent, "");
	vprintf(fmt, args);

	va_end(args);
}

/*
 * implementation
 */

static void print_semi_list(ast_node_t *node)
{
	ast_semi_list_t *sl = (ast_semi_list_t*)node;
	iprintf("SEMI_LIST(\n");
	indent += 2;
	for (int i = 0; i < sl->count; i++)
	{
		print(sl->nodes[i]);
	}
	indent -= 2;
	iprintf(")\n");
}

static void print_and_or(ast_node_t *node)
{
	ast_and_or_t *ao = (ast_and_or_t*)node;
	iprintf("AND_OR(\n");
	indent += 2;
	print(ao->left);
	iprintf("%s\n", ao->is_and ? "&&" : "||");
	print(ao->right);
	indent -= 2;
	iprintf(")\n");
}

static void print_pipeline(ast_node_t *node)
{
	ast_pipeline_t *pl = (ast_pipeline_t*)node;
	iprintf("PIPELINE(\n");
	indent += 2;
	for (int i = 0; i < pl->count; i++)
	{
		print(pl->nodes[i]);
	}
	indent -= 2;
	iprintf(")\n");
}

static void print_command(ast_node_t *node)
{
	ast_command_t *cmd = (ast_command_t*)node;
	iprintf("COMMAND(\n");
	indent += 2;
	if (cmd->assignments)
	{
		iprintf("ASSIGNMENTS(\n");
		indent += 2;
		print(cmd->assignments);
		indent -= 2;
		iprintf(")\n");
	}
	if (cmd->inner_command)
		print(cmd->inner_command);
	indent -= 2;
	iprintf(")\n");
}

static void print_if(ast_node_t *node)
{
	ast_if_t *in = (ast_if_t *)node;
	iprintf("IF(\n");
	indent += 2;
	print(in->condition);
	indent -= 2;
	iprintf(") THEN (\n");
	indent += 2;
	print(in->then_branch);
	indent -= 2;
	if (in->else_branch)
	{
		iprintf(") ELSE (\n");
		indent += 2;
		print(in->else_branch);
		indent -= 2;
	}
	iprintf(")\n");
}

static void print_while(ast_node_t *node)
{
	ast_while_t *wn = (ast_while_t *)node;
	iprintf("WHILE(\n");
	indent += 2;
	print(wn->condition);
	indent -= 2;
	iprintf(") DO (\n");
	indent += 2;
	print(wn->body);
	indent -= 2;
	iprintf(")\n");
}

static void print_for(ast_node_t *node)
{
	ast_for_t *fn = (ast_for_t *)node;
	iprintf("FOR(\n");
	indent += 2;
	iprintf("%s\n", fn->ident);
	iprintf("IN\n");
	print(fn->list);
	indent -= 2;
	iprintf(") DO (\n");
	indent += 2;
	print(fn->body);
	indent -= 2;
	iprintf(")\n");
}

static void print_case(ast_node_t *node)
{
	// not implemented yet
}

static void print_def(ast_node_t *node)
{
	ast_def_t *def = (ast_def_t*)node;
	iprintf("DEF(\n");
	indent += 2;
	iprintf("%s\n", def->name);
	indent -= 2;
	iprintf(") AS (\n");
	indent += 2;
	print(def->body);
	indent -= 2;
	iprintf(")\n");
}

static void print_word_list(ast_node_t *node)
{
	ast_word_list_t *wl = (ast_word_list_t*)node;

	iprintf("WORD_LIST(\n");
	indent += 2;
	for (int i = 0; i < wl->count; i ++ )
	{
		print(wl->words[i]);
	}
	indent -= 2;
	iprintf(")\n");
}

static void print_word(ast_node_t *node)
{
	ast_word_t *w = (ast_word_t *)node;

	iprintf("WORD('%s')\n", w->word);
}

static void print_var(ast_node_t *node)
{
	ast_var_t *v = (ast_var_t*)node;

	iprintf("VAR(%s)\n", v->var_name);
}

static void print_command_exp(ast_node_t *node)
{
	ast_command_exp_t *ce = (ast_command_exp_t*)node;
	iprintf("COMMAND_EXP(\n");
	indent += 2;
	print(ce->command);
	indent -= 2;
	iprintf(")\n");
}

static void print_dict(ast_node_t *node)
{
	ast_dict_t *dict = (ast_dict_t*)node;
	iprintf("DICT(\n");
	indent += 2;
	for (int i = 0; i< dict->count; i++)
	{
		print(dict->items[i]);
	}
	indent -= 2;
	iprintf(")\n");
}

static void print_list(ast_node_t *node)
{
	ast_list_t *list = (ast_list_t*)node;
	iprintf("LIST(\n");
	indent += 2;
	for (int i = 0; i<list->count; i++)
	{
		print(list->items[i]);
	}
	indent -= 2;
	iprintf(")\n");
}
