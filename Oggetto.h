enum Object_type {none = 0, alive_opocio, dead_opocio, cibo_type, sasso_type, pietra_type,
    miniopocio_type, cantiere_type};

class Oggetto
{
public:
    virtual void draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy) = 0;
    virtual void draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy) = 0;
    virtual Object_type get_type() = 0;
    virtual void injured(Oggetto* oggetto, int damage){}
    virtual int lose_weight(int morso) { return 0; }
};
