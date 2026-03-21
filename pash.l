%{
#include <stdio.h>
#include <string.h>
#include "pash.tab.h"

char *q_string(char *orig);
%}

%option noyywrap
%option nodefault
%option reentrant
%option bison-bridge
/*%option stack*/

/*%x CMD_SUBST*/
/*%x PARAM_EXP*/

whitespace	[ \t]+
newline		\n

w_first		[^ \t\n|&,:${[(;'"]
w_middle	[^ \t\n]
w_last		[^ \t\n|&,:}\]);'"]

ident		[a-zA-Z_][a-zA-Z0-9_-]*
word		{w_first}({w_middle}*{w_last})?

%%

{whitespace}	/* skip */
"\\"{newline}	/* whitespace, skip */

{newline}	{ return T_NEWLINE; }
"#"[^\n]{newline}	{ return T_NEWLINE; }

"&&"	{ return T_AND; }
"||"	{ return T_OR; }
"|"	{ return T_PIPE; }

"("	{ return T_LPAREN; }
")"	{ return T_RPAREN; }
"{"	{ return T_LBRACE; }
"}"	{ return T_RBRACE; }
"["	{ return T_LBRACK; }
"]"	{ return T_RBRACK; }
";"	{ return T_SEMI; }
":"	{ return T_COLON; }
","	{ return T_COMMA; }

"$("		{ return T_DOLLAR_LPAREN; }	// cmd_subst?
"${"		{ return T_DOLLAR_LBRACE; }	// param_exp
"$"{ident}	{ yylval->T_VAR = strdup(yytext+1); return T_VAR; }
	/* TODO "${" <param_exp> "}" */
"$"		{ return T_DOLLAR; }

	/* TODO keywords */
"if"	{ return T_IF; }
"then"	{ return T_THEN; }
"elif"	{ return T_ELIF; }
"else"	{ return T_ELSE; }
"fi"	{ return T_FI; }
"for"	{ return T_FOR; }
"do"	{ return T_DO; }
"done"	{ return T_DONE; }
"while"	{ return T_WHILE; }
"case"	{ return T_CASE; }
"esac"	{ return T_ESAC; }
"def"	{ return T_DEF; }
"in"	{ return T_IN; }

{ident} { yylval->T_IDENT = strdup(yytext); return T_IDENT; }
{word}	{ yylval->T_WORD = strdup(yytext); return T_WORD; }
\"(\\.|[^"\\])*\"	{ /* quoted strings */
				yylval->T_WORD = q_string(yytext);
				return T_WORD;
			}
\'(\\.|[^\\'])*\'	{
				yylval->T_WORD = q_string(yytext);
				return T_WORD;
			}

.	{
		fprintf(stderr, "Unexpected character: %s\n", yytext);
		return YYerror;
	}

%%

char *q_string(char *orig)
{
	// will _probably_ be shorter that orig
	char *out = (char*)malloc(strlen(orig));

	char *ptr_in = orig;
	char *ptr_out = out;

	char quote = *(ptr_in++);
	while(*ptr_in != quote)
	{
		switch (*ptr_in)
		{
		case '\\':
			if (ptr_in[1] == quote)
			{
				*(ptr_out++) = ptr_in[1];
				ptr_in += 2;
			}
			else
			{
				*(ptr_out++) = *(ptr_in++);
				*(ptr_out++) = *(ptr_in++);
			}
			break;
		default:
			*(ptr_out++) = *(ptr_in++);
		}
	}
	*ptr_out = 0;

	return out;
}
