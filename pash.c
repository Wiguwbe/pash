

#include <stdio.h>

#include "pash.h"
#include "pash.tab.h"
#include "pash.yy.h"

int init_pash(struct pash *pash, FILE *file)
{
	if (yylex_init(&pash->scanner))
		return 1;

	pash->parser = yypstate_new();
	if (!pash->parser)
	{
		yylex_destroy(pash->scanner);
		return 1;
	}

	// set file
	if (file)
		yyset_in(file, pash->scanner);

	return 0;
}

// TODO free pash

ast_node_t *parse_command(struct pash *pash)
{
	YYSTYPE lval;
	int token;
	ast_node_t *node;
	int status = YYPUSH_MORE;

	while (status == YYPUSH_MORE)
	{
		// where does lval go?
		token = yylex(&lval, pash->scanner);

		status = yypush_parse(pash->parser, token, &lval, &node);

		if (status == 1)
			// parse error
			return NULL;
		if (status == 2)
			// allocation error
			return NULL;
	}

	return node;
}
