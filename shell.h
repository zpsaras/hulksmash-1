#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_BUFFER 4096

struct _COMMAND {
    char alias[128];
    char path[1024];
}; typedef struct _COMMAND COMMAND;

/* This function to read PATH variable from local file
 * if the file does not exist, fail
 */
int init_commands(COMMAND ** cmdArray,char ** path){
    FILE * fd = fopen("PATH","r");
    char buffer[MAX_BUFFER];
    char * cptr;
    char alias[128];
    char cmdPath[1024];
    bool tAlias_fPath;
    COMMAND * cmd;
    if(fd < 0){
        fprintf(stderr,"Failed to open PATH. (Perhaps it hasn't been created)\n");
        return -1;
    }
    fgets(buffer,MAX_BUFFER,fd);
    cptr = strtok(buffer,",|");
    while(cptr != NULL){
        if(tAlias_fPath){
            strncpy(alias, cptr, 128);
            tAlias_fPath = false;
        } else {
            strncpy(cmdPath, cptr, 1024);
            cmd = malloc(sizeof(COMMAND));
            //cmd->alias = alias;
            strncpy(cmd->alias,alias,128);
            //cmd->path = cmdPath;
            strncpy(cmd->path,cmdPath,1024);
            register_command(cmdArray,cmd);
            tAlias_fPath = true;  
        }
    }
}

int write_PATH(char ** fpath){
   return 0; 
}

int register_command(COMMAND ** cmdArray, COMMAND * cmd){
   int i = 0;
   int arraySize = sizeof(cmdArray)/sizeof(cmdArray[0]);
   bool flag=false;
   //Find end of COMMAND array
   for( i=0 ; i<arraySize-1 ; i++){
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
