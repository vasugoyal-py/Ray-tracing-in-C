#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>

#define MAP_WIDTH 32
#define MAP_HEIGHT 32
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define speed 0.15
#define rotSpeed 0.10

void cleanup(void);
void enable_escape_char(void);
char readoutputlast(void);
void present(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);
void render(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const int map[MAP_HEIGHT][MAP_WIDTH], double posX, double posY, double dirX, double dirY, double planeX, double planeY);
bool can_move(const int map[MAP_HEIGHT][MAP_WIDTH], double newX, double newY);

#endif