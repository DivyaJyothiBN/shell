#include "commandtb.h"


/*	Function to initalise all the variables of command structure	*/
void Command()
{
	comm._numberOfAvailableSimpleCommands = MAX_COMMANDS; 
        comm._numberOfSimpleCommands = 0; 
        comm._simpleCommands = (struct SimpleCommand **)malloc(sizeof(struct SimpleCommand *) *  comm._numberOfSimpleCommands); 
	comm._history = (struct history**)malloc(sizeof(struct history*) *  comm._numberOfSimpleCommands);
        comm._outFile = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH); 
        comm._inputFile = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH); ; 
        comm._errFile = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH); 
        comm._background = 0; 

}

/*	Function to initalise all the variables of simple command structure	*/
void SimpleCommand()
{
	int i = comm._numberOfSimpleCommands;
	comm._simpleCommands[i] = (struct SimpleCommand * )malloc(sizeof(struct SimpleCommand ));
	comm._simpleCommands[i]->_numberOfAvailableArguments = MAX_ARGUMENTS ; 
	int numArgs = comm._simpleCommands[i]->_numberOfAvailableArguments; 
        comm._simpleCommands[i]->_numberOfArguments = 0; 
        comm._simpleCommands[i]->_arguments = (char **)malloc(sizeof(char *)*numArgs); 
}

/*	Function to insert command into the command table	*/
void insertSimpleCommand(char *command)
{
	int i = comm._numberOfSimpleCommands;
	comm._simpleCommands[i]->_arguments[0] = (char *)malloc(sizeof(char)*strlen(command)); 
	strcpy(comm._simpleCommands[i]->_arguments[0] , command);
	comm._simpleCommands[i]->_numberOfArguments += 1;
//	printf("Inserted = %s \n",comm._simpleCommands[i]->_arguments[0]);
}

/*	Function to insert arguments of current command into the command table	*/
void insertArgs(char *arg)
{
	int i = comm._numberOfSimpleCommands;
	int j = comm._simpleCommands[i]->_numberOfArguments ;
//	printf("InsertArgs called with %s , %d , %d \n",arg , i , j);
	comm._simpleCommands[i]->_arguments[j] = (char *)malloc(sizeof(char)*strlen(arg)); 
	strcpy(comm._simpleCommands[i]->_arguments[j] , arg);
	comm._simpleCommands[i]->_numberOfArguments += 1;
//	printf("Inserted Arguments = %s \n",comm._simpleCommands[i]->_arguments[j]);	
}

/*	Function to increment the number of simple commands of Command */ 
void incSimpleCommand()
{
	comm._numberOfSimpleCommands += 1 ;
//	printf("Number Simple = %d \n",comm._numberOfSimpleCommands);
}

/*	Function to set the command to background	*/
void setBackground()
{
	comm._background = 1;
}

/*	Function to execute the command present in the current command table	*/
void execute()
{
	int ret; 
		int pid=getpid();
		history.pid=pid;
//	printf("Execute called %d\n" ,comm._numberOfSimpleCommands );
	for(int i = 0; i < comm._numberOfSimpleCommands; i++ ) 
	{ 
		char *command = comm._simpleCommands[i]->_arguments[0];
		if(builtInShellCommand(command))
		{
				executeBuiltInShellCommand(comm._simpleCommands[i]->_arguments,comm._simpleCommands[i]->_numberOfArguments);	
				PRINTTERMINAL 
				continue;
		}

		ret = fork(); 
//		printf("ret = %d \n",ret);
		/*		 Child Process	*/
		if (ret == 0) 
		{ 	
			  execvp(command, comm._simpleCommands[i]->_arguments); 
			  fprintf(stderr , "%s : Command not found\n",command);
			  //perror("execvp"); 
			  _exit(1); 
		} 
		/*	If unable to create a process	*/
	     	else if (ret < 0) 
	     	{ 
		 	 perror("fork"); 
		 	 return; 
		}
		/*	Parent Process	*/
		else
		{
			wait(0);	/*Wait for child process to complete	*/
			PRINTTERMINAL 
		}
	} 
	if (!comm._background)
	{ 
		/*	wait for last process	*/
		waitpid(ret, NULL , 0); 
	} 
}

/*	Function to check if it is built in shell command 	*/
int builtInShellCommand(char *command)
{
	if(strcmp(command,"cd") == 0 || strcmp(command , "help") == 0)
		return 1;
	return 0;	
}

/*	Function to execute the built in shell command	*/
void executeBuiltInShellCommand(char **arguments , int numArguments)
{
	if(strcmp(arguments[0],"cd") == 0)
	{
		printf("hiii");
		
		executeCD(arguments[1]);
	}
	else if(strcmp(arguments[0],"history") == 0)
	{
		printf("here\n");
		executeHistory();
	}
	else
	{
		fprintf(stderr , "Command not implemented\n");
	}
}

/*	Function to execute cd */
void executeCD(char *arguments)
{
		if(arguments == NULL)
		{
			char *home = getenv("HOME");
			setenv("PWD",home,1);
			//printf("home = %s\n",getenv("HOME"));
		}
		else 
		{
			char *argument= (char *)malloc(sizeof(char)*(strlen(arguments) + 1));
			if(arguments[0] == '~')
			{	
				strcpy(argument , getenv("HOME"));
				argument = (char *)realloc(argument , strlen(arguments)+strlen(argument));
				strncat(argument,&(arguments[1]),strlen(arguments+1));
			}
			else
			{
				strcpy(argument , arguments);
			}
			
			if(chdir(argument) == 0)
			{
				char buf[50];
				getcwd(buf,50);
				setenv("PWD",buf,1);
			}
			else
			{
				char *err = (char *)malloc(sizeof(char) * (strlen(argument)+6));
				sprintf(err,"%s %s","cd : ",argument);
				perror(err);
//				printf("\n\n\n");
				free(err);
			}
		}
}


void executeHistory()
 {
printf("Hii\n");
for(int i=0;i<comm._numberOfAvailableSimpleCommands;i++)
{
for(int j=0;j<comm._simpleCommands[i]->_numberOfAvailableArguments;j++)
{
printf("%d. %d  %s",i,history.pid,comm._simpleCommands[i]->_arguments[j] );
}
printf("\n");
}
}


