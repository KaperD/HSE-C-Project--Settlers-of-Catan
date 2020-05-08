//
// Created by mixoz on 21.04.2020.
//

#ifndef UNTITLED3_SDL_H
#define UNTITLED3_SDL_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
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
    BLACK,
    WHITE,
    BLUE,
    RED,
    PINK,
    ORANGE,
    GREEN
};

class Obj {
public:
    Obj(double x1, double y1, double x2,
        double y2, int x, int y, SDL_Texture *_texture, SDL_Texture * texture2_,
        SDL_Texture *_cur_texture, SDL_Rect _dest):
            gui_x1(x1), gui_x2(x2), gui_y1(y1),
            gui_y2(y2), model_x(x), model_y(y),
            texture(_texture), texture2(texture2_), cur_texture(_cur_texture),
            dest(_dest){};

    double gui_x1, gui_y1, gui_x2, gui_y2;
    int model_x, model_y;

    SDL_Texture *texture;
    SDL_Texture *texture2;
    SDL_Texture *cur_texture;

    SDL_Rect dest;

    int built = 0;
    Colour colour;

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
    SDL_Texture *back, *back_ground, *road, *road1,
            *road2, *oct, *cur_road, *cur_road1,
            *cur_road2, *table, *table_1, *table_2,
            *table_time, *house, *house1, *house2,
            *house_cur, *house1_cur, *house2_cur, *Vova, *Vova1, *Vova2;
    SDL_Texture* arr[6];
    SDL_Texture* build_texture_arr[3];
    SDL_Texture* cur_build_texture_arr[3];
    bool quit = false;
    int field_arr[19]{};
    int tmp_sound = 0;
    int render_type{};
    SDL_Renderer *ren;
    SDL_DisplayMode displayMode{};
    SDL_Window *win;
    void load_textures();

    int tmp_coors{};



    GUI();
    ~GUI();
    void destroy_textures();
    void render_background() const;
    void render_tables() const;
    void render_field();
    void render_roads();
    void render_buildings();
    void render_tables_time() const;
    void make_render();
    void get_coors_road();
    void get_coors_building();


    ::game::Event getTurn();

    void add_road(std::pair<int, int> tmp, int player);

    void add_building(std::pair<int, int> tmp, int player);

    int return_road(int x, int y) const;

    int return_building(int x, int y) const;

    std::pair<int, int> tmp_building;

    SDL_Texture *get_building(int i, int type);

    SDL_Texture *get_road(int x, int type);

    SDL_Texture *get_vert_road(int type);
};

} // namespace GUI


#endif //UNTITLED3_SDL_H