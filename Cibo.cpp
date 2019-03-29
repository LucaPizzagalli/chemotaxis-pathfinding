#include "Constant.h"
#include "functions.h"
#include "Mappa.h"
#include "Cibo.h"

Cibo::Cibo(Mappa *in_mappa, int in_posx, int in_posy, int in_cibo)
{
    cibo = in_cibo;
    color[0] = 0;
    color[1] = 100;
    color[2] = 0;
    posx = in_posx;
    posy = in_posy;
    mappa = in_mappa;
    mappa->set_space(posx, posy, this);
}
int Cibo::lose_weight(int morso)
{
    if (cibo <= 0)
    {
        delete this;
        return 0;
    }
    else
    {
        cibo -= morso;
        return morso;
    }
}
Object_type Cibo::get_type()
{
    return cibo_type;
}
void Cibo::draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy)
{
    int x_min = int_max(0,int((posx - player_posx) * zoom_level));
    int x_max = int_min(SCREEN_WIDTH-1,int((posx+1 - player_posx) * zoom_level));
    int y_min = int_max(0,int((posy - player_posy) * zoom_level));
    int y_max = int_min(SCREEN_HEIGHT-1,int((posy+1 - player_posy) * zoom_level));
    for (int x = x_min; x < x_max; x++)
        for (int y = y_min; y < y_max; y++)
        {
            screen_color[y][x][2] = (screen_color[y][x][2]*(CIBO_MAX-cibo)+color[0]*cibo)/CIBO_MAX;
            screen_color[y][x][1] = (screen_color[y][x][1]*(CIBO_MAX-cibo)+color[1]*cibo)/CIBO_MAX;
            screen_color[y][x][0] = (screen_color[y][x][0]*(CIBO_MAX-cibo)+color[2]*cibo)/CIBO_MAX;
        }
}
Cibo::~Cibo()
{
    if(mappa->get_space(posx, posy) == this)
        mappa->set_space(posx, posy, nullptr);
}
