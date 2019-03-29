#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "functions.h"
#include "Mappa.h"
#include "Opocio.h"
#include "MiniOpocio.h"

#define ENERGIA_INIZIALE 5000000//500
#define VITA_INIZIALE 30
#define CARNE_INIZIALE 4000
#define ATTACCO 1
#define MORSO ENERGIA_INIZIALE / 20
#define SMELL 1
#define GRAVIDANZA 500


Opocio::Opocio(Mappa *in_mappa, float in_posx, float in_posy)
{
    dim = 0.5;
    speed = 0.5;
    energia = ENERGIA_INIZIALE;
    vita = VITA_INIZIALE;
    carne = CARNE_INIZIALE;
    gravidanza = GRAVIDANZA;
    cantiere_proximity = -1;
    pietra_proximity = -1;
    color[0] = 0;
    color[1] = 0;
    color[2] = 255;
    enemy = nullptr;
    esitazioni = 0;
    priority[eat_food] = none;
    priority[eat_opocio] = none;
    if(rand()%2==0)//////
    {
        priority[find_cantiere] = none;
        priority[find_pietra] = pietra_type;
        priority[ask_pietra] = alive_opocio;
        priority[ask_cantiere] = none;
    }
    else
    {
        priority[find_cantiere] = cantiere_type;
        priority[find_pietra] = none;
        priority[ask_pietra] = none;
        priority[ask_cantiere] = alive_opocio;
    }
    mappa = in_mappa;
    posx = in_posx;
    posy = in_posy;
    carried = none;
    calpestato = mappa->get_space(int(posx), int(posy));
    left_steps = 0;
    last_smell = 0.0;
    mappa->set_space(int(posx), int(posy), this);
    vel_angle = rand()*2*M_PI/RAND_MAX - M_PI;
    generate_meta(1);
}
Oggetto* Opocio::live()
{
    ///physiological stuff
    if (carne <=0)
        return this;
    if(energia <= 0)
    {
        vita = 0;
        color[0] = 100;
        color[1] = 100;
        color[2] = 100;
    }
    if (vita <= 0)
    {
        carne--;
        return nullptr;
    }
    cantiere_proximity *= 0.99;
    pietra_proximity *= 0.99;
    energia--;
    mappa->add_smell(posx, posy, SMELL);
    if(calpestato && calpestato->get_type() == cibo_type)
    {
        if(calpestato->lose_weight(1) > 0)
            gravidanza--;
        else
            calpestato = nullptr;
        if (gravidanza <= 0)
            for(int x = int(posx) - 1; x <= int(posx) + 1; x++)
                for(int y=int(posy)-1; y<= int(posy)+1; y++)
                {
                    Oggetto *oggetto = mappa->get_space(x, y);
                    if (!oggetto || oggetto->get_type() == cibo_type)
                    {
                        gravidanza = GRAVIDANZA;
                        return new MiniOpocio(mappa, x + 0.5, y + 0.5);
                    }
                }
    }
    ///decide priority list
    if (energia < ENERGIA_INIZIALE-MORSO*2)
        priority[eat_food] = dead_opocio;
    if (energia < ENERGIA_INIZIALE/2)
        priority[eat_opocio] = alive_opocio;
    ///percive surroundings
    Oggetto *vicino = nullptr;
    int tipo_azione = what_nearby(&vicino);
    bool tired = false;
    ///act according to the situation
    if (tipo_azione == attack_opocio || tipo_azione == eat_opocio)
        {
            vicino->injured(this, ATTACCO);
            tired = true;
        }
    else if (tipo_azione == eat_food)
        {
            bite(vicino);
            tired = true;
        }
    else if (tipo_azione == find_pietra)
    {
        priority[find_cantiere] = cantiere_type;
        priority[find_pietra] = none;
        priority[ask_cantiere] = alive_opocio;
        priority[ask_pietra] = none;
        pietra_proximity = 1;
        mappa->add_smell(posx, posy, SMELL*100);/////tune value
        tired = true;
    }
    else if (tipo_azione == find_cantiere)
    {
        carried = pietra_type;
        priority[find_cantiere] = none;
        priority[find_pietra] = pietra_type;
        priority[ask_cantiere] = none;
        priority[ask_pietra] = alive_opocio;
        cantiere_proximity = 1;
        mappa->add_smell(posx, posy, SMELL*100);/////tune value
        tired = true;
    }
    else if (tipo_azione == ask_pietra || tipo_azione == ask_cantiere)
    {
        Opocio *opo_vicino = (Opocio*)vicino;
        if(opo_vicino->is_searching_cantiere()||opo_vicino->is_searching_pietra())
        {
            float other_cantiere = opo_vicino->get_cantiere_prox();
            float other_pietra = opo_vicino->get_pietra_prox();
            opo_vicino->update_information(cantiere_proximity, pietra_proximity);
            update_information(other_cantiere, other_pietra);
            Oggetto* esito = move();
            if(esito == opo_vicino)
                {
                    esitazioni++;
                    if(esitazioni > 1)
                        generate_meta(0);
                }
            else if(esito != nullptr)
                {
                    generate_meta(0);
                    //if(esito->get_type() != alive_opocio)//77forse
                    mappa->add_smell(posx, posy, -SMELL*30);
                }
            tired = true;
        }
    }
    if(!tired)
    {
        if(left_steps <= 0)
        {
            if (mappa->get_smell(posx, posy) > last_smell)
                generate_meta(1);
            else
                generate_meta(0);
        }
        if(move() != nullptr)
            generate_meta(0);
    }
    return nullptr;
}
void Opocio::generate_meta(int coherence)
{
    float angle = rand()*M_PI/2/RAND_MAX;/////decide a probability distribution
    if(rand()%2 == 0)
        angle *= -1;
    if(coherence > 0)
        angle /= 4;
    else if(coherence < 0)
        angle *= 2;
    vel_angle = fmod(vel_angle + angle, 2*M_PI);
    velx = speed*cos(vel_angle);
    vely = speed*sin(vel_angle);
    left_steps = 8;///////////////tune value
    last_smell = mappa->get_smell(posx, posy);
}
Oggetto* Opocio::move()
{
    mappa->set_space(int(posx), int(posy), calpestato);
    Oggetto *vicino = mappa->get_space(int(posx+velx), int(posy+vely));
    if (vicino && vicino->get_type() != cibo_type && vicino->get_type() != pietra_type)
    {
        mappa->set_space(int(posx), int(posy), this);
        return vicino;
    }
    calpestato = vicino;
    posx += velx;
    posy += vely;
    left_steps--;
    mappa->set_space(int(posx), int(posy), this);
    esitazioni = 0;
    return nullptr;
}
int Opocio::what_nearby(Oggetto **vicino)
{
    *vicino = nullptr;
    int obiettivo = N_priority;
    for (int x = int(posx) - 1; x <= int(posx) + 1; x++)
        for (int y = int(posy) - 1; y<= int(posy) + 1; y++)
        {
            Oggetto *ogg = mappa->get_space(x, y);
            if (ogg && ogg != this)
            {
                if (ogg == enemy && ogg->get_type() != dead_opocio)
                {
                    *vicino = ogg;
                    return attack_opocio;
                }
                for (int prio = 0; prio < obiettivo; prio ++)
                    if(priority[prio] != none && ogg->get_type() == priority[prio])///////
                    {
                        *vicino = ogg;
                        obiettivo = prio;
                    }
            }
        }
    enemy = nullptr;
    return obiettivo;
}
void Opocio::injured(Oggetto *in_enemy, int damage)
{
    enemy = in_enemy;
    vita -= damage;
    if(vita <= 0)
    {
        vita = 0;
        color[0] = 255;
        color[1] = 100;
        color[2] = 100;
    }
}
void Opocio::bite(Oggetto *prey)
{
    int morso = prey->lose_weight(MORSO);
    energia += morso;
    if (energia >= ENERGIA_INIZIALE / 2)
        priority[eat_opocio] = none;
    if (energia >= ENERGIA_INIZIALE-MORSO*2)
        priority[eat_food] = none;
}
void Opocio::update_information(float other_cantiere, float other_pietra)
{
    float mia_prox;
    float other_prox;
    if(priority[find_cantiere] != none)
    {
        mia_prox = cantiere_proximity;
        other_prox = other_cantiere;
    }
    else if(priority[find_pietra] != none)
    {
        mia_prox = pietra_proximity;
        other_prox = other_pietra;
    }
    if(other_prox < mia_prox*0.98)
    {
        generate_meta(-1);
        if(prova == 1)
            mappa->add_smell(posx, posy, -SMELL*1500);
    }
    else if(other_prox > mia_prox*1.02)
    {
        left_steps += 22;///////////////tune value
        //if(prova == 1)
        //    mappa->add_smell(posx, posy, SMELL*1500);///possibile idea ma sembra non funziare
    }
    cantiere_proximity = std::max(cantiere_proximity, other_cantiere);
    pietra_proximity = std::max(pietra_proximity, other_pietra);
}
int Opocio::lose_weight(int morso)
{
    carne -= morso;
    if (carne < 0)////qua si potrebbe anche togliere l'if perdendo un pochino di precisione
    {
        morso += carne;
        carne = 0;
    }
    return morso;
}
void Opocio::draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy)
{
    int x = int(posx) - player_posx;
    int y = int(posy) - player_posy;
    if(color_type == 0)
    {
        if(carried == pietra_type)
        {   
            screen_color[y][x][2] = 200;
            screen_color[y][x][1] = 200;
            screen_color[y][x][0] = 200;
        }
        else
        {
            screen_color[y][x][2] = color[0];
            screen_color[y][x][1] = color[1];
            screen_color[y][x][0] = color[2];
        }
    }
    else if(color_type == 1)
    {
        int log_value = int(-std::log(pietra_proximity)*30);
        screen_color[y][x][2] = int_min(255, log_value);
        screen_color[y][x][1] = int_min(255,int_max(0,log_value-255));
        screen_color[y][x][0] = int_min(255,int_max(0,log_value-510));    
    }
    else
    {
        int log_value = int(-std::log(cantiere_proximity)*30);
        screen_color[y][x][2] = int_min(255, log_value);
        screen_color[y][x][1] = int_min(255,int_max(0,log_value-255));
        screen_color[y][x][0] = int_min(255,int_max(0,log_value-510));    
    }
}
void Opocio::draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy)
{
    if(calpestato)
        calpestato->draw(screen_color, zoom_level, player_posx, player_posy);
    int screenx = int((posx - player_posx) * zoom_level);
    int screeny = int((posy - player_posy) * zoom_level);
    int x_min = int_max(0,screenx-int(zoom_level/2+0.55));
    int x_max = int_min(SCREEN_WIDTH-1,screenx+int(zoom_level/2+0.55));
    int y_min = int_max(0,screeny-int(zoom_level/2+0.55));
    int y_max = int_min(SCREEN_HEIGHT-1,screeny+int(zoom_level/2+0.55));
    if(carried == pietra_type)
    {
        for (int x = x_min; x <= x_max; x++)
            for (int y = y_min; y <= y_max; y++)
                if ((x-screenx) * (x-screenx) + (y-screeny) * (y-screeny) <= dim * dim * zoom_level * zoom_level)
                {
                    screen_color[y][x][2] = 200;
                    screen_color[y][x][1] = 200;
                    screen_color[y][x][0] = 200;
                }
    }
    else
        for (int x = x_min; x <= x_max; x++)
            for (int y = y_min; y <= y_max; y++)
                if ((x-screenx) * (x-screenx) + (y-screeny) * (y-screeny) <= dim * dim * zoom_level * zoom_level)
                {
                    screen_color[y][x][2] = color[0];
                    screen_color[y][x][1] = color[1];
                    screen_color[y][x][0] = color[2];
                }
}
bool Opocio::is_searching_cantiere()
{
    return priority[find_cantiere] != none;
}
 bool Opocio::is_searching_pietra()
 {
     return priority[find_pietra] != none;
 }
float Opocio::get_cantiere_prox()
{
    return cantiere_proximity;
}
float Opocio::get_pietra_prox()
{
    return pietra_proximity;
}
void Opocio::set_cantiere_prox(float in_cantiere_proximity)
{
    cantiere_proximity = in_cantiere_proximity;
}
void Opocio::set_pietra_prox(float in_pietra_proximity)
{
    pietra_proximity = in_pietra_proximity;
}
Object_type Opocio::get_type()
{
    if(vita > 0)////if non necessario
        return alive_opocio;
    return dead_opocio;
}
void Opocio::print()
{
    printf("%d %d\n", int(posx), int(posy));
}
Opocio::~Opocio()
{
    if(mappa->get_space(posx, posy) == this)
        mappa->set_space(int(posx), int(posy), calpestato);
}
