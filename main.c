//This is main.c

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <math.h>

#include "helper.h"

int main(void)
{
    enable_escape_char();
    // setvbuf(stdout, NULL, _IONBF, 0);
    atexit(cleanup);
    printf("\033[?25l");


    int map[MAP_HEIGHT][MAP_WIDTH] = 
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
    
   /*
    int map[MAP_HEIGHT][MAP_WIDTH] = 
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    */
    double posX = 3.0, posY = 3.0;
    double dirX = 0.0, dirY = -1.0;
    double planeX = 0.66, planeY = 0.0;

    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]; 
    
    int running = 1;
    system("cls");

    while (running != 0)
{
    if (kbhit())
    {
        u_char c = getch(); // never change this u_char to any other data types it bricks the program idk why but it does so never change it
        if (c == 'Q' || c == 'q')
        {
            running = 0;
        }
        if (c == 'w' || c == 'W')
        {
            double newY = posY + (dirY * speed);
            double newX = posX + (dirX * speed);

            if (can_move(map, newX, posY)) posX = newX;
            if (can_move(map, posX, newY)) posY = newY;
        }
        if (c == 's' || c == 'S')
        {
            double newY = posY - (dirY * speed);
            double newX = posX - (dirX * speed);

            if (can_move(map, newX, posY)) posX = newX;
            if (can_move(map, posX, newY)) posY = newY;
        }
        if (c == 'a' || c == 'A')
        {
            double oldX = dirX;
            dirX = dirX * cos(rotSpeed) + dirY * sin(rotSpeed);
            dirY = -oldX * sin(rotSpeed) + dirY * cos(rotSpeed);

            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) + planeY * sin(rotSpeed);
            planeY = -oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
        if (c == 'd' || c == 'D')
        {

            double oldX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldX * sin(rotSpeed) + dirY * cos(rotSpeed);

            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
            
        }
    }

    render(screen, map, posX, posY, dirX, dirY, planeX, planeY);
    present(screen);

    Sleep(33);
}
    

    return 0;
}
