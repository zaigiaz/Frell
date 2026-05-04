#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

#define LENGTH 256
#define SIG_NO_CUR_DIR 1

enum shell_actions {QUIT, HELP};

void other_actions(const char *input);
enum shell_actions StringtoEnum(const char* str);

// TODO :: use strtok() method to split flags and commands
// TODO :: use execvp to handle things
// TODO :: implement signals from gist to handle diff program outputs
// TODO :: implement jobs and job handling

int main() {

  char input[LENGTH];

  printf("------------------------------------------------\n");
  printf("      Welcome to Frell: Friendly Shell!\n       ");
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
    
    //actions you can take
    other_actions(input);

    // flush buffer
    size_t len = sizeof(input);
    
    memset(input, 0x00, len);
  }


  return 0;
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
