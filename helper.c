/*
This is the code of helper.h

#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define speed 0.15
#define rotSpeed 0.05

void cleanup(void);
void enable_escape_char(void);
char readoutputlast(void);
void present(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);
void render(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], int map[MAP_HEIGHT][MAP_WIDTH], double posX, double posY, double dirX, double dirY, double planeX, double planeY);
bool can_move(int map[MAP_HEIGHT][MAP_WIDTH], double newX, double newY);

#endif
*/

//this is helper.c
#include "helper.h"

#include <stdio.h>
#include <windows.h>
#include <math.h>

void cleanup(void)
{
    printf("\033[?25h");
}

void enable_escape_char(void)
{
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(hOut, &mode);

    SetConsoleMode(
        hOut,
        mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
    );
}

char readoutputlast(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hOut, &info);

    COORD cursor = info.dwCursorPosition;

    // nothing typed yet on this line
    if (cursor.X == 0)
        return '\0';

    // read the single character immediately before the cursor
    COORD pos = { cursor.X - 1, cursor.Y };

    char ch;
    DWORD charsRead;
    ReadConsoleOutputCharacter(hOut, &ch, 1, pos, &charsRead);

    return ch;
}

void present(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1])
{
    char frame[SCREEN_HEIGHT * (SCREEN_WIDTH + 1) + 16];
    int pos = 0;

    pos += sprintf(frame + pos, "\033[H");

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        pos += sprintf(frame + pos, "%s", screen[y]);
        if (y < SCREEN_HEIGHT - 1)
        {
            frame[pos++] = '\n';
        }
    }
    frame[pos] = '\0';

    fputs(frame, stdout);
}

bool can_move(const int map[MAP_HEIGHT][MAP_WIDTH], double newX, double newY)
{
    int mapX = (int)newX;
    int mapY = (int)newY;

    if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
    {
        return false;
    }

    return map[mapY][mapX] == 0;
}



/*
void render(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], int map[MAP_HEIGHT][MAP_WIDTH], double posX, double posY, double dirX, double dirY, double planeX, double planeY)
{
    (void)planeX;
    (void)planeY;

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            screen[y][x] = ' ';
        }
        screen[y][SCREEN_WIDTH] = '\0'; 
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            screen[y][x] = map[y][x] ? '#' : '.';
        }
    }

    int playerScreenX = (int)posX;
    int playerScreenY = (int)posY;
    screen[playerScreenY][playerScreenX] = 'P';

    int arrowX = playerScreenX;
    int arrowY = playerScreenY;
    char arrow = '^';

    if (fabs(dirX) > fabs(dirY))
    {
        arrowX += dirX > 0 ? 1 : -1;
        arrow = dirX > 0 ? '>' : '<';
    }
    else
    {
        arrowY += dirY > 0 ? 1 : -1;
        arrow = dirY > 0 ? 'v' : '^';
    }

    if (arrowX >= 0 && arrowX < SCREEN_WIDTH && arrowY >= 0 && arrowY < SCREEN_HEIGHT)
    {
        screen[arrowY][arrowX] = arrow;
    }
}
*/

void render(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1],
            const int map[MAP_HEIGHT][MAP_WIDTH],
            double posX, double posY,
            double dirX, double dirY,
            double planeX, double planeY)
{
    // Step A: Set null terminators for every row
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        screen[y][SCREEN_WIDTH] = '\0';
    }

    // Step B: Cast a ray for each vertical column
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // 1. Calculate ray position and direction
        double cameraX = 2.0 * x / (double)SCREEN_WIDTH - 1.0; // -1 (left) .. 1 (right)
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        // 2. Set up DDA step parameters
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1.0 / rayDirY);

        int stepX, stepY;
        double sideDistX, sideDistY;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        // 3. Perform DDA grid traversal
        int hit = 0;
        int side = 0; 

        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                hit = 1; 
                break;
            }

            if (map[mapY][mapX] > 0) {
                hit = 1;
            }
        }

        // 4. Calculate perpendicular distance (avoids fisheye distortion)
        double perpWallDist;
        if (side == 0) {
            perpWallDist = (sideDistX - deltaDistX);
        } else {
            perpWallDist = (sideDistY - deltaDistY);
        }
        if (perpWallDist < 0.0001) perpWallDist = 0.0001;

        // 5. Calculate wall column height and draw boundaries
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;

        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // 6. Select shading character (closer = denser glyph, Y-side = slightly dimmer)
        char wallChar;
        if (perpWallDist < 2.0)      wallChar = '#';
        else if (perpWallDist < 4.0) wallChar = '%';
        else if (perpWallDist < 8.0) wallChar = '+';
        else                          wallChar = '.';

        if (side == 1 && wallChar == '#') wallChar = '%'; 

        // 7. Write ceiling, wall, and floor into screen[y][x]
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            if (y < drawStart) {
                screen[y][x] = ' ';           
            } else if (y <= drawEnd) {
                screen[y][x] = wallChar;      
            } else {
                screen[y][x] = '.';          
            }
        }
    }
}