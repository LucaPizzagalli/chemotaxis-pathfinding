#include <SDL2/SDL.h>
#include <time.h>
#include <list>
#include <algorithm>
#include <cmath>
#include <thread>

#include "Constant.h"
#include "Player.h"
#include "Mappa.h"
#include "Sasso.h"
#include "Cibo.h"
#include "Pietra.h"
#include "Cantiere.h"
#include "Opocio.h"
#include "MiniOpocio.h"

int prova;

bool init(SDL_Window **gWindow, SDL_Renderer **gRenderer, SDL_Texture **texture,
	      unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4])
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	atexit(SDL_Quit);
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
		return false;
	}
	*gWindow = SDL_CreateWindow("SDL stuff", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	*gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (*gRenderer == nullptr)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	*texture = SDL_CreateTexture(*gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
	SCREEN_WIDTH, SCREEN_HEIGHT);
	if (*texture == nullptr)
	{
		printf("texture could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	for (int x = 0; x < SCREEN_WIDTH; x++)
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			screen_color[y][x][0] = 0;
			screen_color[y][x][1] = 0;
			screen_color[y][x][2] = 0;
			screen_color[y][x][3] = 255;
		}
	return true;
}
void close(SDL_Window **gWindow, SDL_Renderer **gRenderer, Player *player, Mappa *mappa,
		   std::list<Opocio*> opo_list, std::list<Sasso*> sasso_list, std::list<Cibo*> cibo_list,
		   std::list<Pietra*> pietra_list)
{
	std::list<Opocio*>::iterator opo_it = opo_list.begin();
	while(opo_it != opo_list.end())
	{
		delete *opo_it;
		opo_it = opo_list.erase(opo_it);
	}
	std::list<Sasso*>::iterator sasso_it = sasso_list.begin();
	while(sasso_it != sasso_list.end())
	{
		delete *sasso_it;
		sasso_it = sasso_list.erase(sasso_it);
	}
	std::list<Cibo*>::iterator cibo_it = cibo_list.begin();
	while(cibo_it != cibo_list.end())
	{
		delete *cibo_it;
		cibo_it = cibo_list.erase(cibo_it);
	}//////cibo e pietra da fare
	delete mappa;
	delete player;
	SDL_DestroyRenderer(*gRenderer);
	SDL_DestroyWindow(*gWindow);
	*gWindow = nullptr;
	*gRenderer = nullptr;
	//IMG_Quit();
	SDL_Quit();
}
void carica_mappa(Mappa **mappa, std::list<Sasso*> *sasso_list, std::list<Cibo*> *cibo_list,
	 std::list<Pietra*> *pietra_list, std::list<Opocio*> *opo_list, std::list<MiniOpocio*> *miniopo_list,
	 std::list<Cantiere*> *cantiere_list)
{
	*mappa = new Mappa();
	for (int i = 1; i < MAP_WIDTH-1; i++)
	{
		sasso_list->push_front(new Sasso(*mappa, i, 1));
		sasso_list->push_front(new Sasso(*mappa, i, MAP_HEIGHT - 2));
	}
	for (int i = 2; i < MAP_HEIGHT-2;i++)
	{
		sasso_list->push_front(new Sasso(*mappa, 1, i));
		sasso_list->push_front(new Sasso(*mappa, MAP_WIDTH - 2, i));
	}
	for (int i = 160; i < MAP_HEIGHT-2; i++)
		sasso_list->push_front(new Sasso(*mappa, 400, i));
	for (int i = 40; i < 200; i+=3)
		cantiere_list->push_front(new Cantiere(*mappa, 60, i));
	/*for (int x = 3; x < 400; x++)
		for (int y = 3; y < 400;y++)
		{
			float distance = std::pow((x-100)*(x-100)+(y-100)*(y-100),0.9);
			float distance2 = std::pow((x-100)*(x-100)+(y-100)*(y-100),0.9);
			if(rand()%int(1+ distance/150)==0)
				cibo_list->push_front(new Cibo(*mappa, x, y,
					std::min(CIBO_MAX-120, int(30/(distance2+10)*CIBO_MAX))+rand()%120));
		}
	for (int x = 300; x < 500; x++)
		for (int y = 3; y < 300;y++)
		{
			float distance = std::pow((x-400)*(x-400)+(y-100)*(y-100),0.9);
			float distance2 = std::pow((x-400)*(x-400)+(y-100)*(y-100),0.9);
			if(rand()%int(1+ distance/100)==0)
				pietra_list->push_front(new Pietra(*mappa, x, y,
					std::min(PIETRA_MAX-120, int(30/(distance2+10)*PIETRA_MAX))+rand()%120));
		}*/
	for (int x = 470; x < 500; x++)
		for (int y = 320; y < 350;y++)
			pietra_list->push_front(new Pietra(*mappa, x, y, PIETRA_MAX));
	// const int N_OPOCII = 1000;
	// for(int i=0; i <= N_OPOCII; i++)
	// 	opo_list->push_front(new Opocio(*mappa, 50.0+i%40*11.0, 100.0+i/40*10.0));
	for (int x = 3; x < MAP_WIDTH-4; x++)
		for (int y = 3; y < MAP_HEIGHT-4; y++)
			if(x%11==0&&y%11==0)
				opo_list->push_front(new Opocio(*mappa, x, y));
}

void user_live(Player *player, bool *is_in_game, Mappa *mappa,
     int *SCREEN_TICK_PER_FRAME)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if(e.type == SDL_QUIT)
			*is_in_game = false;
		else if(e.type == SDL_MOUSEBUTTONUP )
		{
			int mousex, mousey;
			SDL_GetMouseState(&mousex, &mousey);
			mousex = int(mousex/player->get_zoom_level() + player->get_posx());
			mousey = int(mousey/player->get_zoom_level() + player->get_posy());
			if (mousex >= 0 && mousex < MAP_WIDTH && mousey >= 0 && mousey < MAP_HEIGHT)
			{
				if(e.button.button == SDL_BUTTON_LEFT)
					mappa->add_smell(mousex, mousey, 1.0e7);
				else if(e.button.button == SDL_BUTTON_RIGHT)
					mappa->add_smell(mousex, mousey, -1.0e7);
			}
		}
		else if(e.type == SDL_KEYDOWN)
		{
			if(e.key.keysym.sym == SDLK_ESCAPE)
				*is_in_game = false;
			else if(e.key.keysym.sym == SDLK_m)
				Mappa::draw_type = (Mappa::draw_type+1)%2;
			else if(e.key.keysym.sym == SDLK_n)
				Opocio::color_type = (Opocio::color_type+1)%3;
			else if(e.key.keysym.sym == SDLK_p)
				prova = (prova+1)%2;
		}
	}
	const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
	if(currentKeyStates[SDL_SCANCODE_A])
		player->add_velx(-4.0/player->get_zoom_level());
	if(currentKeyStates[SDL_SCANCODE_S])
		player->add_vely(4.0/player->get_zoom_level());
	if(currentKeyStates[SDL_SCANCODE_W])
		player->add_vely(-4.0/player->get_zoom_level());
	if(currentKeyStates[SDL_SCANCODE_D])
		player->add_velx(4.0/player->get_zoom_level());
	if(currentKeyStates[SDL_SCANCODE_KP_PLUS])
		player->add_velz(1.01);
	if(currentKeyStates[SDL_SCANCODE_KP_MINUS])
		player->add_velz(0.99);
	if(currentKeyStates[SDL_SCANCODE_SPACE])
		*SCREEN_TICK_PER_FRAME = 1;
	else
		*SCREEN_TICK_PER_FRAME = 1;//////
	player->move();
}

