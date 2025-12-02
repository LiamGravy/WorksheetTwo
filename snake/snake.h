#ifndef SNAKE_H
#define SNAKE_H

void start_snake();
int randomlyPlaceFood();
void move_snake(unsigned int direction);
void increase_snake_length();
void handle_input();
void drawSnake();
void update_game();
void wait();

#endif