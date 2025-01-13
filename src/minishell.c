#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>

#define BRIGHTBLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"

volatile sig_atomic_t interrupted = false;

void signal_handler(int sig){
	interrupted = true;
	write(STDOUT_FILENO, "\n", 1);
}

void change_directory(char *path){
	if(chdir(path) != 0){
		fprintf(stderr, "Error: Cannot change directory to '%s'. %s\n", path, strerror(errno));
	}
}

char *parse(char *input){
	char *arg = (char *) malloc(strlen(input) + 1);
	if(arg == NULL){
		fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
		return NULL;
	}
	char *dst = arg;
	bool in_quotes = false;
	
	while(*input != '\0'){
		if(*input == '"'){
			in_quotes = !in_quotes;
			input++;
			continue;
		}
		if(!in_quotes && *input == ' '){
			break;
		}
		*dst++ = *input++;
	}
	
	while(*input != '\0'){
		if(*input == '"'){
			fprintf(stderr, "Error: Malformed command.\n");
			free(arg);
			return NULL;
		}
		input++;
	}
	
	if(in_quotes){
		fprintf(stderr, "Error: Malformed command.\n");
		free(arg);
		return NULL;
	}
	*dst = '\0';
	return arg;
}

char **tokenize(char *input){
	char **tokens = malloc(sizeof(char *) * 2048);
	if(tokens == NULL){
		fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
		return NULL;
	}
	
	int i = 0;
	char *token = strtok(input, " ");
	while(token != NULL){
		tokens[i++] = token;
		token = strtok(NULL, " ");
	}
	tokens[i] = NULL;
	return tokens;
}

int main(){
	if(signal(SIGINT, signal_handler) == SIG_ERR){
			fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
	}
	char input[4096];
	char cwd[4096];
	bool running = true;
	
	while(running){
		
		if(getcwd(cwd, sizeof(cwd)) == NULL){
			fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
		}
		printf("[%s%s%s]$ ", BRIGHTBLUE, cwd, DEFAULT);
		
		if(fgets(input, sizeof(input), stdin) == NULL){
			if(!feof(stdin)){
				if(!interrupted){
				fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
				}
			}
			input[strlen(input) -1] = '\0';
		} else{
			interrupted = false;
		}
		
		
		input[strlen(input) - 1] = '\0';
				
		if(strcmp(input, "exit") == 0){
			running = false;
			return EXIT_SUCCESS;
		} else if(strncmp(input, "cd", 2) == 0){
			char *arg_start = input + 2;
			while(*arg_start == ' '){
				arg_start++;
			}
			
			if(*arg_start == '\0' || *arg_start == '~'){
				struct passwd *pw = getpwuid(getuid());
				if(pw == NULL){
					fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
					continue;
				}
				char *homedir = pw->pw_dir;
				
				if(strcmp(arg_start, "~") == 0 || strcmp(arg_start, "~/") == 0){
					change_directory(homedir);
				} else{
					char path[PATH_MAX];
					snprintf(path, sizeof(path), "%s%s", homedir, arg_start + 1);
					change_directory(path);
				}
			} else{
				char *arg = parse(arg_start);
				if(arg != NULL){
					change_directory(arg);
					free(arg);
				} 	
			}
		} 	else{
			char **tokens = tokenize(input);
			pid_t pid = fork();
			
			if(pid == 0){
				if(execvp(tokens[0], tokens) == -1){
					fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
				}
				return EXIT_FAILURE;
			} else if(pid > 0){
				int status;
				if(waitpid(pid, &status, 0) == -1){
					fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
				}
			} else{
				fprintf(stderr, "Error: fork() failed.%s.\n", strerror(errno));
			}
			free(tokens);
		}
	}
	return EXIT_SUCCESS;
}
