%{
	#include <string.h>
	#include "y.tab.h"
%}

%%

\n	{	
		return NEWLINE;	
	}

[\t ] 	{
	/* Discard spaces and tabs */
	}
	
">" 	{
		return GREAT;
	}

"<" 	{
		return LESS;
	}

">>" 	{
		return GREATGREAT;
	}

">&" 	{
		return GREATAMPERSAND;
	}

"|" 	{
		return PIPE;
	}

"&" 	{
		return AMPERSAND;
	}

[^ \t\n][^ \t\n]*	{
				/* Assume that file names have only alpha chars */
				yylval.string_val = strdup(yytext);
				return WORD;
			}

%%

int yywrap (void) {return 1;}

void yyerror(char *err)
{
	printf("%s\n",err);
	exit(0);
}
