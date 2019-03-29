class Player
{
protected:
    float *p_posx, *p_posy;
    float velx, vely;
    float *p_zoom_level;
    float velz;

public:
    Player(float *in_p_posx, float *in_p_posy, float *in_p_zoom_level);
    void add_velx(float in_velx);
    void add_vely(float in_vely);
    void add_velz(float in_velz);
    void move();
    float get_zoom_level();
    float get_posx();
    float get_posy();
};
