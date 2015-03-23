# hulksmash
simple shell for OS

Nicholas Bene | Tom Milburn | Zach Psaras

TOKENIZER
---------

tokenizes by white space
delimiters are ' ', '\t', '(double quote)', '(single quote)', '|' and '\n'.
anything in quotes (single or double) is one token.
a single quote inside double quotes will be part of the token (and vice versa)
a pipe is a delimiter and its own token (unless it is in quotes)
if a quote is missing, the matching quote will automatically be added at the end of the input string

FROM TOKENS ONWARDS
-------------------

After we have parsed stdin with the above tokenizer,
the token array is parsed again into an array of structs.
Each struct should represent something of an argv.
This array is then passed to a function that oversees execution
called newexec(COMMAND * commands);
newexec calls execute_parsed2 which in turn executes all commands.
execute_parsed2 also sets up a pipe-stream if necessary and handles
the passing-off of information from one child to another. When this
function has finished, newexec cleans up all dead children(you may
recognize your comment ;] I preferred this usage of wait though I tried
a few others like wait(NULL), (wait(&status) != pid), etc)
Finally, newexec prints The process ID and exit status of every
process caught.

TESTING
-------
Tests were mostly done on a few nonsensical commands
	(e.g.) ls / f | cat
some 'real' ones
	(e.g.) ls . | grep "shell"
some silly ones
	(e.g.) ls / | cat | grep bin
And the entirety of the ones presented in your 'Testing Guidelines' page.

CAVEATS
-------

Unfortunately, we ran into some issues with piping _into_ our shell.
Piping from within the shell should work as expected. If the version you
receive is able to take information piped in from the outside (say, bash),
then great! If not, we never got around to perfecting it.
