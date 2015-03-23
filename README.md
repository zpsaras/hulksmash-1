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
