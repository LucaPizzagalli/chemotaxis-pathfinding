#include "Constant.h"
#include "functions.h"
#include "Mappa.h"
#include "Cantiere.h"

Cantiere::Cantiere(Mappa *in_mappa, int in_posx, int in_posy)
{
    color[0] = 100;
    color[1] = 100;
    color[2] = 100;
    posx = in_posx;
    posy = in_posy;
    mappa = in_mappa;
    mappa->set_space(posx, posy, this);
}
Object_type Cantiere::get_type()
{
    return cantiere_type;
}
void Cantiere::draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy)
{
    int x = int(posx) - player_posx;
    int y = int(posy) - player_posy;
    screen_color[y][x][2] = color[0];
    screen_color[y][x][1] = color[1];
    screen_color[y][x][0] = color[2];
}
void Cantiere::draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy)
{
    int x_min = int_max(0,int((posx - player_posx) * zoom_level));
    int x_max = int_min(SCREEN_WIDTH-1,int((posx+1 - player_posx) * zoom_level));
    int y_min = int_max(0,int((posy - player_posy) * zoom_level));
    int y_max = int_min(SCREEN_HEIGHT-1,int((posy+1 - player_posy) * zoom_level));
    for (int x = x_min; x < x_max; x++)
        for (int y = y_min; y < y_max; y++)
        {
            screen_color[y][x][2] = color[0];
            screen_color[y][x][1] = color[1];
            screen_color[y][x][0] = color[2];
        }
}
Cantiere::~Cantiere()
{
    if(mappa->get_space(posx, posy) == this)
        mappa->set_space(posx, posy, nullptr);
}
