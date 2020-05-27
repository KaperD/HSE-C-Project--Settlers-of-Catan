#ifndef UNTITLED3_SDL_H
#define UNTITLED3_SDL_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <random>
#include <time.h>
#include <mutex>
#include <atomic>
#include <spinlock.h>

#include "game.pb.h"

namespace GUI {

namespace {

class Limiter {
public:
    void storeStartTime();
    void delay();
private:
    const int FPS = 30;
    const int frameDelay = 1000 / FPS;
    uint32_t frameStart = 0;
    int frameTime = 0;
};

} // namespace


class GUI;
void upgrade(GUI* g);
void playMusic(GUI* gui);
void setTimeTable(GUI *gui);

enum Colour {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    NONE
};

enum class Action {
    DICE,
    CARD
};

enum Build {
    X1 = 506,
    X2 = 546,
    Y1 = 130,
    Y2 = 168,
    SCALE_Y = 3
};

enum Rode {
    X1_V = 517,
    X2_V = 545,
    Y1_V = 199,
    Y2_V = 294,
    X1_G = 529,
    X2_G = 600,
    Y1_G = 160,
    Y2_G = 195,
    DX = 8660254,
    DY = 3*50,
    SCALE_X = 100000
};

enum Resourses {
    WOOL, //овцы
    ORE,
    BRICKS, //глина
    WOOD,
    CORN //пшеница
};

class Obj {
public:
    Obj(double x1, double y1, double x2,
        double y2, int x, int y, Colour _colour, SDL_Rect _dest, int _type):
            gui_x1(x1), gui_x2(x2), gui_y1(y1),
            gui_y2(y2), model_x(x), model_y(y),
            colour(_colour), dest(_dest), type(_type){};

    double gui_x1, gui_x2, gui_y1, gui_y2;
    int model_x, model_y;
    int built = 0;
    Colour colour;
    SDL_Rect dest;
    int type = 0;
    bool is(int x, int y) const;
    std::pair<int, int> get_model_coors();
};

class Building_arr {
public:
    Building_arr();
    ~Building_arr();
    std::vector<Obj> vec;
};

class Road_arr {
public:
    Road_arr();
    ~Road_arr();
    std::vector<Obj> vec;
};

class Stage {
public:

private:

};

class Robber_arr {
public:
    Robber_arr(GUI &gui);
    ~Robber_arr();
    std::vector<std::pair<int, int>> vec;
    std::pair<int, int> get_coors(int type, GUI &gui);
    SDL_Texture *texture_oct = nullptr;
    SDL_Texture *texture_robber = nullptr;
    void render(GUI &gui);
    std::atomic<int> x_tmp;
    std::atomic<int> y_tmp;
    std::atomic<int> x_r;
    std::atomic<int> y_r;
    int number;
    void set (int x);
};

class Image {
public:
    explicit Image(std::string s, SDL_Renderer *ren);
    explicit Image(std::string s, int x, int y, int w, int h, SDL_Renderer *ren);
    explicit Image(std::string s, SDL_Rect _dest, SDL_Renderer *ren);
    ~Image();
    void render(SDL_Renderer *ren, SDL_Rect _dest);
    void render(SDL_Renderer *ren);
private:
    SDL_Texture *texture{};
    SDL_Rect dest{};
};

class ImageArr {
public:

private:
    std::vector<Image> vec;
};

class Resourses_arr {
public:
    Resourses_arr();
    ~Resourses_arr();
    std::vector<Obj> vec;
};

class Inscription {
public:
    SDL_Rect dest;
    SDL_Texture *texture;
    Inscription(GUI& gui, int _x, int _y, const std::string &s);
    Inscription() = default;
};


class GUI {
public:
    GUI(int player, int numberOfPlayers);
    ~GUI();
    mutable utility::spinlock mutex_for_render {};

    void setTable(int i);

    void renderCurPlayer();

    Robber_arr *robber = nullptr;
    Road_arr *roads = nullptr;
    Building_arr *buildings = nullptr;
    std::pair<std::atomic<int>, std::atomic<int>> tmp_road;

