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
#define MAX_JOBS 64

// used to implement jobs
typedef struct {
  pid_t pid;
  char command[LENGTH];
  bool running;
} Job;

enum shell_actions { QUIT, HELP };

void other_actions(const char *input);
void change_directory(const char *input);
bool if_background(const char* input);
enum shell_actions StringtoEnum(const char* str);
void add_job(pid_t pid, const char* cmd);
char* clean_job(const char* input);
void start_sig_handles();
void sigchild_handler(int signum);
void sigint_handler(int sig);
void sigtstp_handler(int sig);

Job jobs[MAX_JOBS];
size_t job_count = 0;
pid_t fg_pid = -1;

// TODO :: review code and code structure, make header file?
// TODO :: implement Piping
// TODO :: implement env variables 

int main() {

  char input[LENGTH];

  printf("------------------------------------------------\n");
  printf("      Welcome to Frell: Friendly Shell!         \n");
  printf("      Commands:      Quit,  Help                \n");
  printf("-----------------------------------------------\n");

  start_sig_handles();

  // Main loop of program
  while(true) {

    char buffer[LENGTH];

    if(getcwd(buffer, LENGTH) == NULL) {
      perror("failed to get current directory");
      exit(SIG_NO_CUR_DIR);
    }

    // print listing
    printf("[%s $] ", buffer);

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
    char *cleaned_cmd = clean_job(input);

    // tokenize and parse input
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

    // check if user hit enter and returned null input
    if (args[0] == NULL) {
      free(cleaned_cmd);
      continue;
    }

    // check for cd and run
    if (args[0] != NULL && strcmp(args[0], "cd") == 0) {
      if (args[1]) {
	change_directory(args[1]);
      } else {
	printf("cd: missing argument\n");
      }
      continue;
    }

    pid_t pid = fork();
    if (pid == 0) {
      execvp(args[0], args);
      perror("exec failed");
      exit(1);
    }     
    else if (pid > 0) {
      int status;
      if (!bg) {
        fg_pid = pid;
        waitpid(pid, &status, 0);
	fg_pid = -1;
	if(!WIFEXITED(status)) { printf("command failed"); }
      } else {
        if (bg) {
          add_job(pid, cleaned_cmd);
          printf("[+] Started background job [%d] %s\n", pid, cleaned_cmd);
        }
      }
    }

    else {
      perror("fork failed");
    }

    // flush buffer and reset    
    size_t len = sizeof(input);
    memset(input, 0x00, len);
    free(cleaned_cmd);
  }
  return 0;
}


// handle all the sigaction structs and thier signals
void start_sig_handles() {
  struct sigaction sa;
  sa.sa_handler = sigchild_handler;
  sigaction(SIGCHLD, &sa, NULL);

  sa.sa_handler = sigint_handler;
  sigaction(SIGINT, &sa, NULL);  
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
  case QUIT: perror("\nquitting shell\n"); exit(0);
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

  if (chdir(input) == 0) {
    printf("succesful change directory\n");
  } else {
    // TODO :: Error checking here for if dir exists or not
    perror("couldnt change directory");
  }
}

void add_job(pid_t pid, const char *cmd) {
  if (job_count >= MAX_JOBS) {
    fprintf(stderr, "too many jobs in the jobs_array");
    return;
  }

  jobs[job_count].pid = pid;
  strncpy(jobs[job_count].command, cmd, LENGTH-1);
  jobs[job_count].running = true;
  job_count++;
}

// if the command is a job then clean the '&' from it
char *clean_job(const char *input) {
  char *cmd_clean = malloc(LENGTH);
  strncpy(cmd_clean, input, LENGTH-1);
  if (cmd_clean[strlen(cmd_clean)-1] == '&') {
    cmd_clean[strlen(cmd_clean)-1] = '\0';
  }
  return cmd_clean;
}

// signal handling job cleanup and exiting
void sigchild_handler(int signum) {
  pid_t pid;

  while ((pid = waitpid(-1, &signum, WNOHANG)) > 0) {
    for(size_t i = 0; i < MAX_JOBS; i++) {
      if(jobs[i].pid == pid) {
	printf("[%zu] Done: %s\n", i + 1, jobs[i].command);
	
	// Shift remaining jobs down and exit out of for-loop
        for (size_t j = i; j < job_count - 1; j++) {
          jobs[j] = jobs[j + 1];
        }
	job_count--;
	break;
      }      
    }
  }
}

// handle sigint signal
void sigint_handler(int sig) {
  if(fg_pid > 0) {
    kill(fg_pid, SIGTERM);
  }
}

// handle sigstp signal 
void sigtstp_handler(int sig) {
  // TODO :: implement handling here
}
