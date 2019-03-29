#include "functions.h"
#include "Mappa.h"
#include "Sasso.h"

Sasso::Sasso(Mappa *in_mappa, float in_posx, float in_posy)
{
    color[0] = 100;
    color[1] = 100;
    color[2] = 100;
    posx = in_posx;
    posy = in_posy;
    mappa = in_mappa;
    mappa->set_space(int(posx), int(posy), this);
}
Object_type Sasso::get_type()
{
    return sasso_type;
}
void Sasso::draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy)
{
    int x = int(posx) - player_posx;
    int y = int(posy) - player_posy;
    screen_color[y][x][2] = color[0];
    screen_color[y][x][1] = color[1];
    screen_color[y][x][0] = color[2];
}
void Sasso::draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy)
{
    int screenx = int((posx - player_posx + 0.5) * zoom_level);
    int screeny = int((posy - player_posy + 0.5) * zoom_level);
    int x_min = int_max(0,screenx-int(zoom_level/2+0.5));
    int x_max = int_min(SCREEN_WIDTH-1,screenx+int(zoom_level/2+0.5));
    int y_min = int_max(0,screeny-int(zoom_level/2+0.5));
    int y_max = int_min(SCREEN_HEIGHT-1,screeny+int(zoom_level/2+0.5));
    for (int x = x_min; x <= x_max; x++)
        for (int y = y_min; y <= y_max; y++)
        {
            screen_color[y][x][2] = color[0];
            screen_color[y][x][1] = color[1];
            screen_color[y][x][0] = color[2];
        }
}
Sasso::~Sasso()
{
    mappa->set_space(int(posx), int(posy), nullptr);
}
