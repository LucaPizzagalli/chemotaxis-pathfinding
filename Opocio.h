#define attack_opocio -1
#define eat_food 0
#define eat_opocio 1
#define find_cantiere 2
#define find_pietra 3
#define ask_pietra 4
#define ask_cantiere 5
#define N_priority 6

class Opocio : public Oggetto
{
protected:
    double dim, speed;
    int energia;
    int vita;
    int carne;
    int gravidanza;
    float cantiere_proximity;
    float pietra_proximity;
    int color[3];
    double posx, posy;
    double velx, vely, vel_angle;
    int left_steps;
    //bool extra_step;
    float last_smell;
    int esitazioni;
    Oggetto *calpestato;
    Object_type carried;
    Oggetto *enemy;
    Object_type priority[N_priority];
    Mappa *mappa;

public:
    static int color_type;
    Opocio(Mappa *in_mappa, float in_posx, float in_posy);
    Object_type get_type();
    void generate_meta(int coherence);
    Oggetto* live();
    int what_nearby(Oggetto **vicino);
    void injured(Oggetto *in_enemy, int damage);
    int lose_weight(int morso);
    void bite(Oggetto *prey);
    Oggetto* move();
    void update_information(float other_cantiere, float other_pietra);
    bool is_searching_cantiere();
    bool is_searching_pietra();
    float get_cantiere_prox();
    float get_pietra_prox();
    void set_cantiere_prox(float in_cantiere_proximity);
    void set_pietra_prox(float in_pietra_proximity);
    void draw_1(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], int player_posx, int player_posy);
    void draw(unsigned char screen_color[SCREEN_HEIGHT][SCREEN_WIDTH][4], float zoom_level, float player_posx, float player_posy);
    void print();
    virtual ~Opocio();
};
