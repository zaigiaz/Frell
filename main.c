#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define LENGTH 256
#define SIG_NO_CUR_DIR 1

enum shell_actions { QUIT, HELP };

void other_actions(const char *input);
void change_directory(const char *input);
bool if_background(const char* input);
enum shell_actions StringtoEnum(const char* str);

// TODO :: implement cd command with chdir() and other non-implemented shell functions
// TODO :: make main fork loop into seperate function / structure
// TODO :: implement jobs and job handling
// TODO :: implement signals from gist to handle diff program outputs
// TODO :: implement env variables 
// TODO :: implement Piping
// TODO :: implement Memory Management

int main() {

  char input[LENGTH];

  printf("------------------------------------------------\n");
  printf("      Welcome to Frell: Friendly Shell!         \n");
  printf("      Commands:      Quit,  Help                \n");
  printf("-----------------------------------------------\n");
  
  // Main loop of program
  while(true) {

    char buffer[LENGTH];

    if(getcwd(buffer, LENGTH) == NULL) {
      perror("failed to get current directory");
      exit(SIG_NO_CUR_DIR);
    }

    // print listing
    printf("[%s ] ", buffer);

    // read input
    if(fgets(input, LENGTH, stdin) == NULL) {
      perror("failed to get standard input");
      exit(1);
    }

    // strip newline from fgets function
    size_t length = strlen(input);
    if(length > 0 && input[length-1] == '\n') { 
      input[length-1] = '\0'; 
    }
    
    // actions you can take (util functions for shell)
    other_actions(input);

    // check to see if background process
    bool bg = if_background(input);

    // fork the process and tokenize for arguments
    pid_t pid = fork();
    if (pid == 0) {
      char *args[64];
      size_t arg_count = 0;
      char *token = strtok(input, " ");

      // get each argument for the command in input
      while (token != NULL && arg_count < 63) {
	args[arg_count++] = token;
	token = strtok(NULL, " ");
      }
      
      // null-terminate array
      args[arg_count] = NULL;

      // execute command
      execvp(args[0], args);
      perror("exec failed");
      exit(1);
    } 
    
    // else if we are parent_process
    else if (pid > 0) {
      int status;
      if (!bg) {
        waitpid(pid, &status, 0);
        printf(WIFEXITED(status) ? "Success\n" : "Failure\n");
      } else {
	printf("child started in background");
      }
    }

    else {
      perror("fork failed");
    }

    // flush buffer and reset
    size_t len = sizeof(input);
    memset(input, 0x00, len);
  }

  return 0;
}

bool if_background(const char* input) {
  if (!input || *input == '\0') return false;

  if (input[strlen(input) - 1] == '&') {
    return true;
  }

  return false;
}

// Take shell input and convert to a command
void other_actions(const char* input) {

  int inputAction = StringtoEnum(input);

  switch (inputAction) {
  case QUIT: perror("\nquitting shell\n"); exit(1);
    break;
  case HELP: printf("\nCurrent shell works with basic UNIX commands, through execvp() function\n");
    break;
  default: 
    break;
  }
}


// Take a string from user-input and convert to int
enum shell_actions StringtoEnum(const char* str) {
  if(strcmp(str, "Quit") == 0) { return QUIT; }
  if(strcmp(str, "Help") == 0) { return HELP; }
  return -1;
}


void change_directory(const char *input) {
  printf("implement function here");

  if (chdir(input) == 0) {
    printf("succesful");
  } else {
    perror("couldnt change directory");
  }

}

