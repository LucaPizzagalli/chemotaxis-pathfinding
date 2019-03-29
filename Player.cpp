#include "Player.h"
#include "Constant.h"

#define ENERGIA_INIZIALE 500
#define VITA_INIZIALE 30
#define CARNE_INIZIALE 2000
#define ATTACCO 1
#define MORSO ENERGIA_INIZIALE / 20
#define SMELL 1

Player::Player(float *in_p_posx, float *in_p_posy, float *in_p_zoom_level)
{
    p_posx = in_p_posx;
    p_posy = in_p_posy;
    p_zoom_level = in_p_zoom_level;
    velx = 0.0;
    vely = 0.0;
    velz = 1.0;
}
void Player::move()
{
    *p_posx += velx;
    *p_posy += vely;
    *p_zoom_level *= velz;
    *p_posx += (1-1/velz)/2*SCREEN_WIDTH;
    *p_posy += (1-1/velz)/2*SCREEN_HEIGHT;
    velx *= 0.9;
    vely *= 0.9;
    velz = 1 + (velz-1)*0.8;
}
void Player::add_velx(float in_velx)
{
    velx += in_velx;
}
void Player::add_vely(float in_vely)
{
    vely += in_vely;
}
void Player::add_velz(float in_velz)
{
    velz *= in_velz;
}
float Player::get_zoom_level()
{
    return *p_zoom_level;
}
float Player::get_posx()
{
    return *p_posx;
}
float Player::get_posy()
{
    return *p_posy;
}
