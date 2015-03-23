#include "shell.h"

int main(int argc, char ** argv){
  COMMAND * COMMANDS[COMMAND_ARRAY_SIZE] = { NULL };
	char * myStrings[5] = { "hello", "world", "face", "|", "wow" };
  char * systempath;
  char * execArgs[128];
  int status,i,j;
  pid_t childpid;
  systempath = getenv("PATH");
  fprintf(stdin,"PATH read as: %s\n",systempath);
  if(init_commands(COMMANDS) < 0){
    fprintf(stderr,"Failed init! Exiting\n");
    return -1;
  }
  print_command_list(COMMANDS);

  execArgs[0] = COMMANDS[0]->path;
  execArgs[1] = "..";
  execArgs[2] = NULL;

/*
  if((childpid = fork()) == 0){
    fprintf(stdout,"Forked\n");
    //execl(COMMANDS[0]->path,COMMANDS[0]->alias,(const char*)NULL);
    execvp(*execArgs,execArgs);
  } else {
    while (wait(&status) != childpid);
  }
*/
	parse_tokens(myStrings,5);
	parse_tokens(myStrings,5);
	for( i = 0 ; parsed_commands[i] != NULL ; i++){
		for( j = 0 ; parsed_commands[i][j] != NULL ; j++){
			fprintf(stdout,"%s ",parsed_commands[i][j]);
		}
		fprintf(stdout,"\n");
	}
  //execute(execArgs);
	((_EXIT)funcs[1])(1);
  return 0;
}
