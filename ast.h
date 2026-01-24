#ifndef _PASH_AST_H_
#define _PASH_AST_H_

typedef enum ast_kind {
	AST_WORD,
	AST_WORD_LIST,
} ast_kind_t;

typedef struct ast_node {
	ast_kind_t kind;
	struct ast_node *lhs, *rhs;
	union {
		char *str;
	} value;
} ast_node_t;

// allocator
// static inline ast_node_t *ast_node();

ast_node_t *ast_node_generic(ast_kind_t kind, ast_node_t *l, ast_node_t *r);

// TODO one function per node `kind`
ast_node_t *ast_word(char *str);
ast_node_t *ast_word_list(ast_node_t *l, ast_node_t *r);

#endif
