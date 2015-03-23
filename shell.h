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

char ** parsed_commands[50]; //Hard-coded because screw posterity.


/* OLD COMMAND DEF
struct _COMMAND {
    char alias[128];
    char path[1024];
		int fd[2];
}; typedef struct _COMMAND COMMAND;
*/

struct _COMMAND {
	char * argv[50];
	int fd[2];
}; typedef struct _COMMAND COMMAND;

COMMAND * parsed_commands2[1024];

void print_prompt(char * name){
	char buffer[MAX_BUFFER];
	getcwd(buffer,MAX_BUFFER);
	fprintf(stdout,"%s:%s$ ",name,buffer);
}

/* DEPRECATED
 * This function to read PATH variable from local file
 * if the file does not exist, fail
int init_commands(COMMAND ** cmdArray){
    FILE * fd = fopen("PATH","r");
    char buffer[MAX_BUFFER];
    char * cptr;
    char alias[128];
    char cmdPath[1024];
    bool tAlias_fPath=true;
    COMMAND * cmd;
    if(fd == NULL){
        fprintf(stderr,"Failed to open PATH. (Perhaps it hasn't been created)\n");
        return -1;
    }
    fgets(buffer,MAX_BUFFER,fd);
    cptr = strtok(buffer,",|");
    while(cptr != NULL){
        if(tAlias_fPath){
            strncpy(alias, cptr, 128);
            tAlias_fPath = false;
            cptr = strtok(NULL,",|");
        } else {
            strncpy(cmdPath, cptr, 1024);
            cmd = malloc(sizeof(COMMAND));
            //cmd->alias = alias;
            strncpy(cmd->alias,alias,128);
            //cmd->path = cmdPath;
            strncpy(cmd->path,cmdPath,1024);
            register_command(cmdArray,cmd);
            tAlias_fPath = true;  
            cptr = strtok(NULL,",|");
        }
    }
}

int write_PATH(char ** fpath){
   return 0; 
}

int register_command(COMMAND ** cmdArray, COMMAND * cmd){
   int i = 0;
   bool flag=false;
   //Find end of COMMAND array
   for( i=0 ; i<COMMAND_ARRAY_SIZE ; i++){
       if(cmdArray[i]==NULL){
           //Found empty spot
           flag=true;
           break;
       }
   }
   if(flag){
       cmdArray[i] = cmd;
       return 0;
   } else {
       fprintf(stderr, "Failed to add command! (COMMAND upper-limit probably reached?)\n");
       return -1;
   }
}

void print_command_list(COMMAND ** cmdArray){
  int i;
  for( i=0 ; i<COMMAND_ARRAY_SIZE ; i++){
    if(cmdArray[i]!=NULL){
      fprintf(stdout,"%s\t%s\n",cmdArray[i]->alias,cmdArray[i]->path);
    }
  }
}
*/

void execute(char ** argv, int pfd[],int parsed_index){
  int status;
  pid_t pid;
  if((pid = fork()) == 0){
		if(argv[0][0] == '.' && argv[0][1] == '/'){
			if(execv(*argv,argv) < 0 ){
				fprintf(stderr,"%s: Could not exec! Perhaps file does not exist.\n",*argv);
				exit(1);
			}
		} else {
    	if(execvp(*argv,argv) < 0){
      	fprintf(stderr,"%s: Could not exec! Perhaps file does not exist.\n",*argv);
    		exit(1); //Error
    	}
		}
  } else {
    //Make sure child finishes before parent
    while (wait(&status) != pid);
		printf("Process %u exited with status: %d\n",pid,status);
  }
}

void execute2(char ** argv, int pfd[],int parsed_index){
	int status;
	pid_t pid;

	switch (pid = fork()) {
		case 0: //CHILD
			//Check if we have a pipe left
			if(parsed_commands[parsed_index+1] == NULL){
				//Last in list will always be Destination
				dup2(pfd[0],0);
				close(pfd[1]);
				execvp(*argv,argv);
			} else if(parsed_index == 0){
				//First in list will always be Source
				dup2(pfd[1],1);
				close(pfd[0]);
				execvp(*argv,argv);
			} else {
				dup2(pfd[0],0);
				dup2(pfd[1],1);
				execvp(*argv,argv);
				//Everything in between is always Source AND Destination
			}
		default: //PARENT
			break;
		case -1:
			fprintf(stderr,"%s: Could not exec!",*argv);
			break;
	}
}

void execute_parsed(char *** argv){
	int pfd[2];
	int i,status,pid;
	pipe(pfd);

	for( i = 0 ; parsed_commands[i] != NULL ; i++){
		execute2(parsed_commands[i],pfd,i);
	}
	close(pfd[0]);close(pfd[1]);
	while ((pid = wait(&status)) != -1);
	printf("A process exited with status: %d\n",status);

}

