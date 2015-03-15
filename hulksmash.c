#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 1024
#define ARG_SIZE 1000
#define NAME "-hulksmash-4.20$ "
#define QUIT "-q"

int main()
{
	size_t ln;
	size_t n;
	int args;
	char input[INPUT_SIZE];
	char * tokens[ARG_SIZE];

	printf("%s ", NAME);
	while(fgets(input, INPUT_SIZE, stdin)){
		ln = strlen(input) - 1;
		if(input[ln] == '\n'){input[ln] = '\0';}
		
		if(strcmp(input, QUIT) == 0){
			break;
		}
		
		n = 0;
		for(char * p = strtok(input, " "); p; p = strtok(NULL, " ")){
			if(n > ARG_SIZE){
				break;
			}
			tokens[n++] = p;
		}
		args = n;
		// print all args -- for testing
		for(int i = 0 ; i < args ; ++i){
			printf("token %d is '%s'.\n", i, tokens[i]);
		}
	
		printf("%s ", NAME);
	}
	
	return 0;
}
