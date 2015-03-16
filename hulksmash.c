#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_SIZE 2048
#define ARG_SIZE 50
#define NAME "-hulksmash-4.20$ "
#define QUIT "-q"
#define DELIMS " \t|\"\0"

bool is_delim(char c){
	if(strchr(DELIMS, c) != NULL){
		return true;
	}
	return false;
}

int main()
{
	size_t ln;
	size_t n;
	
	int args;
	int i, k;
	
	char input[INPUT_SIZE];
	char * tokens[ARG_SIZE];
	char * tok;

	char c;
	char last_quote;
	char tok_buff[INPUT_SIZE];

	enum states {OUT, IN} state = OUT;

	printf("%s ", NAME);
	while(fgets(input, INPUT_SIZE, stdin)){
		ln = strlen(input) - 1;
		if(input[ln] == '\n'){input[ln] = '\0';}
		
		if(strcmp(input, QUIT) == 0){
			break;
		}
		
		// breaks up input into tokens
		k = 0;
		n = 0;
		memset(tok_buff, '\0', INPUT_SIZE);
		for(i = 0 ; i < ln ; ++i){
			c = input[i];
			if(state == OUT){
				if((c == ' ') || (c == '\t') || (c == '\"') || 
				   (c == '\'') || (c == '|')){
					if(k > 0){
						tok_buff[k] = '\0';
						tok = malloc(k);
						memcpy(tok, tok_buff, k+1);
						tokens[n++] = tok;
						memset(tok_buff, '\0', k);
						k = 0;
					}
					if((c == '\"') || (c == '\'')){
						last_quote = c;
						state = IN;
					}else if(c == '|'){
						tok_buff[k++] = '|';	
						tok_buff[k] = '\0';
						tok = malloc(k);
						memcpy(tok, tok_buff, k+1);
						tokens[n++] = tok;
						memset(tok_buff, '\0', k);
						k = 0;
					}
				}else{
					tok_buff[k++] = c;
					if(i == ln - 1){
						tok_buff[k] = '\0';
						tok = malloc(k);
						memcpy(tok, tok_buff, k+1);
						tokens[n++] = tok;
						memset(tok_buff, '\0', k);
						k = 0;
					}
				}
			}else if(state == IN){
				if((c == '\"') || (c == '\'')){
					if(c == last_quote){
						if(k > 0){
							tok_buff[k] = '\0';
							tok = malloc(k);
							memcpy(tok, tok_buff, k+1);
							tokens[n++] = tok;
							memset(tok_buff, '\0', k);
							k = 0;
						}
						state = OUT;
					}else{
						tok_buff[k++] = c;
						if(i == ln - 1){
							printf("ERROR! Must have closing quotes.\n");
							return -1;
						}
					}
				}else if(c == '|'){
					printf("ERROR! Can't have '|' symbol in quotations.\n");
					return -1;
				}else{
					tok_buff[k++] = c;
					if(i == ln - 1){
						printf("ERROR! Must have closing quotations.\n");
						return -1;
					}
				}
			}
		}
		args = n;
		// print all args -- for testing
		for(i = 0 ; i < args ; ++i){
			printf("token %d is '%s'.\n", i, tokens[i]);
		}
	
		printf("%s ", NAME);
	}
	
	return 0;
}
