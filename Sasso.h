class Sasso : public Oggetto
{
protected:
	float dim;
	int color[3];
	float posx, posy;
	Mappa *mappa;

public:
	Sasso(Mappa *in_mappa, float in_posx, float in_posy);
	Object_type get_type();
    void draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy);
	void draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy);
	virtual ~Sasso();
};