    Mix_Chunk *sfx, *button_sound, *build_sound, *dice_sound;
    std::pair<SDL_Texture*, int> cur_table;
    SDL_Texture *back, *back_ground, *oct, *cur_road, *cur_road1,
            *cur_road2, *table, *table_1, *table_2, *cur_card_texture,
            *table_time, *house_cur, *house1_cur, *svitok;
    std::vector<SDL_Texture *> message;
    std::atomic<int> table_time_type;
    SDL_Texture* arr[6];
    SDL_Texture* build_texture_arr[3];
    SDL_Texture* cur_build_texture_arr[3];
    std::vector<SDL_Texture *> texture_arr_building[3];
    std::vector<SDL_Texture *> cur_texture_arr_building;

    std::vector<SDL_Texture *> texture_arr_road[3];
    std::vector<SDL_Texture *> cur_texture_arr_road;

    std::vector<SDL_Texture *> dice;
    std::vector<SDL_Texture *> tables_arr;

    SDL_Texture * cur_texture_resourse;
    std::vector<SDL_Texture *> texture_arr_resourses;
    SDL_Texture * texture_resourse_built;
    void updateDevCards (std::vector<bool> vec);
    std::vector<bool> dev_cards_vec = {0, 0, 0, 0, 0};

    void renderBeginingMenu();

    int getPlaceOfGame();

    int getTypeOfGame();

    int getNumOfPlayers();

    int getGameId();

    std::atomic<int> tmp_card;
    std::atomic<int> cur_card;



    std::atomic<bool> quit { false };
    int  end_time_dice = 0;
    std::vector<SDL_Texture *> field_arr;
    int tmp_sound = 0;
    std::atomic<int> render_type;
    SDL_Renderer *ren;
    SDL_DisplayMode displayMode{};
    SDL_Window *win;
    void loadTextures(utility::Random& random, GUI& gui);

    std::atomic<int> tmp_coors { 0 };

    std::atomic<int> dice1 { 0 };
    std::atomic<int> dice2 { 0 };

    SDL_Texture *ppp;


    SDL_Color color = { 243, 195, 79, 255 };
    SDL_Color color_const_table = { 79, 51, 14, 255 };
    SDL_Texture* Text(const std::string &message);
    SDL_Texture* back_resourse;
    SDL_Texture *texture_svitok_up, *texture_svitok_down;
    SDL_Texture *dice_shadow;
    std::vector<SDL_Texture *> table_shadow;
    std::vector<SDL_Texture *> texture_arr_card;
    void makeTextureConstTable(std::vector<int>& vec, SDL_Surface* buff, SDL_Texture *&ans, int type);
    void destroyTextures();
    void renderBackground() const;
    void renderTables() const;
    void renderField();
    void renderRoads();
    void renderBuildings();
    void renderDice();
    void addDice(int x, int y);
    void renderTablesTime() const;
    void makeRender(GUI &gui);
    void getCoorsRoad();
    void getCoorsBuilding();
    void getCoorsCard();
    void setRobber(int x);

    Inscription _build_road, _build, _settlement, _end_turn, _go_back, _go_back2, _go_back3,
                 _roll_the_dice, _play_a_card,_local_game, _game_on_server, _ok,
                 _start_new_game, _join_game, _exit, _2_Players, _3_Players, _buy_card,
                _4_Players, _type_game_id, _exchange, _resources, _next_phase;

    ::game::Event FirstStage();
    ::game::Event ThirdStage();
    ::game::Event SecondStage ();

    ::game::Event getEvent();

    void addRoad(std::pair<int, int> tmp, int player);
    void addBuilding(std::pair<int, int> tmp, int player);
    void renderCards();
    int returnRoad(int x, int y) const;

    int getCoorsResoursesCards();

    int returnBuilding(int x, int y) const;

    int returnResourses(int x, int y) const;

    std::pair<std::atomic<int>, std::atomic<int>> tmp_building;
    std::pair<std::atomic<int>, std::atomic<int>> tmp_resours;

    Resourses_arr *resourses_img = nullptr;

    std::vector<int> players_points;
    std::vector<std::string> players_names;
    int cur_player = 0;

    void updatePlayer(int x);

    void renderText() const;

    int num_players = 4;
    int my_player;
    void updatePoints(std::vector<int> vec);

    void updateResourses(std::vector<int> vec);
    std::vector<int> resourses;

    void renderConstTable();
    void addPlayerName(int x, std::string s);

    int getCoorsRobber(GUI &gui);

    void getCoorsResourses();


    void renderResourses() const;

    std::pair<int, int> tmp_resourses;
    std::pair<int, int> tmp_resourses_num;

};

} // namespace GUI


#endif //UNTITLED3_SDL_H