#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
 
int main(int argc, char *argv[])
{
    int fd = open("file1.txt", O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
    if(fd == -1 ){
        printf("Unable to open the file\n exiting....\n");
        return 0;
    }
    
    int rc = fork();
    if (rc < 0) //fork failed
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) //child
    {
        printf("I am child (pid:%d)\n", (int)getpid());
        int i = 10;
        while (i > 0) {
            write(fd, "Child\n", 6);
            sleep(1);
            i--;
        }
        printf("Write a character from child\n");
    }
    else //parent
    {
        printf("I am parent of %d (pid:%d)\n",
               rc, (int)getpid());
        int j = 10;
        while(j > 0) {
            write(fd, "Parent\n", 7);
            sleep(1);
            j--;
        }
        printf("Write a character from parent\n");
    }
    return 0;
}