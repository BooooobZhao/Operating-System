//Xuebao Zhao
//002108354

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
  // Create pipe
  int fds1[2]; // Parent-to-child
  int fds2[2]; // Child-to-parent
  int err1 = pipe(fds1);
  if (err1 == -1) // If create pipe failed, then report error
  {
    perror("pipe1 failed");
    return 1;
  }
  int err2 = pipe(fds2); // If create pipe failed, then report error
  if (err2 == -1) // If create pipe failed, then report error
  {
    perror("pipe2 failed");
    return 1;
  }
  
  int pid = fork(); // Spawn child
  int countChild = 0; // Count the print number of the child
  int countParent = 0; // Count the print number of the parent
  
  if (pid == 0)
  {
    // Child
    char recBuffer; // Used to store the char send from parent
    char sendBuffer = 'c'; // Used to send to parent
    
    close(fds1[1]); // Close the unused write end in parent-to-child pipe
    close(fds2[0]); // Close the unused read end in child-to-parent pipe
    
    while (countChild < 5)
    {
      if (read(fds1[0], &recBuffer, sizeof(recBuffer)) > 0) // If not blocked
      {
        countChild++;
        printf("%d. Child\n", countChild); // Print
        fflush(stdout);
      }
      else
        break;
      write(fds2[1], &sendBuffer, sizeof(sendBuffer)); // Write to Child-to-parent pipe
    }
    close(fds1[0]); // Close the read end in parent-to-child pipe
    close(fds2[1]); // Close the write end in child-to-parent pipe
  }
  else
  {
    // Parent
    char recBuffer; // Used to store the char send from child
    char sendBuffer = 'p'; // Used to send to child
    
    close(fds1[0]); // Close the unused read end in parent-to-child pipe
    close(fds2[1]); // Close the unused write end in child-to-parent pipe
    
    while (countParent < 5)
    {
      countParent++;
      printf("%d. Parent\n", countParent); // Print
      fflush(stdout);
      write(fds1[1], &sendBuffer, sizeof(sendBuffer));
      
      if (read(fds2[0], &recBuffer, sizeof(recBuffer)) > 0)  // If not blocked
        ;
      else
        break;
    }
    close(fds1[1]); // Close the write end in parent-to-child pipe
    close(fds2[0]); // Close the read end in child-to-parent pipe
  }
}