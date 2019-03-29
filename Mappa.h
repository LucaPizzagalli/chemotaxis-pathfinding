#include "Constant.h"
#include "Oggetto.h"
#include <vector>

struct A_smell { int index; float intensity; };

class Mappa
{
  Oggetto **occupazione;
	float *smell2;
  float *smell1;
  float *stable_smell, *unstable_smell;
  std::vector<A_smell> new_smell;
  
public:
  static int draw_type;
  Mappa();
  void compute_diffusion();
  void confirm_diffusion();
  void add_smell(int x, int y, float in_smell);
  float get_smell(int x, int y);
  void set_space(int x, int y, Oggetto *oggetto);
  Oggetto* get_space(int x, int y);
  void draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy);
  void draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy);
  void print();
};
