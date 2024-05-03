#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_args(int argc, char **argv)
{
    for(int i = 0; i < argc; i++) // For each argument
    {
        printf("argv[%d] = '%s'\n", i, argv[i]); // Do the print
    }
}

int get_args(char *in, char **argv, int max_args)
{
  int num = 0; // The number of arguments
  char *temp; // Store the argument temporarily
  temp = strtok(in, " ");
  while (temp != NULL) // Still can find a new arguments
  {
    argv[num] = strdup(temp); // Do the copy
    num++;
    temp = strtok(NULL, " "); // Find the next argument
  }
  return num;
}

int main()
{
  char s[200];
  char *argv[10];
  int argc;
  
  // Read a string from the user
  printf("Enter a string: ");
  fgets(s, sizeof(s), stdin);
  unsigned int l;
  if (s) // The string is not empty
  {
    l = strlen(s) - 1;
    if (s[l] == '\n') s[l] = '\0'; // Get rid of '\n' with '\0'
  }
  // Extract arguments and print them
  argc = get_args(s, argv, 10);
  print_args(argc, argv);
  return 0;
}