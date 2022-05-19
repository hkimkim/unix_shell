#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define MAXARGS 5
#define MAXLINE 8192

// built-in function for change directory
void* cd(char* input) {
  int res;
  char buffer[MAXLINE];

  // Change directory to given input
  res = chdir(input);

  if (res == -1) {
    perror("Error");
  } else {
    getcwd(buffer, MAXLINE);
    printf("Directory changed to: %s\n", buffer);
  }
}

// built-in function for help
void* help(char* input) {
  puts("\nList of Built-in Commands provided:\n"
      "> cd [name of directory]: changes directory to given directory\n"
      "> help : prints all the built-in commands provided\n"
      "> exit : terminate the shell\n"
      "> joke : tells a random joke\n"
      "> And all other unix commands\n");
}


// built-in function to terminate shell
void* Exit(char* input) {
  exit(0);
}

// Built-in function that tells a random joke
void* joke(char* input) {
  int index;
  srand(time(NULL));

  char* joke[] = {"Why can't you trust atoms?\n",
                  "Who did the zombine take to the dance?\n",
                  "What was the baby computer's first word?\n",
                  "Why did the computer take a break?\n",
                  "What do you call a rich elf?\n",
                  "What was Beethoven's favorite fruit?\n"};

  char* answer[] = {"Because they make up everything.\n",
                    "His ghoul-friend.\n",
                    "Data!\n",
                    "It needed to grab a byte to eat!\n",
                    "Welfy.\n",
                    "Ba-na-na-NAAA!\n"};

  index = (rand() % 6);
  printf("\n%s", joke[index]);
  sleep(1);
  printf("> %s\n", answer[index]);
}


// list of built-in commands
char* commands[] = {"cd", "help", "joke", "exit"};

// list of pointers to built-in commands
void* (*command_functions[])(char*) = {cd, help, joke, Exit};

// Wrapper of fork(). Checks for fork errors, quits on error.
pid_t Fork(void) {
  pid_t pid;
  if ((pid = fork()) < 0) {
    fprintf(stderr, "%s: %s\n", "fork error", strerror(errno));
    exit(0);
  }
  return pid;
}

// Wrapper of fgets. Checks and quits on Unix error.
char* Fgets(char *ptr, int n, FILE *stream) {
  char *rptr;
  if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream)) {
    fprintf(stderr, "%s\n", "fgets error");
    exit(0);
  }
  return rptr;
}

// Check given command is a built-in command
int builtin_command(char** argv) {
  int i = 0;

  while (i < 4) {
    int res = strcmp(commands[i], argv[0]);

    if (res == 0) {
      // call the right function.
      command_functions[i](argv[1]);  // some argument
      return 1;
    }
    i++;
  }
  return 0;
}


void eval(char *cmdline) {
  const char* delim = " ";
  char* argv[MAXARGS];  // Argument list execve()
  char buf[MAXLINE];  // Holds modified command line
  pid_t pid;  // Process id

  strcpy(buf, cmdline);

  // Split buf into token and save to argv
  char* rest = buf;
  char* token = strtok_r(buf, delim, &rest);

  int index = 0;
  while (token != NULL) {
    // strip newline at the end of token
    token[strcspn(token, "\n")] = 0;
    argv[index] = token;
    index++;
    token = strtok_r(NULL, delim, &rest);
  }

  // Add NULL to last for execve()
  argv[index] = NULL;

  // Ignore if no arguments given
  if (argv[0] == NULL) {
    return;
  }

  if (!builtin_command(argv)) {
    pid  = Fork();

    // Create child process and execute program in unix
    if (pid == 0) {
      // Create path for execve
      char path[100] = "/bin/";
      strcat(path, argv[0]);
      argv[0] = path;

      // execute command
      int res = execve(argv[0], argv, NULL);

      if (res < 0) {
        printf("Command not found -- Did you mean something else?\n");
      }
      exit(1);
    }
  } else {
    return;
  }

  wait(NULL);
  return;
}

int main() {
  char cmdline[MAXLINE];  // command line buffer
  while (1) {
    // Print command prompt
    printf("k-sea-shell>> ");
    // Read input from user
    Fgets(cmdline, MAXLINE, stdin);
    // If we get the eof, quit the program/shell
    if (feof(stdin)) {
      exit(0);
    }

    // Otherwise, evaluate the input and execute.
    eval(cmdline);
  }
}
