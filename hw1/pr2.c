#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  while (1) // Reat the process in an infinite loop
  {
    printf("$ "); // Print the prompt
    char ret[1024];
    fgets(ret, sizeof(ret), stdin); // Read a string from the user
    unsigned int l;
    if (ret) // The string is not empty
    {
        l = strlen(ret) - 1;
        if (ret[l] == '\n') ret[l] = '\0'; // Get rid of '\n' with '\0'
    }
    printf("%s\n", ret);
  }
  return 0;
}