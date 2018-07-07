#include "commandtb.h"
#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <libgen.h>
#include <pwd.h>
#include <unistd.h>

/*	Function to initalise all the variables of command structure	*/
void Command()
{
	comm._numberOfAvailableSimpleCommands = MAX_COMMANDS; 
        comm._numberOfSimpleCommands = 0; 
        comm._simpleCommands = (struct SimpleCommand **)malloc(sizeof(struct SimpleCommand *) *  comm._numberOfSimpleCommands);
	// comm._history = (struct history**)malloc(sizeof(struct history *) * 10); 
        comm._outFile = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH); 
        comm._inputFile = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH); 
        comm._errFile = (char *)malloc(sizeof(char)*MAX_FILE_LENGTH); 
        comm._background = 0; 
	comm.append = 0;

}

/*	Function to initalise all the variables of simple command structure	*/
void SimpleCommand()
{
	int i = comm._numberOfSimpleCommands;
	comm._simpleCommands[i] = (struct SimpleCommand * )malloc(sizeof(struct SimpleCommand ));
	//comm._history[i] = (struct history * )malloc(sizeof(struct history ));
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
}

/*	Function to insert arguments of current command into the command table	*/
void insertArgs(char *arg)
{
	int i = comm._numberOfSimpleCommands;
	int j = comm._simpleCommands[i]->_numberOfArguments ;
	comm._simpleCommands[i]->_arguments[j] = (char *)malloc(sizeof(char)*strlen(arg)); 
	strcpy(comm._simpleCommands[i]->_arguments[j] , arg);
	comm._simpleCommands[i]->_numberOfArguments += 1;
}

/*	Function to increment the number of simple commands of Command */ 
void incSimpleCommand()
{
	comm._numberOfSimpleCommands += 1 ;
}

/*	Function to set the command to background	*/
void setBackground()
{
	comm._background = 1;
}

void setAppend()
{
	comm.append = 1;
}
/*	Function to execute the command present in the current command table	*/
void execute()
{
	int ret; 
	int fdin;
	int fdout;
	int tmpin = dup(0);
	int tmpout = dup(1);
	
	if(comm._inputFile != NULL)
	{
		fdin = open(comm._inputFile , O_RDONLY);
	}
	else
	{
		fdin = dup(tmpin);
	}
	
	for(int i = 0; i < comm._numberOfSimpleCommands; i++ ) 
	{ 
		dup2(fdin , 0);
		close(fdin);
		//printf("I = %d\n",i);
		if(i == comm._numberOfSimpleCommands -1)
		{
			if(comm._outFile != NULL)
			{
				if(comm.append)
					fdout = open(comm._outFile , O_WRONLY|O_APPEND);
				else
		


			fdout = open(comm._outFile , O_CREAT | O_TRUNC | O_WRONLY);
			}
			else
			{
				fdout = dup(tmpout);
			}
		}
		else
		{
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
			//printf("Pipe opened\n");	
		}
		dup2(fdout , 1);
		close(fdout);
		//printf("i = %d\n",i);
		//printf("%s pere\n",comm._simpleCommands[i]->_arguments[0]);
		
		//printf("\ngjyjyha\n");
		//char *command =	malloc(sizeof(char)*(strlen(comm._simpleCommands[i]->_arguments[0])+1));
		//printf("\ngjyjyha\n");	
		char * command=comm._simpleCommands[i]->_arguments[0];
		//printf("\ngjyjyhb\n");
		command[strlen(comm._simpleCommands[i]->_arguments[0])]='\0';	
		//printf("\ngjyjyh\n");
			
		if(builtInShellCommand(command))
		{	
				//comm._history[i]->_arguments = comm._simpleCommands[i]->_arguments[0];
				//printf("\nIam here\n");
				//printf("%d\n",count);
				comm._history[count]._arguments=(char *)malloc(sizeof(char)*strlen(command));
				strcpy(comm._history[count]._arguments,command);
				//printf("\nhistorty %s\n",comm._history[count]._arguments);
				
				executeBuiltInShellCommand(comm._simpleCommands[i]->_arguments,comm._simpleCommands[i]->_numberOfArguments);	
				count++;
				continue;
		}

		ret = fork(); 
		int pid= getpid();
		comm._history[count].pid=pid;
		/*		 Child Process	*/
		if (ret == 0) 
		{ 	
			//printf("pwd");
			
			execvp(command, comm._simpleCommands[i]->_arguments); 
				
			  fprintf(stderr , "%s : Command not found\n",command);
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
			wait(0);
			comm._history[count]._arguments=(char *)malloc(sizeof(char)*strlen(command));
			strcpy(comm._history[count]._arguments,command);
			count++;
			/*Wait for child process to complete	*/
			//printf("i = %d\n",i);
			//PRINTTERMINAL 
		}
	}
	dup2(tmpin , 0); 
	dup2(tmpout , 1);
	close(tmpin);
	close(tmpout);
	 
	if (!comm._background)
	{ 
		/*	wait for last process	*/
		waitpid(ret, NULL , 0); 
	}
	else
	{
		printf("[%d] %d\n",1,ret);
	}
}

