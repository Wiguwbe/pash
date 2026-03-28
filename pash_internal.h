#ifndef _PASH_PASH_INTERNAL_H_
#define _PASH_PASH_INTERNAL_H_

#include <stdio.h>
#include "parser.gen.h"

enum reader_type {
	READER_FILE,
	READER_STRING,
	READER_CUSTOM,
};

// this should be the `auxil` for packcc
struct pash {
	pcc_context_t *pcc_context;
	enum reader_type reader_type;
	union {
		struct {
			FILE *file;
		} file;
		struct {
			// this moves with the input
			char *input;
		} string;
		struct {
			int (*callback)(void *);
			void *user_data;
		} custom;
	} reader;
};

int pash_getchar(struct pash *pash);

#endif
