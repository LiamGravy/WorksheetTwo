#include "snake.h"
#include "randomNumberGenerator.h"
#include "../drivers/io.h"
#include "../drivers/keyboard.h"
#include "../drivers/timerInterrupt.h"

#define TOP_BORDER 0       
#define BOTTOM_BORDER 24 
#define LEFT_BORDER 0
#define RIGHT_BORDER 79
#define NumOfRows 25
#define NumOfCols 80
#define MaxSnakeLength 2000 //Max length of snake in cells (80x25)


unsigned int SnakeHeadPosition;
unsigned int previousPosition;
unsigned int CurrentapplePosition;
unsigned int direction; //0=up, 1=right, 2=down, 3=left
unsigned int SnakeBody[MaxSnakeLength];
unsigned int CurrentSnakeLength = 0;
unsigned int gameRunning = 0;


void start_snake()
{
    clear_screen();
    set_text_colour(FB_GREEN, BG_BLACK);
    printf("Press any key to begin BrainHurt Snake", 1);

    while(keyboard_pop_head_char() == 0)
    {
        // Wait for key press
    }

    clear_screen();
    SnakeHeadPosition = 1000; //Start snake in middle of screen
    SnakeBody[0] = SnakeHeadPosition; // Initialize the first segment of the snake's body
    CurrentapplePosition = randomlyPlaceFood();
    direction = 1; //Start moving right
    gameRunning = 1;
    while (gameRunning)
    {
        handle_input();
        update_game();
        wait();
    }


}


int randomlyPlaceFood()
{
    int applePosition = getRandomNumber(0, 1999); //Get random position for apple in grid 80x25 (2000 cells)
    framebuffer_write_cell(applePosition, '@', FB_RED, BG_BLACK); //Place apple at random position
    return applePosition;
}

void move_snake(unsigned int direction)
{
    int current_row = SnakeHeadPosition / NumOfCols;
    int current_col = SnakeHeadPosition % NumOfCols;

    if (CurrentSnakeLength > 0)
    {
        framebuffer_write_cell(SnakeBody[CurrentSnakeLength - 1], ' ', FB_BLACK, BG_BLACK); //Erase tail segment
    }

    framebuffer_write_cell(SnakeHeadPosition, ' ', FB_BLACK, BG_BLACK); //Erase head before moving

    for (int i = CurrentSnakeLength - 1; i > 0; i--)
    {
        SnakeBody[i] = SnakeBody[i - 1]; //Shift body segments forward
    }

    if (CurrentSnakeLength > 0)
    {
        SnakeBody[0] = SnakeHeadPosition; 
    }

    if (direction == 0 && current_row > TOP_BORDER) //Up
    {
        SnakeHeadPosition -= 80;
    }
    else if (direction == 1 && current_col < RIGHT_BORDER) //Right
    {
        SnakeHeadPosition += 1;
    }
    else if (direction == 2 && current_row < BOTTOM_BORDER) //Down
    {
        SnakeHeadPosition += 80;
    }
    else if (direction == 3 && current_col > LEFT_BORDER) //Left
    {
        SnakeHeadPosition -= 1;
    }
}

void increase_snake_length()
{
    if (CurrentSnakeLength < MaxSnakeLength)
    {
        if (CurrentSnakeLength > 0)
        {
            SnakeBody[CurrentSnakeLength] = SnakeBody[CurrentSnakeLength - 1]; //Add new segment at tail
        } 
        else
        {
            SnakeBody[CurrentSnakeLength] = SnakeHeadPosition; //If first segment, set to head position
        }
    }
    CurrentSnakeLength++;
}

void handle_input() {
    unsigned char c = keyboard_pop_head_char(); 
    
    if (c != 0) {
        // Use WASD for controls
        if (c == 'w' && direction != 2) direction = 0;
        else if (c == 's' && direction != 0) 
        {
            direction = 2;
        }
        else if (c == 'a' && direction != 1) 
        {
            direction = 3;
        }
        else if (c == 'd' && direction != 3) 
        {
            direction = 1;
        }
        else if (c == 'q') 
        {
            gameRunning = 0; // Quit key
        }
    }
}

void drawSnake()
{
    framebuffer_write_cell(SnakeHeadPosition, 'O', FB_GREEN, BG_BLACK); //Draw head

    for (unsigned int i = 0; i < CurrentSnakeLength; i++)
    {
        framebuffer_write_cell(SnakeBody[i], 'o', FB_GREEN, BG_BLACK); //Draw body segments
    }
}

void update_game()
{
    move_snake(direction);
    if (SnakeHeadPosition == CurrentapplePosition)
    {
        increase_snake_length();
        CurrentapplePosition = randomlyPlaceFood();
    }

    for (unsigned int i = 0; i < CurrentSnakeLength; i++)
    {
        if (SnakeHeadPosition == SnakeBody[i])
        {
            gameRunning = 0; // End game on self-collision
        }
    }
    drawSnake();
}

void wait()
{
    unsigned int start_ticks = get_timer_ticks();
    while ((get_timer_ticks() - start_ticks) < 5) {}
}