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
    char alias[128];
    char path[1024];
}; typedef struct _COMMAND COMMAND;

void print_prompt(char * name){
	char buffer[MAX_BUFFER];
	getcwd(buffer,MAX_BUFFER);
	fprintf(stdout,"%s:%s$ ",name,buffer);
}

/* This function to read PATH variable from local file
 * if the file does not exist, fail
 */
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

void execute(char ** argv){
  int status;
  pid_t pid;
  if((pid = fork()) == 0){
    if(execvp(*argv,argv) < 0){
			if(execv(*argv,argv) < 0 ){
      	fprintf(stderr,"%s: Could not exec! Perhaps file does not exist.\n",*argv);
			}
    	exit(1); //Error
    }
  } else {
    //Make sure child finishes before parent
    while (wait(&status) != pid);
		printf("Process exited with status: %d\n",status);
  }
}
