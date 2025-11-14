#include "shell.h"
#include "io.h"
#include "keyboard.h"

struct Command //Creates the template for the Shell Commands 
{
    const char* name;
    void(*function)(char* args);
};

void shell_print(char* args)
{
    printf(args, 1); //Prints to the framebuffer
}

void shell_exit(char* args)
{
    outb(EXIT_PORT, EXIT_CODE); // Exit QEMU
}

void shell_help(char* args)
{
    printf("Available commands:\n", 1);
    for (int i = 0; ShellCommands[i].name != 0; i++) //Loops through ShellCommands array 
    {
        printf(" - ", 1);
        printf(ShellCommands[i].name, 1); //Prints out each command name
        printf("\n", 1);
    }
}

static struct Command ShellCommands[] = 
{
    {"print", shell_print},
    {"exit", shell_exit},
    {"help", shell_help},
    {0, 0} //Null command to signify end of array
};


