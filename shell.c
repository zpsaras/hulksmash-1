#include "shell.h"

#define INPUT_SIZE 1000000
#define ARG_SIZE 2048
#define MAX_ARGS 50
#define NAME "-hulksmash-4.20$"
#define DELIMS " \t|\"\0"
#define QUOTE_ERROR "ERROR! Must have matching and closing quotes. Assuming missing quote is at end of input.\n"

char * tokens[ARG_SIZE];
char tok_buff[INPUT_SIZE];
enum states {OUT, IN, QUOTE} state = OUT;
int i, k, n;
char * tok;

// advances from start to end. returns new length
/*int advance(char * string, int start, int end){
	char c;
	int i;
	
	string[end+2] = '\0';
	for(i = end ; i >= start ; i--){
		string[i+1] = string[i];
	}
	string[start] = '\0';
	return end+1;
}*/

void reset_in(char * string){
	int i;
	for(i = 0 ; i < INPUT_SIZE ; ++i){
		string[i] = '\0';
	} 
}

int tokenize(char * string, int ln){
	int i, args, pos;
	char last;
	
	state = OUT;
	args = pos = 0;
	for(i = 0 ; i < ln ; ++i){
		if(state == IN){
			if(string[i] == ' ' || string[i] == '\0'){
				string[i] = '\0';
				tokens[args] = string + pos;
				args++;
				state = OUT;
			}else if(string[i] == '|'){
				ln = advance(string, i, ln-1);	
				tokens[args] = string + pos;
				args++;
				if(string[i+2] != ' ' && string[i+2] != '\0'){
					ln = advance(string, i+2, ln-1);
				}else{
					string[i+2] = '\0';
				}
				pos = i+1;
				tokens[args] = string + pos;
				args++;
				state = OUT;
				i += 2;
			}else if(string[i] == '\"' || string[i] == '\''){
				last = string[i];
				string[i] = '\0';
				i++;
				tokens[args] = string + pos;
				args++;
				state = QUOTE;
				pos = i;
			}else{
				if(i == ln - 1){
					tokens[args] = string + pos;
					args++;
					state = OUT;
					string[i+1] = '\0';
				}
			}
		}else if(state == OUT){
			if(string[i] == ' ' || string[i] == '\0'){
				string[i] = '\0';	
			}else if(string[i] == '|'){
				pos = i;
				if(string[i+1] != ' ' && string[i+1] != '\0'){
					ln = advance(string, i+1, ln-1);
				}else{
					string[i+1] = '\0';
				}
				tokens[args] = string + pos;
				args++;
				i++;
			}else if(string[i] == '\"' || string[i] == '\''){
				last = string[i];
				i++;
				pos = i;
				state = QUOTE;
			}else{
				pos = i;
				if(i == ln - 1){
					string[ln] = '\0';
					tokens[args] = string + pos;
					args++;
				}
				state = IN;
			}

		}else if(state == QUOTE){
			if(string[i] == last){
				if(string[i+1] == ' '){
					string[i+1] = '\0';
				}
				string[i] = '\0';
				tokens[args] = string + pos;
				state = OUT;
				args++;
				i++;
			}else{
				if(i == ln - 1){
					printf("ERROR: unclosed quotes. automatically adding to end\n");
					string[ln] = '\0';
					tokens[args] = string + pos;
					args++;
				}
			}
		}
	}
	return args;
}


int main()
{
	int args, i, ln, j, k;	
	char input[INPUT_SIZE];
	
	if(!isatty(fileno(stdin))){
		printf("DEBUG\n");
		fgets(input, INPUT_SIZE, stdin);
		ln = strlen(input) -1;
		args = tokenize(input,ln);
		printf("%d\n",args);
		printf("%s\n",input);
		printf("%s\n",tokens[0]);
		tokens[args]=NULL;
		parse_tokens2(tokens,args);
		newexec(parsed_commands2);
		return 0;
	}
	print_prompt(NAME);
	while(fgets(input, INPUT_SIZE, stdin)){
		ln = strlen(input) - 1;
		if(input[ln] == '\n'){input[ln] = '\0';}
		
		if((args = tokenize(input, ln)) == -1){
			print_prompt(NAME);
			continue;
		}
		//test_print(input, ln);
		if(args > 50){
			printf("ERROR! Must have maximum of 50 args\n");
			print_prompt(NAME);
			continue;
		}
		for(i = 0 ; i < args ; ++i){
			printf("token %d: %s\n", i, tokens[i]);
		}
		
		tokens[args] = NULL;	

		parse_tokens2(tokens,args);

		//Sorry for the lack of inspiration PK ;]
		if(tokens[0] == NULL){
			//Do nothing
		} else if(strcmp(tokens[0],"cd") == 0){
			if(tokens[2] != NULL){
				fprintf(stderr,"cd: too many arguments\n");
			}
			if(((_CHDIR)funcs[0])(tokens[1]) != 0) {
				fprintf(stderr,"cd: %s: No such directory\n",tokens[1]);
			}
		} else if(strcmp(tokens[0],"exit") == 0) {
			if(tokens[1]!=NULL){
				((_EXIT)funcs[1])(atoi(tokens[1]));
			} else {
				((_EXIT)funcs[1])(0);
			}
				
		} else {
			newexec(parsed_commands2);
		}
		
		print_prompt(NAME);
		reset_in(input);
	}
	return 0;
}
