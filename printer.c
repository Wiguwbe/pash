
#include "printer.h"
#include "ast.h"

#include <stdlib.h>
#include <string.h>

static void (*printer)(ast_node_t *node) = NULL;

// consider configuring which printers are available and built
// available printers
void print_ast_pretty(ast_node_t *);
void print_ast_json(ast_node_t *);

__attribute__((constructor))
static void select_printer()
{
	// set default
	printer = print_ast_pretty;

	// select correct printer
	char *env_str = getenv("AST_PRINTER");
	if (!env_str)
		return;

	// else, strcmp and set
	if (!strcmp(env_str, "json"))
		printer = print_ast_json;
}

void print_ast(ast_node_t *node)
{
	if (!printer)
		select_printer();

	printer(node);
}

