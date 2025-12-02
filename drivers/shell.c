#include "shell.h"
#include "io.h"
#include "keyboard.h"
#include "../snake/snake.h"

void shell_print(char* args);
void shell_sum(char* args);
void shell_mult(char* args);
void shell_max(char* args);
void shell_set_bg(char* args);
void shell_set_fg(char* args);
void shell_colour_codes(char* args);
void shell_exit(char* args);
void shell_help(char* args);
void shell_version(char* args);
void shell_clear(char* args);
void shell_snake(char* args);

int string_to_integer(char* str);
void integer_to_char(int num, char* string);
void split_three_args(char* args, char* str1, char* str2, char* str3);

struct Command //Creates the template for the Shell Commands 
{
    const char* name;
    void(*function)(char* args);
    const char* description;
};

static struct Command ShellCommands[] = 
{
    {"print", shell_print, " | Write the string you want to display."},
    {"sum", shell_sum, " | Write three numbers you want added, separated by spaces."},
    {"mult", shell_mult, " | Write three numbers you want to multiply, separated by spaces."},
    {"max", shell_max, " | Write three numbers you want maxed separated by spaces."},
    {"set-bg", shell_set_bg, " | Set the background color. Provide a number from 0-15."},
    {"set-fg", shell_set_fg, " | Set the foreground color. Provide a number from 0-15."},
    {"colour-codes", shell_colour_codes, " | Displays the available colour codes for bg and fg."},
    {"snake", shell_snake, " | Starts the BrainHurt Snake game."},
    {"exit", shell_exit, " | Write exit to quit the shell."},
    {"help", shell_help, " | Write help to see this message."},
    {"version", shell_version, " | Displays the current version of Brainhurt."},
    {"clear", shell_clear, " | Write clear to clear the screen."},
    {0, 0, 0} //Null command to signify end of array
};

void shell_sum(char* args)
{
    char str1[16];
    char str2[16];
    char str3[16];
    split_three_args(args, str1, str2, str3);
    int num1 = string_to_integer(str1);
    int num2 = string_to_integer(str2);
    int num3 = string_to_integer(str3);
    int result = sum_of_three(num1, num2, num3);
    char result_str[16];
    integer_to_char(result, result_str);
    printf(result_str, 1); //Prints the result to the framebuffer
    newline();
}

void shell_mult(char* args)
{
    char str1[16];
    char str2[16];
    char str3[16];
    split_three_args(args, str1, str2, str3);
    int num1 = string_to_integer(str1);
    int num2 = string_to_integer(str2);
    int num3 = string_to_integer(str3);
    int result = mult_of_three(num1, num2, num3);
    char result_str[16];
    integer_to_char(result, result_str);
    printf(result_str, 1); //Prints the result to the framebuffer
    newline();
}

void shell_max(char* args)
{
    char str1[16];
    char str2[16];
    char str3[16];
    split_three_args(args, str1, str2, str3);
    int num1 = string_to_integer(str1);
    int num2 = string_to_integer(str2);
    int num3 = string_to_integer(str3);
    int result = max_of_three(num1, num2, num3);
    char result_str[16];
    integer_to_char(result, result_str);
    printf(result_str, 1); //Prints the result to the framebuffer
    newline();
}

void shell_print(char* args)
{
    printf(args, 1); //Prints to the framebuffer
    newline();
}

void shell_set_bg(char* args)
{
    int temp = string_to_integer(args);
    if (temp < 0 || temp > 15)
    {
        printf("Error: Invalid color code. Must be between 0 and 15.", 1);
        newline();
        return;
    }
    else
    {
        set_background_color(temp); //Sets the background color
    }
}

void shell_set_fg(char* args)
{
    int temp = string_to_integer(args);
    if (temp < 0 || temp > 15)
    {
        printf("Error: Invalid color code. Must be between 0 and 15.", 1);
        newline();
        return;
    }
    else
    {
        set_foreground_color(temp); //Sets the foreground color
    }
}

void shell_colour_codes(char* args)
{
    (void)args; //args not needed
    clear_screen();
    printf("Colour Codes:", 1);
    newline();
    printf("0: Black", 1); newline();
    printf("1: Blue", 1); newline();
    printf("2: Green", 1); newline();
    printf("3: Cyan", 1); newline();
    printf("4: Red", 1); newline();
    printf("5: Magenta", 1); newline();
    printf("6: Brown", 1); newline();
    printf("7: Light Grey", 1); newline();
    printf("8: Dark Grey", 1); newline();
    printf("9: Light Blue", 1); newline();
    printf("10: Light Green", 1); newline();
    printf("11: Light Cyan", 1); newline();
    printf("12: Light Red", 1); newline();
    printf("13: Light Magenta", 1); newline();
    printf("14: Light Brown", 1); newline();
    printf("15: White", 1); newline();
}

