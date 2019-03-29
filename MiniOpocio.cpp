#include <math.h>
#include <algorithm>
#include <stdio.h>
#include "functions.h"
#include "Mappa.h"
#include "Opocio.h"
#include "MiniOpocio.h"

#define VITA_INIZIALE 1
#define INFANZIA 2000
#define SMELL 0.4

MiniOpocio::MiniOpocio(Mappa *in_mappa, float in_posx, float in_posy)
{
    dim = 0.4;
    speed = 0.2;
    vita = VITA_INIZIALE;
    infanzia = INFANZIA;
    color[0] = 0;
    color[1] = 0;
    color[2] = 155;

    mappa = in_mappa;
    posx = in_posx;
    posy = in_posy;
    calpestato = mappa->get_space(int(posx), int(posy));
    left_steps = 0;
    extra_step = false;
    last_smell = 0.0;
    mappa->set_space(int(posx), int(posy), this);
    generate_meta();
}
Oggetto* MiniOpocio::live()
{
    ///physiological stuff
    if (vita <= 0)
        return this;
    infanzia--;
    if (infanzia <= 0)
    {
        mappa->set_space(int(posx), int(posy), calpestato);
        calpestato = new Opocio(mappa, posx, posy);
        return calpestato;
    }
        mappa->add_smell(posx, posy, SMELL);
        ///act according to the situation
        if (left_steps <= 0)
        {
            if (extra_step)
                generate_meta();
            else
            { ///////////////tune value
                float new_smell = mappa->get_smell(posx, posy);
                if (new_smell > last_smell)
                    left_steps = 8;
                else
                    left_steps = 1;
                //left_steps = rand()%std::max(1,3+int((new_smell-last_smell)/new_smell));
                extra_step = true;
            }
    }
    if(!move())
        generate_meta();
    return nullptr;
}
void MiniOpocio::generate_meta()
{
    float angle = rand()*2*M_PI/RAND_MAX;
    velx = speed*cos(angle);
    vely = speed*sin(angle);
    left_steps = 10;///////////////tune value
    last_smell = mappa->get_smell(posx, posy);
    extra_step = false;
}
bool MiniOpocio::move()
{
    mappa->set_space(int(posx), int(posy), calpestato);
    Oggetto *vicino = mappa->get_space(int(posx+velx), int(posy+vely));
    if (vicino && vicino->get_type() != cibo_type)
    {
        mappa->set_space(int(posx), int(posy), this);
        return false;
    }
    calpestato = vicino;
    posx += velx;
    posy += vely;
    left_steps--;
    mappa->set_space(int(posx), int(posy), this);
    return true;
}
void MiniOpocio::injured(Oggetto *in_enemy, int damage)
{
    vita -= damage;
}
void MiniOpocio::draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy)
{
    int x = int(posx) - player_posx;
    int y = int(posy) - player_posy;
    screen_color[y][x][2] = color[0];
    screen_color[y][x][1] = color[1];
    screen_color[y][x][0] = color[2];
}
void MiniOpocio::draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy)
{
    if(calpestato)
        calpestato->draw(screen_color, zoom_level, player_posx, player_posy);
    int screenx = int((posx - player_posx) * zoom_level);
    int screeny = int((posy - player_posy) * zoom_level);
    int x_min = int_max(0,screenx-int(zoom_level/2+0.55));
    int x_max = int_min(SCREEN_WIDTH-1,screenx+int(zoom_level/2+0.55));
    int y_min = int_max(0,screeny-int(zoom_level/2+0.55));
    int y_max = int_min(SCREEN_HEIGHT-1,screeny+int(zoom_level/2+0.55));
    for (int x = x_min; x <= x_max; x++)
        for (int y = y_min; y <= y_max; y++)
            if ((x-screenx) * (x-screenx) + (y-screeny) * (y-screeny) <= dim * dim * zoom_level * zoom_level)
            {
                screen_color[y][x][2] = color[0];
                screen_color[y][x][1] = color[1];
                screen_color[y][x][0] = color[2];
            }
}
Object_type MiniOpocio::get_type()
{
    return miniopocio_type;
}
MiniOpocio::~MiniOpocio()
{
    if(mappa->get_space(posx, posy) == this)
        mappa->set_space(int(posx), int(posy), calpestato);
}