void parse_tokens(char ** tokens, int args){
	char * string;
	int i;
	int j=0;
	int k=0;
	bool FIRST = true;
	//parsed_commands[x][y] -> token
	//parsed_commands[x] -> array of tokens
	parsed_commands[j] = malloc(sizeof(char*)*sizeof(char*));
	for( i = 0 ; i < args ; i++ ){
		if( strcmp(tokens[i],"|") == 0 ){
			//fprintf(stdout,"FOUND PIPE\n");
			parsed_commands[j][k] = NULL;
			k=0;
			j++;
			parsed_commands[j] = malloc(sizeof(char*)*sizeof(char*));
			continue;
		} else {
			//fprintf(stdout,"Token is: %s\n",tokens[i]);
			parsed_commands[j][k] = malloc(sizeof(char*));
			parsed_commands[j][k] = tokens[i];
			k++;
		}
	}
	parsed_commands[j+1] = NULL;
}

void parse_tokens2(char ** tokens, int args){
	int i;
	int j=0;
	int k=0;
	bool FIRST = true;
	parsed_commands2[j] = malloc(sizeof(COMMAND));
	for( i = 0 ; i < args ; i++){
		if ( strcmp(tokens[i],"|") == 0 ){
			//fprintf(stdout,"FOUND PIPE\n");
			parsed_commands2[j]->argv[k] = NULL;
			k=0;
			j++;
			parsed_commands2[j] = malloc(sizeof(COMMAND));
			continue;
		} else {
			//fprintf(stdout,"Adding token: %s\n",tokens[i]);
			parsed_commands2[j]->argv[k] = malloc(sizeof(char*));
			parsed_commands2[j]->argv[k] = tokens[i];
			k++;
		}
	}
	parsed_commands2[j+1] = NULL;
	//fprintf(stdout,"PARSED COMMANDS\n");
}

void prepare_pipes(COMMAND ** commands){
	int i;

	fprintf(stdout,"PREPPING PIPES\n");
	for( i = 0 ; commands[i] != NULL ; i++ ){
		pipe(commands[i]->fd);
		if( i == 0 && commands[i+1] == NULL ){ //Only command
			//No need to set up pipes
			fprintf(stdout,"NO SETUP\n");
		} else if( i == 0 ){ /* Always source */
			fprintf(stdout,"SETTING FIRST\n");
			dup2(commands[i]->fd[1], 1);
			close(commands[i]->fd[0]);
			fprintf(stdout,"SET FIRST\n");
		} else if( commands[i+1] == NULL ){ /* Always destination/output to stdout */
			//Takes output from last command as input
			fprintf(stdout,"SETTING LAST\n");
			dup2(commands[i]->fd[0],commands[i-1]->fd[1]);
			close(commands[i]->fd[1]);
			fprintf(stdout,"SET LAST\n");
		} else { //Anywhere in-between is always src&dest
			fprintf(stdout,"SETTING INTERMEDIATE\n");
			dup2(commands[i]->fd[0],commands[i-1]->fd[1]);
			dup2(commands[i]->fd[1],1);
			fprintf(stdout,"INTERMEDIATE SET\n");
		}
	}
	fprintf(stdout,"PIPES PREPPED\n");
}

void close_pipes(COMMAND ** commands){
	int i;
	for( i = 0 ; commands[i] != NULL ; i++ ){
		close(commands[i]->fd[0]);
		close(commands[i]->fd[1]);
	}
}


void execute_parsed2(COMMAND ** commands){
	int i,status,pid;
	int p[2];
	int fd_in = 0;

	for( i = 0 ; commands[i] != NULL ; i++){
		pipe(p);
		if((pid = fork()) == -1){
				exit(EXIT_FAILURE);
		} else if (pid == 0){
			dup2(fd_in, 0);
			if(commands[i+1] != NULL){
				dup2(p[1],1);
			}
			close(p[0]);
			if(commands[i]->argv[0][0] == '.' && commands[i]->argv[0][1] == '/'){
				//Local exec
				if(execv(commands[i]->argv[0],commands[i]->argv)!=0){
					fprintf(stderr,"%s: Failed to exec.\n",commands[i]->argv[0]);
				}
			} else {
				if(execvp(commands[i]->argv[0],commands[i]->argv)!=0){
					fprintf(stderr,"%s: Failed to exec.\n",commands[i]->argv[0]);
				}
			}
			exit(EXIT_FAILURE);
		} else {
			wait(NULL);
			close(p[1]);
			fd_in = p[0];
		}
	}
}