void shell_snake(char* args)
{
    (void)args; //args not needed
    start_snake(); //Starts the snake game
}

void shell_exit(char* args)
{
    (void)args; //args not needed
    outb(EXIT_PORT, EXIT_CODE); // Exit QEMU
}

void shell_help(char* args)
{
    (void)args; //args not needed
    printf("Available commands:", 1);
    newline();
    for (int i = 0; ShellCommands[i].name != 0; i++) //Loops through ShellCommands array 
    {
        printf(" - ", 1);
        printf(ShellCommands[i].name, 1); //Prints out each command name
        printf(ShellCommands[i].description, 1); //Prints out each command description
        newline();
    }
}

void shell_version(char* args)
{
    (void)args; //args not needed
    printf("Brainhurt version 1.2.7", 1);
    newline();
}

void shell_clear(char* args)
{
    (void)args; //args not needed
    clear_screen(); //Clears the screen
}

void get_buffer(char* input_buffer)
{
    int index = 0;
    char c;
    while((c = keyboard_pop_head_char()) != 0)
    {
        if (index >= 255) //Exits while loop if buffer full
        {
            //input_buffer[index] = '\0'; //Null terminate the string         <-------------------------------------------- Moved this to line 64 and it worky
            break;
        };
        input_buffer[index] = c; //Stores the character in the buffer
        index++;
    };
    input_buffer[index] = '\0'; //Null terminate the string
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

int string_comparison(char* string1, char* string2)
{
    int index = 0;
    while(string1[index] != '\0' && string2[index] != '\0') //Loops until null terminator
    {
        if (string1[index] != string2[index]) //If characters don't match
        {
            return 0; //Strings are not equal
        }
        index++;
    }
    if (string1[index] == '\0' && string2[index] == '\0') //If both strings end at the same time
    {
        return 1; //Strings are equal
    }
    return 0; //Strings are not equal
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

    //Find and execute the command
    for (int i = 0; ShellCommands[i].name != 0; i++) //Loops through ShellCommands array 
    {
        if (string_comparison(function, (char*)ShellCommands[i].name)) //Compares the input function to the command name
        {
            ShellCommands[i].function(arguments); //Calls the function with the arguments
            return; //Exits the function after executing the command
        }
    }

}

int string_to_integer(char* str)
{
    int result = 0; //Initialises the result variable to zero.
    while (*str >= '0' && *str <= '9') //Loops through each character in the string until a non-digit is found
    {
    result = result * 10 + (*str - '0'); //Multiplies the current value in result by 10 THEN adds the new integer. eg (if str is "5" the result will be 0 * 10 + 5 = 5.)
    str++; //Moves to the next character in the string
    }

    return result;
}

void integer_to_char(int num, char* string)
{
    int index = 0;
    if (num == 0)
    {
        string[0] = '0';
        string[1] = '\0';
        return;
    }

    int temp = num;
    while (temp != 0) //Counts the number of digits in the integer
    {
        temp /= 10;
        index++;
    }
    string[index] = '\0'; //Null terminates the string (we go inreverse)

    while (num != 0) //Converts the integer to a string
    {
        index--;
        string[index] = (num % 10) + '0'; //Gets the last digit and converts it to a character
        num /= 10; //Removes the last digit from the integer
    }
}

void split_three_args(char* args, char* str1, char* str2, char* str3){
    int index = 0;
    while (*args != ' ' && *args != '\0' && *args >= '0' && *args <= '9' && index < 15) //Gets first argument
    {
        str1[index] = *args;
        index++;
        args++;
    }
    str1[index] = '\0';
    if (*args == ' ') //Skips space
    {
        args++;
    }
    index = 0;
    while (*args != ' ' && *args != '\0' && *args >= '0' && *args <= '9' && index < 15) //Gets second argument
    {
        str2[index] = *args;
        index++;
        args++;
    }
    str2[index] = '\0';
    if (*args == ' ') //Skips space
    {
        args++;
    }
    index = 0;
    while (*args != ' ' && *args != '\0' && *args >= '0' && *args <= '9' && index < 15) //Gets third argument
    {
        str3[index] = *args;
        index++;
        args++;
    }
    str3[index] = '\0';
    if (str1[0] == '\0' || str2[0] == '\0' || str3[0] == '\0') //Checks if any arguments are missing
    {
        printf("Error: Missing arguments", 1);
        newline();
        return;
    }
}