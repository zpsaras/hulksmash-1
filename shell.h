#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#define MAX_BUFFER 4096
#define COMMAND_ARRAY_SIZE 1024 

typedef int (*_CHDIR)(char *);
typedef void (*_EXIT)(int);
typedef void (*fp)(void);

fp funcs[2] = { (fp)chdir, (fp)exit };

struct _COMMAND {
	char * argv[50];
}; typedef struct _COMMAND COMMAND;

COMMAND * parsed_commands2[1024];

/* For that pretty prompt */
void print_prompt(char * name){
	char buffer[MAX_BUFFER];
	getcwd(buffer,MAX_BUFFER);
	fprintf(stdout,"%s:%s$ ",name,buffer);
}
// advances chars in string by 1 from start to end
int advance(char * string, int start, int end){
	char c;
	int i;
	
	string[end+2] = '\0';
	for(i = end ; i >= start ; i--){
		string[i+1] = string[i];
	}
	string[start] = '\0';
	return end+1;
}

/* V2.0 of tokens[] parser */
void parse_tokens2(char ** tokens, int args){
	int i;
	int j=0;
	int k=0;
	bool FIRST = true;
	parsed_commands2[j] = malloc(sizeof(COMMAND));
	for( i = 0 ; i < args ; i++){
		if ( strcmp(tokens[i],"|") == 0 ){
			parsed_commands2[j]->argv[k] = NULL;
			k=0;
			j++;
			parsed_commands2[j] = malloc(sizeof(COMMAND));
			continue;
		} else {
			parsed_commands2[j]->argv[k] = malloc(sizeof(char*));
			parsed_commands2[j]->argv[k] = tokens[i];
			k++;
		}
	}
	parsed_commands2[j+1] = NULL;
}

/* Actually executes things */
void execute_parsed2(COMMAND ** commands){
	int i,status,pid;
	int fd[2];
	int fd_in = 0;

	for( i = 0 ; commands[i] != NULL ; i++){
		pipe(fd);
		if((pid = fork()) == -1){
				exit(EXIT_FAILURE);
		} else if (pid == 0){
			dup2(fd_in, 0);
			if(commands[i+1] != NULL){
				dup2(fd[1],1);
			}
			close(fd[0]);
			if(commands[i]->argv[0][0] == '.' && commands[i]->argv[0][1] == '/'){
				//Local exec
				if(execv(commands[i]->argv[0],commands[i]->argv)!=0){
					fprintf(stderr,"%s: Failed to exec.\n",commands[i]->argv[0]);
				}
			} else {
                //PATH exec
				if(execvp(commands[i]->argv[0],commands[i]->argv)!=0){
					fprintf(stderr,"%s: Failed to exec.\n",commands[i]->argv[0]);
				}
			}
			exit(EXIT_FAILURE);
		} else {
			close(fd[1]);
			fd_in = fd[0];
		}
	}
}

/* Oversees execution */
void newexec(COMMAND ** commands){
	int pid,status;
	execute_parsed2(commands);
	while ((pid = wait(&status)) != -1)	/* pick up all the dead children */
				fprintf(stderr, "\x1b[31mProcess %u exited with status: %d\x1b[0m\n", pid, WEXITSTATUS(status));
}
