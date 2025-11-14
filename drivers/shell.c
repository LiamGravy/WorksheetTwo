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

//Processing the input

void get_buffer(char* input_buffer)
{
    int index = 0;
    char c;
    while(c = keyboard_pop_head_char())
    {
        if (index >= 255) //Exits while loop if buffer full
        {
            input_buffer[index] = '\0'; //Null terminate the string
            break;
        };
        input_buffer[index] = c; //Stores the character in the buffer
        index++;
    };
}

void split_on_space(char* buffer, char* function)
{
    int index = 0; 
    while(buffer[index] != ' ' && buffer[index] != '\0') //Loops until space or null terminator
    {
        function[index] = buffer[index]; //Stores the character in the function buffer
        index++;
    }
    function[index] = '\0'; //Null terminate the string
}

void split_after_space(char* buffer, char* arguments)
{
    int buffer_index = 0;
    int arg_index = 0;
    while(buffer[buffer_index] != ' ' && buffer[buffer_index] != '\0')
    {
        buffer_index++; //Skips to the space or null terminator
    }

    if (buffer[buffer_index] == ' ') //If space found, move to next character
    {
        buffer_index++;
        while(buffer[buffer_index] != '\0') //Loops until null terminator
        {
            arguments[arg_index] = buffer[buffer_index]; //Stores the character in the arguments buffer
            buffer_index++;
            arg_index++;
        }
        arguments[arg_index] = '\0'; //Null terminate the string
    }
    else if (buffer[buffer_index] == '\0')
    {
        arguments[arg_index] = '\0'; //Null terminate the string if null terminator reached
    }
}

void process_input(void)
{
    char input_buffer[256];
    char function[256];
    char arguments[256];

    //Get the input from the keyboard buffer
    get_buffer(input_buffer);

    //Split the input into function and arguments
    split_on_space(input_buffer, function);
    split_after_space(input_buffer, arguments);

}