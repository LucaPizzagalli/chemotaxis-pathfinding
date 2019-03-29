class Pietra : public Oggetto
{
protected:
	int pietra;
	int color[3];
	int posx, posy;
	Mappa *mappa;

public:
	Pietra(Mappa *in_mappa, int in_posx, int in_posy, int in_pietra);
	int lose_weight(int morso);
	Object_type get_type();
	void draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy);
	void draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy);
	virtual ~Pietra();
};
