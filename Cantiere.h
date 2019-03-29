class Cantiere : public Oggetto
{
protected:
	int cantiere;
	int color[3];
	int posx, posy;
	Mappa *mappa;

public:
	Cantiere(Mappa *in_mappa, int in_posx, int in_posy);
	Object_type get_type();
	void draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy);
	void draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy);
	virtual ~Cantiere();
};
