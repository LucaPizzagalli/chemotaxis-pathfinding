class Cibo : public Oggetto
{
protected:
	int cibo;
	int color[3];
	int posx, posy;
	Mappa *mappa;

public:
	Cibo(Mappa *in_mappa, int in_posx, int in_posy, int in_cibo);
	int lose_weight(int morso);
	Object_type get_type();
	void draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy);
	virtual ~Cibo();
};
