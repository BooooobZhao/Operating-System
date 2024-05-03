//Xuebao Zhao
//002108354

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SUB_COMMANDS 5 // The maximum number of the subcommand
#define MAX_ARGS 10

struct SubCommand {
    char* line; // Contain the entire sub-command as a C string
    char* argv[MAX_ARGS]; // Arguments
};

struct Command {
    struct SubCommand sub_commands[MAX_SUB_COMMANDS];
    int num_sub_commands; // Indicates how many sub-commands are present
};

void PrintArgs(char** argv)
{
    int i = 0;
    while (argv[i] != NULL) // Stop until NULL
    {
        printf("argv[%d] = '%s'\n", i, argv[i]);
        i++;
    }
}

void ReadArgs(char* in, char** argv, int size)
{
    int num = 0; // The number of arguments
    char* temp; // Store the argument temporarily
    temp = strtok(in, " ");
    while (temp != NULL && num < size) // Still can find a new arguments
    {
        argv[num] = strdup(temp); // Do the copy
        num++;
        temp = strtok(NULL, " "); // Find the next argument
    }
    argv[num] = NULL;
}

void ReadCommand(char* line, struct Command* command)
{
    int num = 0; // The number of arguments
    char* temp; // Store the argument temporarily
    temp = strtok(line, "|");
    while (temp != NULL && num < MAX_SUB_COMMANDS) // Still can find a new arguments and the amount is less than MAX_SUB_COMMANDS
    {
        command->sub_commands[num].line = strdup(temp); // Do the copy
        num++;
        temp = strtok(NULL, "|"); // Find the next argument
    }
    
    command->num_sub_commands = num;
    
    int i = num - 1;
    for (i; i >= 0; i--) // From the end to the front
    {
        char* in;
        in = command->sub_commands[i].line;
        ReadArgs(in, command->sub_commands[i].argv, MAX_ARGS); // Populates the sub-command
    }
}

void PrintCommand(struct Command* command)
{
    int i = 0;
    for (i; i < command->num_sub_commands; i++) // Limit the number of print less than num_sub_commands
    {
        printf("Command %d:\n", i);
        PrintArgs(command->sub_commands[i].argv); // Print each argument
        printf("\n");
    }
}

int main()
{
    char s[200];
    struct Command com;
    // Read a command from the user
    printf("Enter a string: ");
    fgets(s, sizeof(s), stdin);
    unsigned int l;
    if (s) // The string is not empty
    {
        l = strlen(s) - 1;
        if (s[l] == '\n') s[l] = '\0'; // Get rid of '\n' with '\0'
    }

    // Extract arguments and print them
    ReadCommand(s, &com);
    PrintCommand(&com);
    return 0;
}