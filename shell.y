%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "commandtb.h"
	int yylex();
	void yyerror();

%}

%token NOTOKEN  GREAT  NEWLINE  GREATGREAT  PIPE  AMPERSAND  LESS  GREATAMPERSAND;

%token <string_val> WORD
%start symbol

%union
{
	char *string_val; 
}

%%


symbol:
	{
		PRINTTERMINAL /*	Print */
		Command();	/* Inititalise the command table	*/
	}
	command_list ;

arg_list:
	 WORD arg_list 
	 {
	 	insertArgs($1);	/*	Insert Arguments into the command table	*/
	 }
	| /*empty*/ 
	;
	
cmd_and_args:
	{
		SimpleCommand();	/*	Initialise the command table entry of simple command	*/
	} 
	WORD
	{
		/*printf("Command = %s\n",$2);*/
		insertSimpleCommand($2);	/*	Insert the command into the command table	*/
	} 
	arg_list 
	{
		incSimpleCommand();	/*	Increment the number of simple commands */ 
	}
	;
	
pipe_list:
	pipe_list PIPE cmd_and_args
	| cmd_and_args
	;
	
io_modifier:
	GREATGREAT WORD { addOutputFile($2);setAppend();}
	| GREAT WORD { addOutputFile($2);}
	| GREATGREAT AMPERSAND WORD
	| GREATAMPERSAND WORD
	| LESS WORD { addInputFile($2);}
	;
	
io_modifier_list:
	io_modifier_list io_modifier
	| /*empty*/
	;
	
background_optional:
	AMPERSAND	
	{
		/*	Update the background value	*/
		setBackground();
	}
	| /*empty*/
	;
	
command_line:
	pipe_list io_modifier_list background_optional NEWLINE 
	{
		//printf("Success\n");
		execute();	/*	Execute the command	*/
		Command();	/*	Initalise the command table	*/
	};
	| NEWLINE /*accept empty cmd line*/	
	{
		/*printf("Success\n"); */
	};
	| error NEWLINE{yyerrok;};
	/*error recovery*/

command_list :
	command_list 
	command_line
	{
		PRINTTERMINAL
	}
	| /* command loop*/
	;

%%


int main()
{
	yyparse();
	printf("Exit\n");
	exit(0);
}