/*	Function to check if it is built in shell command 	*/
int builtInShellCommand(char *command)
{	
	if(strcmp(command,"cd") == 0 || strcmp(command , "help") == 0 || strcmp(command,"history") == 0|| strcmp(command,"pss")==0||strcmp(command,"systeminfo")==0 )
		return 1;
	return 0;	
}

/*	Function to execute the built in shell command	*/
void executeBuiltInShellCommand(char **arguments , int numArguments)
{	
	
	if(strcmp(arguments[0],"cd") == 0)
	{	
		int pid= getpid();
		comm._history[count].pid=pid;
		//count++;
		//printf("herer");
		
		executeCD(arguments[1]);
	}
	else if(strcmp(arguments[0],"help") == 0)
	{	
		int pid= getpid();
		comm._history[count].pid=pid;
		executeHelp(arguments[1]);
	
	}
	else if(strcmp(arguments[0],"history") == 0)
	{
		//printf("here\n");
		executeHistory();
	}
	else if(strcmp(arguments[0],"pss") == 0)
	{
		//printf("here\n");
		executepss();
	}
	else if(strcmp(arguments[0],"systeminfo") == 0)
	{
		//printf("here\n");
		executesysteminfo();
	}
	else
	{
		fprintf(stderr , "Command not implemented\n");
	}
}

/*	Function to execute cd */
void executeCD(char *arguments)
{
	char *argument;
		if(arguments == NULL)
		{
			sprintf(argument , "%s",getenv("HOME"));
		}
		else 
		{
			argument= (char *)malloc(sizeof(char)*(strlen(arguments) + 1));
			if(arguments[0] == '~')
			{	
				strcpy(argument , getenv("HOME"));
				sprintf(argument , "%s%s",argument , &(arguments[1]));
			}
			else
			{
				strcpy(argument , arguments);
			}
			
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
				free(err);
			}
}

void executeHelp(char *arguments)
{
	if(arguments == NULL)
	{
		char *help = "HELP COMMAND\nExisting Shell Commands :\nhelp\ncd\npwd\n";
		fprintf(stdout , "%s",help);
	}
}



void addOutputFile(char *file)
{
	sprintf(comm._outFile,"%s",file);
}

void addInputFile(char *file)
{
	sprintf(comm._inputFile,"%s",file);
}

void executeHistory()
 {
//printf("Hii\n");
printf("command\t\t pid\n");
for(int i=0;i<count;i++)
{
printf("%s\t\t%d\n",comm._history[i]._arguments,comm._history[i].pid);
}
printf("\n");

}

void executesysteminfo()
{
FILE *fp;
	struct dirent *entry;
	char path[1024];

	FILE* f = fopen ("/proc/cpuinfo", "r");
	FILE* p = fopen ("/proc/meminfo", "r");
	FILE* s = fopen ("/proc/partitions", "r");
	
	DIR *dirp;
	
	char *a = malloc (256);
	char *b = malloc (128);
	char *c = malloc (128);
	
	printf("****************************************************************************\n");
	printf("CPU information:\n");
	char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), f)) {
        i++;
        if(i == 7 )
        {	
            printf(" %s", line);   
        }
    }
	fclose (f);
printf("****************************************************************************\n");
printf("Memory information:\n");
char line1[256];
    int j= 0;
    while (fgets(line1, sizeof(line), p)) {
        j++;
	
	if(j== 1 )
        {	
            printf("  %s", line1);   
        }
	if(j== 2 )
        {	
            printf("  %s", line1);   
        }

        if(j== 3 )
        {	
            printf("  %s", line1);   
        }
	
    }
	fclose (p);

char line3[256];
    int k= 0;
printf("****************************************************************************\n");
printf("Disk info:\n");
    while (fgets(line3, sizeof(line), s)) {
        k++;
            printf(" %s", line3);   
    }
	fclose (s);
printf("****************************************************************************\n");
}

void executepss()
{
FILE *fp;
DIR *dirp;
	
	struct dirent *entry;
	char path[1024];
char line[256];
printf("****************************************************************************\n");
printf("Process info:\n");
dirp = opendir ("/proc/");
while ((entry = readdir (dirp)) != NULL)
  	{
		//printf("entered while lopp");
		if (check_if_number (entry->d_name))
    		{
			strcpy(path,"/proc/");
			strcat(path,entry->d_name);
			//printf("hi\n");
			
			strcat(path,"/status");
			//printf("%s\n",path);
			
			fp=fopen(path,"r");
	char line4[256];
   		 int l= 0;
	printf("****************************************************************************\n");
	printf("Process info:\n");
    	while (fgets(line4, sizeof(line), fp)) {
        l++;
		if(l>1 && l<11)
            		printf(" %s", line4);   
    	}
	fclose (fp);

	}
}

}


int check_if_number (char *str)
{
  int i;
  for (i=0; str[i] != '\0'; i++)
  {
    if (!isdigit (str[i]))
    {
      return 0;
    }
  }
  return 1;
}

