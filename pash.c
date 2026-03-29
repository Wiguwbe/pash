#include <stdlib.h>
#include <string.h>

#include "parser.gen.h"
#include "pash.h"
#include "pash_internal.h"

static struct pash *init_pash_generic(void)
{
	struct pash *p = (struct pash*)malloc(sizeof(struct pash));
	if (!p)
		return NULL;

	memset(p, 0, sizeof(struct pash));

	pcc_context_t *pcc_context = pcc_create(p);
	if (!pcc_context)
	{
		free(p);
		return NULL;
	}
	p->pcc_context = pcc_context;

	return p;
}

void pash_free(struct pash *p)
{
	pcc_destroy(p->pcc_context);
	free(p);
}


struct pash *init_pash_file(FILE *file)
{
	struct pash *p = init_pash_generic();
	if (!p)
		return NULL;

	p->reader_type = READER_FILE;
	p->reader.file.file = file;

	return p;
}

struct pash *init_pash_string(char *input)
{
	struct pash *p = init_pash_generic();
	if (!p)
		return NULL;

	p->reader_type = READER_STRING;
	p->reader.string.input = input;

	return p;
}

struct pash *init_pash_callback(int (*getchar)(void *), void *ud)
{
	struct pash *p = init_pash_generic();
	if (!p)
		return NULL;

	p->reader_type = READER_CUSTOM;
	p->reader.custom.callback = getchar;
	p->reader.custom.user_data = ud;

	return p;
}

int pash_getchar(struct pash *p)
{
	int c;
	switch(p->reader_type)
	{
	case READER_FILE:
		c = fgetc(p->reader.file.file);
		if (c == EOF)
			c = -1;
		break;
	case READER_STRING:
		c = *p->reader.string.input;
		if (c == 0)
			c = -1;
		else
			p->reader.string.input ++;
		break;
	case READER_CUSTOM:
		c = p->reader.custom.callback(p->reader.custom.user_data);
	}

	return c;
}

ast_node_t *parse_command(struct pash *p)
{
	ast_node_t *node = NULL;

	int needs_more = pcc_parse(p->pcc_context, &node);

	// do something with `needs_more`?

	return node;
}

void pash_set_debug(struct pash *p, int d)
{
	p->debug = d;
}

static const char *dbg_str[] = {
	"Evaluating rule",
	"Matched rule",
	"Abandoning rule",
};
void pash_debug(
	struct pash *p,
	int level,
	int event,
	char *rule,
	int pos,
	char *buffer,
	int length
)
{
	if (!p->debug)
		return;

	fprintf(
		stderr,
		"%*s%s %s @%zu [%.*s]\n",
		level * 2, "",
		dbg_str[event],
		rule,
		pos,
		length,
		buffer
	);
}
