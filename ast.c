
#include "ast.h"

#include <stdlib.h>
#include <string.h>

// TODO replace `malloc` with something that panics
// (or uses an arena allocator)

static inline ast_node_t *ast_node()
{
	ast_node_t *node = (ast_node_t*)malloc(sizeof(ast_node_t));
	if (!node)
		return NULL;
	memset(node, 0, sizeof(ast_node_t));
	return node;
}

// TODO ast_free

ast_node_t *ast_node_generic(ast_kind_t kind, ast_node_t *l, ast_node_t *r)
{
	ast_node_t *node = ast_node();
	if (!node)
		return NULL;
	node->kind = kind;
	node->lhs = l;
	node->rhs = r;
	return node;
}

ast_node_t *ast_word(char *str)
{
	ast_node_t *node = ast_node();
	if (!node)
		return NULL;
	node->kind = AST_WORD;
	node->value.str = str;	// should be duped already

	return node;
}
