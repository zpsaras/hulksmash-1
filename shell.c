#include "shell.h"

#define INPUT_SIZE 1000000
#define ARG_SIZE 2048
#define MAX_ARGS 50
#define NAME "-hulksmash-4.20$"
#define DELIMS " \t|\"\0"
#define QUOTE_ERROR "ERROR! Must have matching and closing quotes. Assuming missing quote is at end of input.\n"

char * tokens[ARG_SIZE];
char tok_buff[INPUT_SIZE];
enum states {OUT, IN} state = OUT;
int i, k, n;
char * tok;

void add_tok(){
	tok_buff[k] = '\0';
	tok = malloc(k+1);
	memcpy(tok, tok_buff, k+1);
	tokens[n++] = tok;
	memset(tok_buff, '\0', INPUT_SIZE);
	k = 0;

}

void mismatched(char * string, char quote, int z){
	printf(QUOTE_ERROR);
	//input[z] = quote;
	string[z] = quote;
	printf("reading input as: %s\n", string);
	add_tok();
}

int tokenize(char * string, int ln){

	char c, last_quote;


	memset(tok_buff, '\0', INPUT_SIZE);
	k = n = 0;
	state = OUT;
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
					if(i == ln -1){
						mismatched(string, last_quote, ln);
					}
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
						mismatched(string, last_quote, ln);
					}
				}
			}else{
				tok_buff[k++] = c;
				if(i == ln - 1){
					mismatched(string, last_quote, ln);
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
	
	if(!isatty(fileno(stdin))){
		printf("WOAH SHUT IT DOWN\n");
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
	//printf("%s ", NAME);
	print_prompt(NAME);
	while(fgets(input, INPUT_SIZE, stdin)){
		ln = strlen(input) - 1;
		if(input[ln] == '\n'){input[ln] = '\0';}
		
		if((args = tokenize(input, ln)) == -1){
			print_prompt(NAME);
			continue;
		}
		if(args > 50){
			printf("ERROR! Must have maximum of 50 args\n");
			print_prompt(NAME);
			continue;
		}
	
		tokens[args] = NULL;
		
		// print all args -- for testing
		/*
		for(i = 0 ; i < args ; ++i){
			printf("token %d is '%s'.\n", i, tokens[i]);
		}
		*/
		
		parse_tokens2(tokens,args);
		/*
		fprintf(stderr,"%s\n",parsed_commands[0][2]);
		for( j = 0 ; parsed_commands[j] != NULL ; j++){
			fprintf(stderr,"Eh?\n");
			for( k = 0 ; parsed_commands[j][k] != NULL ; k++){
				fprintf(stdout,"%s ",parsed_commands[j][k]);
			}
			fprintf(stdout,"\n");
		}
		*/

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


		//printf("%s ", NAME);
		print_prompt(NAME);
	}
	
	return 0;
}
