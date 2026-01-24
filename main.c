
#include <stdio.h>

#include "ast.h"
#include "pash.tab.h"

int yyerror(ast_node_t *node, char *s) { return 0; }

int main()
{
#ifdef YYDEBUG
	yydebug = 1;
#endif
	while (1)
	{
		ast_node_t *cmd = NULL;

		int rc = yyparse(&cmd);

		//printf("%d %p\n", rc, cmd);

		if (rc)
			break;

		if (!cmd)
			break;

		//printf("GOT A COMMAND\n");

		// execute(cmd)
		// ast_free(cmd)
	}

	return 0;
}
