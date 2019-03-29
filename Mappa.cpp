#include <stdio.h>
#include <math.h>
#include "functions.h"
#include "Mappa.h"

#define VISCOSITA 55
#define STABILITA 0.99997

Mappa::Mappa()
{
    occupazione = new Oggetto*[MAP_WIDTH*MAP_HEIGHT];
	smell2 = new float[MAP_WIDTH*MAP_HEIGHT];
    smell1 = new float[MAP_WIDTH*MAP_HEIGHT];
    for (int index = 0; index < MAP_WIDTH*MAP_HEIGHT; index++)
    {
        occupazione[index] = nullptr;
        smell1[index] = 0;
        smell2[index] = 0;
    }
    stable_smell = smell1;
    unstable_smell = smell2;
}
void Mappa::add_smell(int x, int y, float in_smell)
{
    if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        printf("ERROR: sei fuori dalla mappa 2! %d %d\n", x, y);
    else
        new_smell.push_back(A_smell{x+MAP_WIDTH*y, in_smell});
}
float Mappa::get_smell(int x, int y)
{
    if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return 0;
    return stable_smell[x+MAP_WIDTH*y];
}
void Mappa::set_space(int x, int y, Oggetto *oggetto)
{
    if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        printf("ERROR: sei fuori dalla mappa! %d %d\n", x, y);
    else
        occupazione[x+MAP_WIDTH*y] = oggetto;
}
Oggetto* Mappa::get_space(int x, int y)
{
    if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return nullptr;
    return occupazione[x+MAP_WIDTH*y];
}
void Mappa::draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy)
{
    if(draw_type == 0)
    {
    }
    else
    {
    int x_min = int_max(0,-player_posx);
    int x_max = int_min(SCREEN_WIDTH,MAP_WIDTH-player_posx);
    int y_min = int_max(0,-player_posy);
    int y_max = int_min(SCREEN_HEIGHT,MAP_HEIGHT-player_posy);
    for (int screenx = x_min; screenx < x_max; screenx++)
        for (int screeny = y_min; screeny < y_max; screeny++)
        {
            int x = screenx + player_posx;
            int y = screeny + player_posy;
            if (stable_smell[x+MAP_WIDTH*y] < 0)
            {
                int log_value = int(std::log(1-stable_smell[x+MAP_WIDTH*y])*15);
                screen_color[screeny][screenx][2] = int_min(255, log_value);
                screen_color[screeny][screenx][1] = int_min(255,int_max(0,log_value-255));
                screen_color[screeny][screenx][0] = int_min(255,int_max(0,log_value-510));
            }
            else
            {
                int log_value = int(std::log(1+stable_smell[x+MAP_WIDTH*y])*15);
                screen_color[screeny][screenx][2] = int_min(255,int_max(0,log_value-510));
                screen_color[screeny][screenx][1] = int_min(255,int_max(0,log_value-255));
                screen_color[screeny][screenx][0] = int_min(255, log_value);
            }
        }
    }
}
void Mappa::draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy)
{
    if(draw_type == 0)
    {
    }
    else
    {
        int x_min = int_max(0,int(-player_posx*zoom_level));
        int x_max = int_min(SCREEN_WIDTH,int((MAP_WIDTH-player_posx)*zoom_level));
        int y_min = int_max(0,int(-player_posy*zoom_level));
        int y_max = int_min(SCREEN_HEIGHT,int((MAP_WIDTH-player_posy)*zoom_level));
        for (int screenx = x_min; screenx < x_max; screenx++)
            for (int screeny = y_min; screeny < y_max; screeny++)
            {
                int x = int(screenx / zoom_level + player_posx);
                int y = int(screeny / zoom_level + player_posy);
                if (stable_smell[x+MAP_WIDTH*y] < 0)
                {
                    int log_value = int(std::log(1-stable_smell[x+MAP_WIDTH*y])*40);
                    screen_color[screeny][screenx][2] = int_min(255, log_value);
                    screen_color[screeny][screenx][1] = int_min(255,int_max(0,log_value-255));
                    screen_color[screeny][screenx][0] = int_min(255,int_max(0,log_value-510));
                }
                else
                {
                    int log_value = int(std::log(1+stable_smell[x+MAP_WIDTH*y])*40);
                    screen_color[screeny][screenx][2] = int_min(255,int_max(0,log_value-510));
                    screen_color[screeny][screenx][1] = int_min(255,int_max(0,log_value-255));
                    screen_color[screeny][screenx][0] = int_min(255, log_value);
                }
            }
    }
}
void Mappa::compute_diffusion()
{
    for (int x = 1; x < MAP_WIDTH-1; x++)
        for (int y = 1; y < MAP_HEIGHT-1; y++)
            unstable_smell[x+MAP_WIDTH*y] = stable_smell[x+MAP_WIDTH*y]+(stable_smell[x-1+MAP_WIDTH*y]+stable_smell[x+MAP_WIDTH*(y-1)]+stable_smell[x+1+MAP_WIDTH*y]+stable_smell[x+MAP_WIDTH*(y+1)]-4*stable_smell[x+MAP_WIDTH*y])/VISCOSITA;
    for (int x = 1; x < MAP_WIDTH-1; x++)
        for (int y = 1; y < MAP_HEIGHT-1; y++)
            unstable_smell[x+MAP_WIDTH*y] = unstable_smell[x+MAP_WIDTH*y]*STABILITA;
}
void Mappa::confirm_diffusion()
{
    float *temp = unstable_smell;
    for (A_smell a_smell : new_smell)
        unstable_smell[a_smell.index] += a_smell.intensity;
    new_smell.clear();
    unstable_smell = stable_smell;
    stable_smell = temp;
}
void Mappa::print()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
            if(occupazione[x+MAP_WIDTH*y])
                printf("%d ", occupazione[x+MAP_WIDTH*y]->get_type());
            else
                printf("0 ");
        printf("\n");
    }
}
