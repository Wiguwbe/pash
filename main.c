
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "pash.h"

#include "printer.h"

void yyerror(void *_unused, char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int main(int argc, char **argv)
{
	FILE *input = stdin;
	if (argc == 2)
	{
		input = fopen(argv[1], "r");
		if (!input)
		{
			perror("failed to open file");
			abort();
		}
	}

	struct pash *pash;
	pash = init_pash_file(input);
	if (!pash)
	{
		perror("wtf");
		return 1;
	}
	{
		char *pcc_debug_env = getenv("PCC_DEBUG");
		if (pcc_debug_env && *pcc_debug_env == '1')
			pash_set_debug(pash, 1);
	}
	while (1)
	{
		ast_node_t *cmd = parse_command(pash);

		if (cmd == empty_line)
			continue;

		fprintf(stderr, "cmd: %p\n", cmd);

		if (!cmd)
			break;

		print_ast(cmd);
		ast_node_free(cmd);
	}

	pash_free(pash);

	if (input != stdin)
		fclose(input);

	return 0;
}
