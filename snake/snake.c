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
#define BG_COLOUR BG_DARK_GREY
#define FB_COLOUR FB_LIGHT_GREEN

#define NUMBER_OF_APPLES 10
#define GAME_SPEED 3 //Lower is faster

 int SnakeHeadPosition;
 int previousPosition;
 int CurrentapplePosition;
unsigned int direction; //0=up, 1=right, 2=down, 3=left
int SnakeBody[MaxSnakeLength];
int ApplePositions[NUMBER_OF_APPLES];
unsigned int CurrentSnakeLength = 0;
unsigned int gameRunning = 0;

void start_snake()
{
    clear_screen();
    set_text_colour(FB_COLOUR, BG_COLOUR);
    printf("Press any key to begin BrainHurt Snake", 1);

    while(keyboard_pop_head_char() == 0 && EnterPressed == 0)
    {
        // Wait for key press
    }
    keyboard_on = 0; // Disable keyboard input for the game
    snakeClean();
    set_background_color(BG_COLOUR);
    SnakeHeadPosition = 500; //Start snake in middle of screen
    SnakeBody[0] = SnakeHeadPosition; // Initialize the first segment of the snake's body
    for (int i = 0; i < NUMBER_OF_APPLES; i++)
    {
        ApplePositions[i] = randomlyPlaceFood();
    }
    direction = 1; //Start moving right
    gameRunning = 1;
    while (gameRunning)
    {
        handle_input();
        update_game();
        wait();
    }
    clear_screen();
    set_text_colour(FB_RED, BG_BLACK);
    printf("Game Over! Your score: ", 1);
    fb_write_integer(CurrentSnakeLength);
    newline();
    set_text_colour(FB_GREEN, BG_BLACK);

}

int randomlyPlaceFood()
{
    int applePosition = getRandomNumber(0, 1999); //Get random position for apple in grid 80x25 (2000 cells)
    framebuffer_write_cell(applePosition, '@', FB_RED, BG_COLOUR); //Place apple at random position
    return applePosition;
}

void move_snake(unsigned int direction)
{

    previousPosition = SnakeHeadPosition;

    int current_row = SnakeHeadPosition / NumOfCols;
    int current_col = SnakeHeadPosition % NumOfCols;

    if (CurrentSnakeLength > 0)
    {
        framebuffer_write_cell(SnakeBody[CurrentSnakeLength - 1], ' ', FB_COLOUR, BG_COLOUR); //Erase tail segment
    }

    framebuffer_write_cell(SnakeHeadPosition, ' ', FB_COLOUR, BG_COLOUR); //Erase head before moving

    for (int i = CurrentSnakeLength - 1; i > 0; i--)
    {
        SnakeBody[i] = SnakeBody[i - 1]; //Shift body segments forward
    }

    if (CurrentSnakeLength > 0)
    {
        SnakeBody[0] = SnakeHeadPosition; 
    }

if (direction == 0) //Up
    {
        if(current_row > TOP_BORDER)
            SnakeHeadPosition -= 80;
        else
            snakeEnd();
    }
    else if (direction == 1) //Right
    {
        if(current_col < RIGHT_BORDER)
            SnakeHeadPosition += 1;
        else
            snakeEnd();
    }
    else if (direction == 2) //Down
    {
        if(current_row < BOTTOM_BORDER)
            SnakeHeadPosition += 80;
        else
            snakeEnd();
    }
    else if (direction == 3) //Left
    {
        if(current_col > LEFT_BORDER)
            SnakeHeadPosition -= 1;
        else
            snakeEnd();
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
            SnakeBody[CurrentSnakeLength] = previousPosition; //If first segment, set to head position
        }
        CurrentSnakeLength++;
    }
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
            snakeEnd(); 
        }
    }
}

void drawSnake()
{
    framebuffer_write_cell(SnakeHeadPosition, 'O', FB_COLOUR, BG_COLOUR); //Draw head

    for (unsigned int i = 0; i < CurrentSnakeLength; i++)
    {
        framebuffer_write_cell(SnakeBody[i], 'o', FB_COLOUR, BG_COLOUR); //Draw body segments
    }
}

void update_game()
{
    move_snake(direction);

    for (int i = 0; i < NUMBER_OF_APPLES; i++)
    {
        if (SnakeHeadPosition == ApplePositions[i])
        {
            increase_snake_length();
            ApplePositions[i] = randomlyPlaceFood();
        }
    }

    for (unsigned int i = 0; i < CurrentSnakeLength; i++)
    {
        if (SnakeHeadPosition == SnakeBody[i])
        {
            snakeEnd(); // End game on self-collision
        }
    }
    drawSnake();
}

void wait()
{
    unsigned int start_ticks = get_timer_ticks();
    while ((get_timer_ticks() - start_ticks) < GAME_SPEED) {}
}

void snakeClean() 
{
    clear_screen();
    for (unsigned int i = 0; i < 80 *25; i++) //Loops through all cells of the framebuffer (80 columns x 25 rows)
    {
    framebuffer_write_cell(i, ' ', 0, BG_COLOUR); 
    }
    framebuffer_move_cursor(0); 
}

void snakeEnd()
{
    gameRunning = 0;
    keyboard_on = 1; // Re-enable keyboard input
    
    for (int i = 0; i < MaxSnakeLength; i++)
    {
        SnakeBody[i] = 0;
    }
    for (int i = 0; i < NUMBER_OF_APPLES; i++)
    {
        ApplePositions[i] = 0;
    }
    CurrentSnakeLength = 0;
    set_text_colour(FB_BLACK, BG_BLACK);
    snakeClean();
}