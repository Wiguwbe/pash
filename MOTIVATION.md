# Motivation

_and a bit of history_

This idea first appeared in the beginning of my professional career.

Back then, we were working a lot with Python and Bash.<br>
Python was good for quick and complex "systems" functions and "commands";<br>
Bash was good at orchestrating and calling those methods.

But there was a clear limitation in the interaction between the two.

Python was able to have complex objects and classes; Bash was ... mostly
strings.<br>
There was a need to transform a "list of strings" into those complex objects.

> Bash now has arrays and associative arrays; it's possible to do complex
> objects with variable "references", though not very safe or easy

We _could_ use Python as a "shell" (a REPL really) ... but having to type all
those parenthesis and quotes felt very little shell-like. Well, as the name
says, it's a _REPL_, not a shell.

Across the land, there was Groovy for Java/JVM, which used slightly less
parenthesis; and PowerShell for the .NET (or whatever) ecosystem which,
honestly, I only looked at the word "Shell", but felt fun-_ish_.

Now, Python is already an interpreted language; it's fairly easy to kick up an
editor or REPL and do powerfull things with it.

But it still felt like there could be something _more_; something more
_functional_ and pipe-like...

So I set out on an adventure to create that for Python.

And I didn't.

---

Jump forward a few years and I'm yet again working with Python (like I had ever
stopped).

This time there's a "structured" "database" with _m-n_ relations all around.

Of course there's a CLI and even a TUI, but for folks like me that like to play
in the shell, a shell was needed.

I tried a FUSE but I couldn't make it make sense. The standard file operations
didn't really map well to the "database".

On the shell-like front, Python's `cmd` (and `cmd2`) seemed promising, probably
enough, but way too simple without variables or control statements (`if`/`for`,
etc...).

`shlex` was already half the language, but I didn't feel like implementing the
parser, specially not in Python (probably too slow).

I looked at full-fledged shell-like solutions, like `xonsh`, oils' `osh` and
`ysh` (which could've been a good candidate), but I couldn't find an easy way
to embed (and sanbox) any of them.

> `YSH` does seem like a good shell/language that could take over in the near
> future.

I settled for `libdash`, which already provided me with an AST of the parsed
commands.

It was going fine.<br>
Things were mostly working; pipes, ands, ors, conditionals ... and then you hit
the multiple input redirection (multiple files in and out).

It's hard to map multiple input "sources" and multiple outputs to Python.
It's already hard to map input _and_ arguments to a Python function.<br>
I think Jinja2 passes the input as first argument?<br>
But how would you pass all the other inputs?<br>
And what about outputs?<br>
Python mostly returns "1" value (may be a tuple tho). Generators are an option,
but implementing multiple generators in a single "function"? That's weird.

There was still the lingering problem of the lack of complex objects.<br>
Yet again there would need to be a transformation between strings and complex
Python objects.

So I gave up on that.<br>
And I decided, yet again, to setup on a quest to create this shell-like
language with complex data objects and none of the "weird" shell features.

Similar to `libdash`, this is to be an embeddable language without a standard
library or commands. It should be good to be used in sandbox environments where
scripting is needed.
