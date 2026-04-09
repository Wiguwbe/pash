
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

// entrypoint
void print_ast_json(ast_node_t *node);

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
static void print_subscripts(ast_node_t *node);

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

	[AST_SUBSCRIPTS]	= print_subscripts,
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

void print_ast_json(ast_node_t *node)
{
	print(node);
}

// helper to print json strings
void print_json_string(char *string)
{
	unsigned char *p = (unsigned char*)string;
	putchar('"');

	for(; *p; p++)
	{
		unsigned char c = *p;

		switch (c)
		{
		case '"':	fputs("\\\"", stdout); break;
		case '\\':	fputs("\\\\", stdout); break;
		case '\n':	fputs("\\n", stdout); break;
		case '\r':	fputs("\\r", stdout); break;
		case '\t':	fputs("\\t", stdout); break;
		case '\b':	fputs("\\b", stdout); break;
		case '\f':	fputs("\\f", stdout); break;

		default:
			if (c < 0x20)
				printf("\\u%04x", c);
			else
				putchar(c);
		}
	}
	putchar('"');
}

/*
 * Implementation
 */

static void print_semi_list(ast_node_t *node)
{
	ast_semi_list_t *sl = (ast_semi_list_t*)node;
	printf("{\"semi_list\": [\n");
	int comma = 0;
	for (int i = 0; i < sl->count; i++)
	{
		if (comma)
			putchar(',');
		else
			comma = 1;
		print(sl->nodes[i]);
	}
	printf("]}\n");
}

static void print_and_or(ast_node_t *node)
{
	ast_and_or_t *ao = (ast_and_or_t*)node;
	printf("{\"and_or\": {\n");
	printf("\"left\":");
	print(ao->left);
	printf(",\"is_and\": %s,\n", ao->is_and ? "true" : "false");
	printf("\"right\":");
	print(ao->right);
	printf("}}\n");
}

static void print_pipeline(ast_node_t *node)
{
	ast_pipeline_t *pl = (ast_pipeline_t*)node;
	printf("{\"pipeline\":[\n");
	int comma = 0;
	for (int i = 0; i < pl->count; i++)
	{
		if (comma)
			putchar(',');
		else
			comma = 1;
		print(pl->nodes[i]);
	}
	printf("]}\n");
}

static void print_command(ast_node_t *node)
{
	ast_command_t *cmd = (ast_command_t*)node;
	printf("{\"command\": {\n");
	if (cmd->assignments)
	{
		printf("\"assignments\":");
		print(cmd->assignments);
	}
	if (cmd->assignments && cmd->inner_command)
		putchar(',');
	if (cmd->inner_command)
	{
		printf("\"inner_command\":");
		print(cmd->inner_command);
	}
	printf("}}\n");
}

static void print_if(ast_node_t *node)
{
	ast_if_t *in = (ast_if_t *)node;
	printf("{\"if\":");
	print(in->condition);
	printf(",\"then\":");
	print(in->then_branch);
	if (in->else_branch)
	{
		printf(",\"else\":");
		print(in->else_branch);
	}
	printf("}\n");
}

static void print_while(ast_node_t *node)
{
	ast_while_t *wn = (ast_while_t *)node;
	printf("{\"while\":");
	print(wn->condition);
	printf(",\"do\":");
	print(wn->body);
	printf("}\n");
}

static void print_for(ast_node_t *node)
{
	ast_for_t *fn = (ast_for_t *)node;
	printf("{\"for\": \"%s\", \"in\":", fn->ident);
	print(fn->list);
	printf(",\"do\":");
	print(fn->body);
	printf("}\n");
}

static void print_case(ast_node_t *node)
{
	// not implemented yet
}

static void print_def(ast_node_t *node)
{
	ast_def_t *def = (ast_def_t*)node;
	printf("{\"def\": \"%s\", \"as\":", def->name);
	print(def->body);
	printf("}\n");
}

static void print_word_list(ast_node_t *node)
{
	ast_word_list_t *wl = (ast_word_list_t *)node;

	printf("{\"word_list\": [\n");
	int comma = 0;
	for (int i = 0; i < wl->count; i++)
	{
		if (comma)
			putchar(',');
		else
			comma = 1;
		print(wl->words[i]);
	}
	printf("]}\n");
}

static void print_word(ast_node_t *node)
{
	ast_word_t *w = (ast_word_t*)node;
	printf("{\"word\":");
	print_json_string(w->word);
	printf("}\n");
}

static void print_var(ast_node_t *node)
{
	ast_var_t *v = (ast_var_t *)node;
	printf("{\"var\": \"%s\"\n", v->var_name);
	if (v->subscripts)
	{
		printf(",\"subscripts\":");
		print_subscripts(v->subscripts);
	}
	printf("}\n");
}

static void print_command_exp(ast_node_t *node)
{
	ast_command_exp_t *ce = (ast_command_exp_t *)node;
	printf("{\"command_exp\":");
	print(ce->command);
	printf("}\n");
}

static void print_dict(ast_node_t *node)
{
	ast_dict_t *dict = (ast_dict_t *)node;
	printf("{\"dict\":[\n");
	int comma = 0;
	// TODO print as list of key/value objects?
	for (int i = 0; i < dict->count; i++)
	{
		if (comma)
			putchar(',');
		else
			comma = 1;
		print(dict->items[i]);
	}
	printf("]}\n");
}

static void print_list(ast_node_t *node)
{
	ast_list_t *list = (ast_list_t *)node;
	printf("{\"list\": [\n");
	for (int i = 0; i<list->count; i++)
	{
		if (i)
			putchar(',');
		print(list->items[i]);
	}
	printf("]}\n");
}

static void print_subscripts(ast_node_t *node)
{
	ast_subscripts_t *subs = (ast_subscripts_t*)node;
	printf("{\"subscripts\": [\n");
	for (int i = 0; i<subs->count; i++)
	{
		if (i)
			putchar(',');
		print(subs->items[i]);
	}
	printf("]}\n");
}