int Opocio::color_type = 0;
int Mappa::draw_type = 0;
int main(int argc, char* args[])
{
	SDL_Window *gWindow = nullptr;
	SDL_Renderer *gRenderer = nullptr;
	SDL_Texture *texture = nullptr;
	unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4];
	int SCREEN_TICK_PER_FRAME = 60;
	bool is_counting_time = false;
	int cicle_time = 0, partial_time = 0;
	prova = 0;
	if (!init(&gWindow, &gRenderer, &texture, screen_color))
		return 1;
	srand(42);//srand(time(nullptr));
	float player_posx = 0.0, player_posy = 0.0, zoom_level = 1.0;
	Player *player = new Player(&player_posx, &player_posy, &zoom_level);
	Mappa *mappa = nullptr;
	std::list<Sasso*> sasso_list;
	std::list<Cibo*> cibo_list;
	std::list<Pietra*> pietra_list;
	std::list<Cantiere*> cantiere_list;
	std::list<Opocio*> opo_list;
	std::list<MiniOpocio*> miniopo_list;
	carica_mappa(&mappa, &sasso_list, &cibo_list, &pietra_list, &opo_list, &miniopo_list, &cantiere_list);
	bool is_in_game = true;
	std::thread diffusion_thread(&Mappa::compute_diffusion, mappa);
	while (is_in_game)
	{
		cicle_time = SDL_GetTicks();
		partial_time = cicle_time;
		user_live(player, &is_in_game, mappa, &SCREEN_TICK_PER_FRAME);
		diffusion_thread.join();
		mappa->confirm_diffusion();
		diffusion_thread = std::thread(&Mappa::compute_diffusion, mappa);
		if(is_counting_time)
			printf("diff: %d ", SDL_GetTicks() - partial_time);
		partial_time = SDL_GetTicks();

		std::list<Opocio*>::iterator opo_it = opo_list.begin();
		while (opo_it != opo_list.end())
		{
			Oggetto* esito=(*opo_it)->live();
			if (esito)
			{
				if (esito == *opo_it)
				{
					delete *opo_it;
					opo_it = opo_list.erase(opo_it);
				}
				else if(esito->get_type() == miniopocio_type)
				{
					miniopo_list.push_front((MiniOpocio *)esito);
					opo_it++;
				}
			}
			else
				opo_it++;
		}
		std::list<MiniOpocio*>::iterator miniopo_it = miniopo_list.begin();
		while (miniopo_it != miniopo_list.end())
		{
			Oggetto* esito=(*miniopo_it)->live();
			if (esito)
			{
				if (esito == *miniopo_it)
				{
					delete *miniopo_it;
					miniopo_it = miniopo_list.erase(miniopo_it);
				}
				else if(esito->get_type() == alive_opocio)
				{
					delete *miniopo_it;
					miniopo_it = miniopo_list.erase(miniopo_it);
					opo_list.push_front((Opocio *)esito);
				}
			}
			else
				miniopo_it++;
		}
		if(is_counting_time)
			printf("opo: %d ", SDL_GetTicks() - partial_time);
		partial_time = SDL_GetTicks();

		// SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		// SDL_RenderClear(gRenderer);
		if(zoom_level>0.98 && zoom_level <1.02)
			mappa->draw_1(screen_color, int(player_posx), int(player_posy));
		else
			mappa->draw(screen_color, zoom_level, player_posx, player_posy);
		if(is_counting_time)
			printf("map_r: %d ", SDL_GetTicks() - partial_time);
		partial_time = SDL_GetTicks();
		int x_min = std::max(0, int(player_posx));
		int x_max = std::min(MAP_WIDTH - 1, int(player_posx + SCREEN_WIDTH / zoom_level));
		int y_min = std::max(0, int(player_posy));
		int y_max = std::min(MAP_HEIGHT - 1, int(player_posy + SCREEN_HEIGHT / zoom_level));
		Oggetto *oggetto;
		if(zoom_level>0.98 && zoom_level <1.02)
			for (int x = x_min; x <= x_max; x++)
				for (int y = y_min; y <= y_max; y++)
				{
					oggetto = mappa->get_space(x, y);
					if (oggetto)
						oggetto->draw_1(screen_color, int(player_posx), int(player_posy));
				}
		else
		{
			for (int x = x_min; x <= x_max; x++)
				for (int y = y_min; y <= y_max; y++)
				{
					oggetto = mappa->get_space(x, y);
					if (oggetto && (oggetto->get_type()==cibo_type || oggetto->get_type()==pietra_type))
						oggetto->draw(screen_color, zoom_level, player_posx, player_posy);
				}
			for (int x = x_min; x <= x_max; x++)
				for (int y = y_min; y <= y_max; y++)
				{
					oggetto = mappa->get_space(x, y);
					if (oggetto && oggetto->get_type()!=cibo_type && oggetto->get_type()!=pietra_type)
						oggetto->draw(screen_color, zoom_level, player_posx, player_posy);
				}
		}
		if(is_counting_time)
			printf("cose_r: %d ", SDL_GetTicks() - partial_time);
		partial_time = SDL_GetTicks();
		
		SDL_UpdateTexture(texture,NULL,&screen_color[0][0][0],SCREEN_WIDTH * 4);
        SDL_RenderCopy( gRenderer, texture, NULL, NULL );
		SDL_RenderPresent(gRenderer);

		if(is_counting_time)
			printf("ext_r: %d \n", SDL_GetTicks() - partial_time);
		partial_time = SDL_GetTicks();
		cicle_time = SDL_GetTicks() - cicle_time;
		if (cicle_time < SCREEN_TICK_PER_FRAME)
			SDL_Delay( SCREEN_TICK_PER_FRAME - cicle_time );
	}
	diffusion_thread.join();
	close(&gWindow, &gRenderer, player, mappa, opo_list, sasso_list, cibo_list, pietra_list);////
	return 0;
}
