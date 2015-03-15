#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 1024
#define NAME "-hulksmash-4.20$ "
#define QUIT "-q"

int main()
{
	char input[INPUT_SIZE];
	
	printf("%s ", NAME);
	while(fgets(input, INPUT_SIZE, stdin)){
		size_t ln = strlen(input) - 1;
		if(input[ln] == '\n'){input[ln] = '\0';}

		printf("%s\n", input);
		if(strcmp(input, QUIT) == 0){
			break;
		}

		printf("%s ", NAME);
	}
}
