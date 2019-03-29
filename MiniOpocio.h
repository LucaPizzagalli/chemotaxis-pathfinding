class MiniOpocio : public Oggetto
{
protected:
    double dim, speed;
    int vita;
    int infanzia;
    int color[3];
    double posx, posy;
    double velx, vely;
    int left_steps;
    bool extra_step;
    float last_smell;
    Oggetto *calpestato;
    Mappa *mappa;

public:
    MiniOpocio(Mappa *in_mappa, float in_posx, float in_posy);
    Object_type get_type();
    void generate_meta();
    Oggetto* live();
    void injured(Oggetto *in_enemy, int damage);
    bool move();
    void draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy);
	void draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy);
    virtual ~MiniOpocio();
};
