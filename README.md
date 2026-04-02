# PASH

_P-something A-something SHell_

<!--
    talk about:
    - motivation
        python's `cmd` (limitations)
        libdash (embedding shell)
        shell-like language with complex structs (lists/dicts)
        strings are first class citizens (there are only strings)
    - features and limitations
        objects and lists
        no glob expansion, no vars inside string
        no subshell
        no redirections
        no `case`?
    - quick examples
        show off features
    - how to use
        external API (pash.h)
        AST (ast.h)
    - how to build
        just `make` ?
    - future plans
        "code" as objects (like groovy)
-->

<!-- TODO introduce shell -->

This is a shell-like language with complex objects (dicts and lists).

The idea is to serve as a bare bones language to be embedded and sandboxed.

## Motivation

**TL;DR**: I wanted an embedded and sandboxed shell with complex objects.

If you're interested, read the full history [here](MOTIVATION.md).

## Features and Limitations

The main features of this project are:
- Complex objects -- dictionaries/maps and lists/arrays;
- Variables and command expansions (`$()`) can hold any arbitrary value;
- Be sandboxed and embedded.

With those features, come a _few_ limitations:
- No glob expansion -- we're not working with files;
- No redirection or here-docs -- again, not really working with files;
- No variable expansion in strings -- it would be weird to expand a dict of
   lists inside a string;
- No job control -- including subshells;
- No `case` control structure (yet?).

Like other shell languages, strings are first class citizens and often don't
need quotes.

## Examples

_The tests folder can also be used as examples_

```
my_list=[alpha, bravo, charlie]

for item in $my_list; do
    echo {word: $item} | write-to --append file.txt
done
```

## Using

> The file `pash.h` contains the public API;<br>
> The file `ast.h` contains the AST definitions.

Initialize _pash_
```C
struct pash *pash = init_pash_file(stdin);
```

Read commands and eval them
```
ast_node_t *command = parse_command(pash);

if (command == NULL)
    break;

if (command == empty_line)
    continue;

your_function_to_eval_command(command);

ast_node_free(command)
```

Either evaluating the commands in the application or re-parsing the AST to a
higher level language, you'll probably want an AST visitor.<br>
The `printer-*.c` files can provide a base for that.

## Building

There shouldn't be any external dependencies other than a C compiler and `make`.

Running `make` should output the `.so`, `.a` and a `pash` binary for tests.

## Future

There are some things that are planned or could be implemented, such as:
- code objects/lambdas/closures, similar to groovy;
- arithmetic (`$(( stuff ))`), which is probably a _must have_;
- the `case` control structure;
- variable suffix operations, like `${var[4][key]}`;
- Python bindings/module, similar to how `libdash` does it.

There are some things that are not planned to be implemented at all:
- sub-shells;
- variable/command expansion in strings;
- multi-quote strings;
- file redirection.
