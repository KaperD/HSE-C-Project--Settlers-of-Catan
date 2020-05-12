//
// Created by mixoz on 21.04.2020.
//

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

#include "game.pb.h"

namespace GUI {


class GUI;
void upgrade(GUI* g);
void play_music(GUI* gui);


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


class Obj {
public:
    Obj(double x1, double y1, double x2,
        double y2, int x, int y, Colour _colour, SDL_Rect _dest, int _type):
            gui_x1(x1), gui_x2(x2), gui_y1(y1),
            gui_y2(y2), model_x(x), model_y(y),
            colour(_colour), dest(_dest), type(_type){};

    double gui_x1, gui_y1, gui_x2, gui_y2;
    int model_x, model_y;

    SDL_Rect dest;

    int built = 0;
    Colour colour;
    int type = 0;

    bool is(int x, int y) const;
    std::pair<int, int> get_model_coors();
};

class Building_arr {
public:
    explicit Building_arr(GUI& gui);
    ~Building_arr();
    std::vector<Obj> vec;
};

class Road_arr {
public:
    explicit Road_arr(GUI& gui);
    ~Road_arr();
    std::vector<Obj> vec;
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

class Inscription {
    public:
        SDL_Rect dest;
        SDL_Texture *texture;
        explicit Inscription(GUI& gui, int _x, int _y,const std::string &s);
        Inscription() = default;
    };


class GUI {
public:

    
    std::mutex mutex_for_roads {};  
    std::mutex mutex_for_buildings {};
    std::mutex mutex_for_ren {};

    Road_arr *roads = nullptr;
    Building_arr *buildings = nullptr;
    std::pair<int, int> tmp_road;

    Mix_Chunk *sfx, *button_sound, *build_sound;
    std::pair<SDL_Texture*, int> cur_table;
    SDL_Texture *back, *back_ground, *oct, *cur_road, *cur_road1,
            *cur_road2, *table, *table_1, *table_2,
            *table_time, *house_cur, *house1_cur, *svitok;
    SDL_Texture* arr[6];
    SDL_Texture* build_texture_arr[3];
    SDL_Texture* cur_build_texture_arr[3];

    std::vector<SDL_Texture *> texture_arr_building[3];
    std::vector<SDL_Texture *> cur_texture_arr_building;

    std::vector<SDL_Texture *> texture_arr_road[3];
    std::vector<SDL_Texture *> cur_texture_arr_road;

    std::vector<SDL_Texture *> dice;
    std::vector<SDL_Texture *> tables_arr;

    void render_begining_menu();

    int get_place_of_game();

    int get_type_of_game();

    int get_num_of_players();

    int get_game_id();

    bool quit = false;
    int  end_time_dice = 0;
    std::vector<SDL_Texture *> field_arr;
    int tmp_sound = 0;
    int render_type;
    SDL_Renderer *ren;
    SDL_DisplayMode displayMode{};
    SDL_Window *win;
    void load_textures(utility::Random& random, GUI& gui);

    int tmp_coors{};

    int dice1 = 0;
    int dice2 = 0;

    SDL_Texture *ppp;
    

    SDL_Color color = { 243, 195, 79, 255 };
    SDL_Color color_const_table = { 79, 51, 14, 255 };
    SDL_Texture* Text(const std::string &message);
    SDL_Surface *svitok_up, *svitok_down;
    SDL_Surface *svitok_up1, *svitok_down1;
    SDL_Texture *texture_svitok_up, *texture_svitok_down;
    SDL_Texture *dice_shadow;
    std::vector<SDL_Texture *> table_shadow;
    void make_texture_const_table(std::vector<int> vec, SDL_Surface x, SDL_Texture *&ans, int type);
    GUI();
    ~GUI();
    void destroy_textures();
    void render_background() const;
    void render_tables() const;
    void render_field();
    void render_roads();
    void render_buildings();
    void render_dice();
    void add_dice(int x, int y);
    void render_tables_time() const;
    void make_render();
    void get_coors_road();
    void get_coors_building();

    Inscription _build_road, _build, _settlement, _end_turn, _go_back,
                 _roll_the_dice, _play_a_card,_local_game, _game_on_server, 
                 _start_new_game, _join_game, _exit, _2_Players, _3_Players,
                _4_Players, _type_game_id;

    ::game::Event FirstStage();
    ::game::Event ThirdStage();

    ::game::Event getEvent();

    void add_road(std::pair<int, int> tmp, int player);

    void add_building(std::pair<int, int> tmp, int player);

    int return_road(int x, int y) const;

    int return_building(int x, int y) const;

    std::pair<int, int> tmp_building;

    SDL_Texture *get_building(int i, int type);

    SDL_Texture *get_road(int x, int type);

    SDL_Texture *get_vert_road(int type);

    std::vector<int> players_points;
    std::vector<std::string> players_names;
    int cur_player = 0;

    void update_player(int x);

    void render_text() const;

    int num_players = 4;

    void update_points(std::vector<int> vec);

    void update_resourses(std::vector<int> vec);
    std::vector<int> resourses;

    void render_const_table();
    void add_player_name(int x, std::string s);

};

} // namespace GUI


#endif //UNTITLED3_SDL_H