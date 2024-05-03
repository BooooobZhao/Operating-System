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
    while (1)
    {
      printf("Hello, I am child (pid: %d), my parent is %d\n", (int)getpid(), (int)getppid());
      sleep(2); // Wait 2 seconds
    }
  }
  else
  {
    sleep(4); // Wait 4 seconds
    printf("Hello, I am parent (pid: %d)\n", (int)getpid());
  }
  return 0;
}