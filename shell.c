#include "shell.h"

#define INPUT_SIZE 2048
#define ARG_SIZE 50
#define NAME "-hulksmash-4.20$"
#define QUIT "-q"
#define DELIMS " \t|\"\0"

char * tokens[ARG_SIZE];
char tok_buff[INPUT_SIZE];
enum states {OUT, IN} state = OUT;
int i, k, n;
char * tok;

void add_tok(){
	tok_buff[k] = '\0';
	tok = malloc(k);
	memcpy(tok, tok_buff, k);
	//memcpy(tok, tok_buff, k+1);
	tokens[n++] = tok;
	memset(tok_buff, '\0', k);
	k = 0;

}

int tokenize(char * string, int ln){

	char c, last_quote;

	memset(tok_buff, '\0', INPUT_SIZE);
	k = n = 0;
	for(i = 0 ; i < ln ; ++i){
		c = string[i];
		if(state == OUT){
			if((c == ' ') || (c == '\t') || (c == '\"') || 
			   (c == '\'') || (c == '|')){
				if(k > 0){
					add_tok();
				}
				if((c == '\"') || (c == '\'')){
					last_quote = c;
					state = IN;
				}else if(c == '|'){
					tok_buff[k++] = '|';
					add_tok();	
				}
			}else{	
				tok_buff[k++] = c;
				if(i == ln - 1){
					add_tok();
				}
			}
		}else if(state == IN){
			if((c == '\"') || (c == '\'')){
				if(c == last_quote){
					if(k > 0){
						add_tok();
					}
					state = OUT;
				}else{
					tok_buff[k++] = c;
					if(i == ln - 1){
						printf("ERROR! Must have closing quotes.\n");
						return -1;
					}
				}
			}else{
				tok_buff[k++] = c;
				if(i == ln - 1){
					printf("ERROR! Must have closing quotations.\n");
					return -1;
				}
			}
		}
	}

	return n;
}

int main()
{
	int args, i, ln, j,k;
	
	char input[INPUT_SIZE];

	//printf("%s ", NAME);
	print_prompt(NAME);
	while(fgets(input, INPUT_SIZE, stdin)){
		ln = strlen(input) - 1;
		if(input[ln] == '\n'){input[ln] = '\0';}
		
		if(strcmp(input, QUIT) == 0){
			break;
		}
		
		if((args = tokenize(input, ln)) == -1){
			print_prompt(NAME);
			continue;
		}
	
		tokens[args] = NULL;
		
		// print all args -- for testing
		for(i = 0 ; i < args ; ++i){
			printf("token %d is '%s'.\n", i, tokens[i]);
		}
		
		fprintf(stderr,"#Args: %d\n",args);
		parse_tokens(tokens,args);
		fprintf(stderr,"%s\n",parsed_commands[0][2]);
		for( j = 0 ; parsed_commands[j] != NULL ; j++){
			fprintf(stderr,"Eh?\n");
			for( k = 0 ; parsed_commands[j][k] != NULL ; k++){
				fprintf(stdout,"%s ",parsed_commands[j][k]);
			}
			fprintf(stdout,"\n");
		}

		//Sorry for the lack of inspiration PK ;]
		if(tokens[0] == NULL){
			//Do nothing
		} else if(strcmp(tokens[0],"cd") == 0){
			if(((_CHDIR)funcs[0])(tokens[1]) != 0) {
				fprintf(stderr,"cd: %s: No such directory\n",tokens[1]);
			}
		} else if(strcmp(tokens[0],"exit") == 0) {
			((_EXIT)funcs[1])(0);
		} else {
			execute_parsed(parsed_commands);
		}


		//printf("%s ", NAME);
		print_prompt(NAME);
	}
	
	return 0;
}
