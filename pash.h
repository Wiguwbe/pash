#ifndef _PASH_PASH_H_
#define _PASH_PASH_H_

#include <stdio.h>

#include "ast.h"

struct pash;

// FILE or fd?
struct pash *init_pash_file(FILE *file);
struct pash *init_pash_string(char *input);
struct pash *init_pash_callback(int (*getchar)(void *), void *user_data);

ast_node_t *parse_command(struct pash*);

void pash_free(struct pash *);

#endif
