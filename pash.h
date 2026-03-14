#ifndef _PASH_PASH_H_
#define _PASH_PASH_H_

#include "ast.h"
#include "pash.tab.h"
#include "pash.yy.h"

// TODO make it as an object
struct pash {
	yyscan_t scanner;
	yypstate *parser;
};

// FILE or fd?
int init_pash(struct pash *pash, FILE *file);

ast_node_t *parse_command(struct pash*);

#endif
