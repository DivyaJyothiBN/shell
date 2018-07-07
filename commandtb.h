// Command Data Structure 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline.h>
#include <history.h>

char* input, shell_prompt[100];

#define MAX_COMMANDS 5
#define MAX_ARGUMENTS 5
#define MAX_FILE_LENGTH 15
#define MAX_COMMAND_LENGTH 20

#define pcs "\e[1;34m%s%s\e[0m\x1b[32m%s%s\e[0m"
#define PRINTTERMINAL snprintf(pcs, getenv("USER"),"@",getenv("PWD"),"$"); 
// Describes a simple command and arguments 
struct SimpleCommand { 
        // Available space for arguments currently preallocated 
        int _numberOfAvailableArguments; 
 
        // Number of arguments 
        int _numberOfArguments; 
 
        // Array of arguments 
        char ** _arguments; 
 
}; 

struct history {
	int pid;
	char * _arguments;
		};
struct reminder{
	
		};
// Describes a complete command with the multiple pipes if any 
// and input/output redirection if any. 
struct Command { 
        int _numberOfAvailableSimpleCommands; 
        int _numberOfSimpleCommands; 
        struct SimpleCommand ** _simpleCommands;
	struct history _history[20];
        int append; 
        char * _outFile; 
        char * _inputFile; 
        char * _errFile; 
        int _background; 
 
};


	
      	void prompt(); 
        void print(); 
        void execute(); 
        void clear(); 
 
        void Command(); 
	
	void SimpleCommand();
	
	void insertSimpleCommand(char *);
	void insertArgs(char *);

int builtInShellCommand(char *command);
void executeBuiltInShellCommand(char **arguments, int numArguments);

void incSimpleCommand();
void setBackground();
void setAppend();
void addOutputFile(char *file);
void addInputFile(char *file);

void executeCD(char *arguments);
void executeHelp(char *arguments);

/*        void insertSimpleCommand( SimpleCommand * simpleCommand ); 
void insertArgument( struct SimpleCommand , char * argument); 
struct  SimpleCommand(); 
 */
 /*       static Command _currentCommand; 
        static SimpleCommand *_currentSimpleCommand; */

struct Command comm;
 
