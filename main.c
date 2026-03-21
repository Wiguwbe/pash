
#include <stdio.h>
#include <stdlib.h>

#include "pash.h"

// if printer_define
#include "pash.tab.h"
#include "printer.h"

void yyerror(void *_unused, char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int main()
{
#ifdef YYDEBUG
	{
		char *yydebug_env = getenv("YYDEBUG");
		if (yydebug_env && *yydebug_env == '1')
			yydebug = 1;
	}
#endif

	struct pash pash;
	if (init_pash(&pash, NULL))
	{
		perror("wtf");
		return 1;
	}
	while (1)
	{
		ast_node_t *cmd = parse_command(&pash);

		printf("cmd: %p\n", cmd);

		if (!cmd)
			break;

		//printf("GOT A COMMAND\n");
		print_ast(cmd);
		ast_node_free(cmd);

		// execute(cmd)
		// ast_free(cmd)
	}

	// TODO free stuff

	return 0;
}
