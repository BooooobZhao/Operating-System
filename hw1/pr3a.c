#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  int rc = fork(); // Create the child process
  if (rc < 0)
  {
    // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  else if (rc == 0)
  {
    // child (new process)
    int input;
    printf("Enter a number: ");
    scanf("%d", &input);
    exit(input); // Return the integer value
  }
  else
  {
    int status;
    // parent goes down this path (original process)
    wait(&status);
    status = status >> 8; // Get the status number
    printf("Child exited with status %d\n", status);
  }
  return 0;
}