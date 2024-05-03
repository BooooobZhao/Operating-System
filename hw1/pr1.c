#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SUB_COMMANDS 5 // The maximum number of the subcommand
#define MAX_ARGS 10

struct SubCommand
{
  char *line;           // Contain the entire sub-command as a C string
  char *argv[MAX_ARGS]; // Arguments
};

struct Command
{
  char *stdin_redirect = NULL;
  char *stdout_redirect = NULL;
  int background = 0;
  struct SubCommand sub_commands[MAX_SUB_COMMANDS];
  int num_sub_commands; // Indicates how many sub-commands are present
};

void PrintArgs(char **argv)
{
  int i = 0;
  while (argv[i] != NULL)
  {
    if (strcmp(argv[i], "<") == 0 || strcmp(argv[i], ">") == 0 || strcmp(argv[i], "&") == 0)
      break;
    printf("argv[%d] = '%s'\n", i, argv[i]);
    i++;
  }
}

void ReadArgs(char *in, char **argv, int size)
{
  int num = 0; // The number of arguments
  char *temp;  // Store the argument temporarily
  temp = strtok(in, " ");
  while (temp != NULL && num < size) // Still can find a new arguments
  {
    argv[num] = _strdup(temp); // Do the copy
    num++;
    temp = strtok(NULL, " "); // Find the next argument
  }
  argv[num] = NULL;
}

void ReadCommand(char *line, struct Command *command)
{
  int num = 0;
  char *temp;
  temp = strtok(line, "|");
  while (temp != NULL && num < MAX_SUB_COMMANDS)
  {
    command->sub_commands[num].line = _strdup(temp);
    num++;
    temp = strtok(NULL, "|");
  }
  command->num_sub_commands = num;

  for (int i = 0; i < num; i++)
  {
    ReadArgs(command->sub_commands[i].line, command->sub_commands[i].argv, MAX_ARGS);
  }
}

void PrintCommand(struct Command *command)
{
  for (int i = 0; i < command->num_sub_commands; i++)
  {
    printf("Command %d:\n", i);
    PrintArgs(command->sub_commands[i].argv);
    printf("\n");
  }
  printf("\n");
  printf("Redirect stdin: %s\n", command->stdin_redirect);
  printf("Redirect stdout: %s\n", command->stdout_redirect);
  if (command->background == 1)
    printf("Backgound: yes\n");
  else
    printf("Backgound: no\n");
}

void ReadRedirectsAndBackground(struct Command *command)
{
  int i = command->num_sub_commands - 1;
  for (i; i >= 0; i--)
  {
    int j = 0;
    char *temp;
    temp = command->sub_commands[i].argv[j];
    while (temp != NULL)
    {
      if (strcmp(temp, "&") == 0)
        command->background = 1;
      else if (strcmp(temp, ">") == 0)
      {
        j++;
        temp = command->sub_commands[i].argv[j];
        if (temp != NULL)
          command->stdout_redirect = _strdup(temp);
        else
        {
          printf("Error! No output file found!");
          return;
        }
      }
      else if (strcmp(temp, "<") == 0)
      {
        j++;
        temp = command->sub_commands[i].argv[j];
        if (temp != NULL)
          command->stdin_redirect = _strdup(temp);
        else
        {
          printf("Error! No input file found!");
          return;
        }
      }
      j++;
      temp = command->sub_commands[i].argv[j];
    }
  }
}

int main()
{
  char s[200];
  Command *com = new Command();
  // Read a command from the user
  printf("Enter a string: ");
  fgets(s, sizeof(s), stdin);
  unsigned int l;
  if (s) // The string is not empty
  {
    l = strlen(s) - 1;
    if (s[l] == '\n')
      s[l] = '\0'; // Get rid of '\n' with '\0'
  }

  // Extract arguments and print them
  ReadCommand(s, com);
  ReadRedirectsAndBackground(com);
  PrintCommand(com);
  return 0;
}