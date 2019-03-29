#include "Constant.h"
#include "functions.h"
#include "Mappa.h"
#include "Pietra.h"

Pietra::Pietra(Mappa *in_mappa, int in_posx, int in_posy, int in_pietra)
{
    pietra = in_pietra;
    color[0] = 100;
    color[1] = 100;
    color[2] = 100;
    posx = in_posx;
    posy = in_posy;
    mappa = in_mappa;
    mappa->set_space(posx, posy, this);
}
int Pietra::lose_weight(int morso)
{
    if (pietra <= 0)
    {
        delete this;
        return 0;
    }
    else
    {
        pietra -= morso;
        return morso;
    }
}
Object_type Pietra::get_type()
{
    return pietra_type;
}
void Pietra::draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy)
{
    int x = int(posx) - player_posx;
    int y = int(posy) - player_posy;
    screen_color[y][x][2] = (screen_color[y][x][2]*(PIETRA_MAX-pietra)+color[0]*pietra)/PIETRA_MAX;
    screen_color[y][x][1] = (screen_color[y][x][1]*(PIETRA_MAX-pietra)+color[1]*pietra)/PIETRA_MAX;
    screen_color[y][x][0] = (screen_color[y][x][0]*(PIETRA_MAX-pietra)+color[2]*pietra)/PIETRA_MAX;
}
void Pietra::draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy)
{
    int x_min = int_max(0,int((posx - player_posx) * zoom_level));
    int x_max = int_min(SCREEN_WIDTH-1,int((posx+1 - player_posx) * zoom_level));
    int y_min = int_max(0,int((posy - player_posy) * zoom_level));
    int y_max = int_min(SCREEN_HEIGHT-1,int((posy+1 - player_posy) * zoom_level));
    for (int x = x_min; x < x_max; x++)
        for (int y = y_min; y < y_max; y++)
        {
            screen_color[y][x][2] = (screen_color[y][x][2]*(PIETRA_MAX-pietra)+color[0]*pietra)/PIETRA_MAX;
            screen_color[y][x][1] = (screen_color[y][x][1]*(PIETRA_MAX-pietra)+color[1]*pietra)/PIETRA_MAX;
            screen_color[y][x][0] = (screen_color[y][x][0]*(PIETRA_MAX-pietra)+color[2]*pietra)/PIETRA_MAX;
        }
}
Pietra::~Pietra()
{
    if(mappa->get_space(posx, posy) == this)
        mappa->set_space(posx, posy, nullptr);
}
