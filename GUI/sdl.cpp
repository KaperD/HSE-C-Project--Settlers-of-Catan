#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <cassert>
#include "sdl.h"

namespace GUI {

using ::game::Event;
using ::game::EventType;

//void play_music(GUI* gui) {
//    while (!gui->quit){
//        std::cout << 1;
//        Mix_PlayChannel(-1, gui->sfx, 0);
//        for (int i = 0; i < 18000; ++i) {
//            if (gui->quit) return;
//            SDL_Delay(10);
//        }
//    }
//}

void GUI::load_textures() {
    for (int i = 0; i < 19; ++i) {
        field_arr[i] = rand()%6;
    }
    std::string s = "image/oct .bmp";
    for (int i = 0; i < 6; ++i) {
        s[9] = i + '0';
        arr[i] = IMG_LoadTexture(ren, s.c_str());
    }
    back_ground = IMG_LoadTexture(ren, "image/back_ground.bmp");
    back = IMG_LoadTexture(ren, "image/back.bmp");
    road = IMG_LoadTexture(ren, "image/road.bmp");
    road1 = IMG_LoadTexture(ren, "image/road1.bmp");
    road2 = IMG_LoadTexture(ren, "image/road2.bmp");
    cur_road = IMG_LoadTexture(ren, "image/cur_road.bmp");
    cur_road1 = IMG_LoadTexture(ren, "image/cur_road1.bmp");
    cur_road2 = IMG_LoadTexture(ren, "image/cur_road2.bmp");
    table = IMG_LoadTexture(ren, "image/table.bmp");
    assert(table != nullptr);
    table_1 = IMG_LoadTexture(ren, "image/table_1.bmp");
    table_2 = IMG_LoadTexture(ren, "image/table_1.bmp");
    table_time = IMG_LoadTexture(ren, "image/table_time.bmp");
    house = IMG_LoadTexture(ren, "image/house.bmp");
    house_cur = IMG_LoadTexture(ren, "image/house_cur.bmp");
    house1_cur = IMG_LoadTexture(ren, "image/house_cur.bmp");
    house2_cur = IMG_LoadTexture(ren, "image/house_cur.bmp");
    house1 = IMG_LoadTexture(ren, "image/house1.bmp");
    house2 = IMG_LoadTexture(ren, "image/house2.bmp");
    build_texture_arr[0] = house;
    cur_build_texture_arr[0] = house_cur;
    build_texture_arr[1] = house1;
    cur_build_texture_arr[1] = house1_cur;
    build_texture_arr[2] = house2;
    cur_build_texture_arr[2] = house2_cur;
//    sfx = nullptr;
//    sfx = Mix_LoadWAV("image/music.wav");
//    button_sound = Mix_LoadWAV("image/button_sound.wav");
//    build_sound = Mix_LoadWAV("image/build_sound.wav");
//    if (sfx == nullptr)  std::cout << "HUY";
}

void GUI::destroy_textures() {
    for (auto & i : arr) {
        SDL_DestroyTexture(i);
    }
//    Mix_CloseAudio();
    SDL_DestroyTexture(road);
    SDL_DestroyTexture(road1);
    SDL_DestroyTexture(road2);
    SDL_DestroyTexture(back_ground);
    SDL_DestroyTexture(back);
}

void GUI::render_background() const {
    SDL_Rect dest1;
    dest1.x = 0;
    dest1.y = 0;
    dest1.w = displayMode.w;
    dest1.h = displayMode.h;
    assert(ren != nullptr);
    SDL_RenderCopy(ren,back_ground,nullptr,&dest1); //Копируем в рендер фон
    SDL_RenderCopy(ren,back,nullptr,&dest1); //Копируем в рендер фон
}

void GUI::render_tables() const {
    SDL_Rect dest;
    dest.x = 200;
    dest.y = 98;
    dest.w = 480 - 200;
    dest.h = 280 - 98;
    if (render_type == 0)
        SDL_RenderCopy(ren, table, nullptr,&dest);
    if (render_type)
        SDL_RenderCopy(ren, table_1, nullptr,&dest);
    dest.x = 200;
    dest.y = 300;
    dest.w = 480 - 200;
    dest.h = 280 - 98;
    if (render_type == 0)
        SDL_RenderCopy(ren, table_2, nullptr,&dest);
}


void GUI::render_field() {
    SDL_Rect dest;
    dest.x = displayMode.w / 2 - 250*sqrt(3);
    dest.y = displayMode.h / 2 - 550;
    dest.w = 100*sqrt(3);
    dest.h = 200;
    int k = 3;
    int it = 0;
    for (int i = 0; i < 5; ++i){
        dest.y += 150;
        for (int q = 0; q < k; ++q){
            dest.x += 100*sqrt(3);
            SDL_RenderCopy(ren, arr[field_arr[it++]], nullptr, &dest); //Копируем в рендер персонажа
        }
        dest.x-= 100*sqrt(3)*k;
        if (i < 2) {k+=1;dest.x-= 50*sqrt(3);}
        else {k-=1;dest.x+= 50*sqrt(3);}
    }

}

void GUI::render_roads() {
    std::lock_guard<std::mutex> lock(mu);
    for (auto e: roads->vec) {
        if (e.built)
            SDL_RenderCopy(ren, e.texture, nullptr, &e.dest);
    }
    if (render_type == 1) {
        int e = return_road(tmp_road.first, tmp_road.second);
        if (e == -1) return;
        SDL_RenderCopy(ren, roads->vec[e].cur_texture, nullptr, &roads->vec[e].dest);
    }
}


void GUI::render_buildings() {
    std::lock_guard<std::mutex> lock(mu);
    for (auto e: buildings->vec) {
        if (e.built)
            SDL_RenderCopy(ren, e.texture, nullptr, &e.dest);
    }
    if (render_type == 2) {
        int e = return_building(tmp_building.first, tmp_building.second);
        if (e == -1) return;
        SDL_RenderCopy(ren, buildings->vec[e].cur_texture, nullptr, &buildings->vec[e].dest);
    }
}

void GUI::render_tables_time() const {
    SDL_Rect (dest);
    dest.x = 200 + 500;
    dest.y = 98 - 20;
    dest.w = 480 - 200;
    dest.h = 280 - 98;
    if (clock() < cur_table.second) SDL_RenderCopy(ren, cur_table.first, nullptr, &dest);
}



void GUI::make_render() {
    SDL_RenderClear(ren);
    render_background();
    render_field();
    render_roads();
    render_buildings();
    render_tables();
    render_tables_time();

}

void upgrade(GUI* g) {
    while (true) {
//        std::cout << 3;
        SDL_Delay(20);
        g->make_render();
        SDL_RenderPresent(g->ren);
        if (g->quit) return;
    }
}


GUI::GUI() {
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Init(SDL_INIT_AUDIO);
//    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    SDL_GetDesktopDisplayMode(0,&displayMode);
    win = SDL_CreateWindow("Hello World!", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_FULLSCREEN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}


void GUI::get_coors_road () {
//    Mix_PlayChannel(-1, build_sound, 0);
    int old_render_type = render_type;
    render_type = 1;
    //SDL_Rect dest;
    SDL_Event e;
    clock_t begin_time = clock();
    while (!quit) {
        clock_t end_time = clock();
        if (end_time - begin_time > CLOCKS_PER_SEC * 30) {
            cur_table = std::make_pair(table_time, clock() + 5*CLOCKS_PER_SEC);
            render_type = old_render_type;
            return;
        }
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                break;
            }
            if( e.type == SDL_MOUSEMOTION ) {
                int x,y;
                x = e.motion.x;
                y = e.motion.y;
                tmp_road = std::make_pair(x, y);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
//                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                std::cout << x << ' ' << y << '\n';
                if (x > 200 && x < 480 && y > 98 && y < 280) {
                    render_type = old_render_type;
                    return;
                }
                tmp_coors = return_road(x, y);
                if (tmp_coors != -1) return;
            }
        }
    }
}

void GUI::get_coors_building () {
//    Mix_PlayChannel(-1, build_sound, 0);
    int old_render_type = render_type;
    render_type = 2;
    //SDL_Rect dest;
    SDL_Event e;
    clock_t begin_time = clock();
    while (!quit) {
        clock_t end_time = clock();
        if (end_time - begin_time > CLOCKS_PER_SEC * 30) {
            cur_table = std::make_pair(table_time, clock() + 5*CLOCKS_PER_SEC);
            render_type = old_render_type;
            return;
        }
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                break;
            }
            if( e.type == SDL_MOUSEMOTION ) {
                int x,y;
                x = e.motion.x;
                y = e.motion.y;
                tmp_house = std::make_pair(x, y);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
//                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                std::cout << x << ' ' << y << '\n';
                if (x > 200 && x < 480 && y > 98 && y < 280) {
                    render_type = old_render_type;
                    return;
                }
                tmp_coors = return_road(x, y);
                if (tmp_coors != -1) return;
            }
        }
    }
}

Event GUI::getTurn () {
    SDL_Event e;
    render_type = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                Event event;
                event.set_type(EventType::ENDGAME);
                return event;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                Event event;
                event.set_type(EventType::ENDGAME);
                return event;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
//                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > 200 && x < 480 && y > 98 && y < 280) { // дорога
                    get_coors_road();
                    auto p = roads->vec[tmp_coors].get_model_coors();
                    Event event;
                    event.set_type(EventType::BUILD);
                    auto q = event.mutable_buildinfo();
                    q->set_buildingtype(3);
                    q->set_x(p.first);
                    q->set_y(p.second);
                    return event;
                }
                if (x > 200 && x < 480 && y > 300 && y < 482) { // деревня
                    get_coors_building();
                    auto p = buildings->vec[tmp_coors].get_model_coors();
                    Event event;
                    event.set_type(EventType::BUILD);
                    auto q = event.mutable_buildinfo();
                    q->set_buildingtype(1);
                    q->set_x(p.first);
                    q->set_y(p.second);
                    return event;
                }
            }
        }
    }
    Event event;
    event.set_type(EventType::ENDGAME);
    return event;
}

bool Obj::is(int x, int y) const {
    return x <= gui_x1 && x >= gui_x2 && y <= gui_y1 && y >= gui_y2;
}

std::pair<int, int> Obj::get_model_coors() {
    return std::make_pair(model_x, model_y);
}

SDL_Texture *GUI::get_road(int x, int type) {
    return type ? (x % 2 == 0 ? road1 : road2) : (x % 2 == 0 ? cur_road1 : cur_road2);
}

SDL_Texture *GUI::get_vert_road(int type) {
    return type ? road : cur_road;
}

void GUI::add_road(std::pair<int, int> tmp, int player) {
    static_cast<void>(player);
    std::lock_guard<std::mutex> lock(mu);
    for (auto e:roads->vec) {
        if (tmp.first == e.model_x && tmp.second == e.model_y) {
            e.built++;
            return;
        }
    }
}

int GUI::return_road(int x, int y) const {
    for (int i = 0; i < roads->vec.size(); ++i)
        if (roads->vec[i].is(x, y)) return i;
    return -1;
}

int GUI::return_building(int x, int y) const {
    for (int i = 0; i < buildings->vec.size(); ++i)
        if (buildings->vec[i].is(x, y)) return i;
    return -1;
}

Road_arr::Road_arr(GUI& gui) {
    SDL_Rect dest;
    for (int i = 0; i < 5; ++i){
        int a = 0;
        int b = 11;
        if (i == 0) {a = 1; b = 10;}
        if (i == 4) {a = 1; b = 10;}
        for (int j = a; j < b; ++j){
            if ((i+j) % 2 != 0) continue;
            dest.x = 470 + 20 + j*50*sqrt(3);
            dest.y = 150 + 50 + 3*i*50;
            dest.w = 80;
            dest.h = 100;
            SDL_Texture *texture = gui.get_vert_road(0);
            SDL_Texture *cur_texture = gui.get_vert_road(1);
            Obj tmp(X1_V + j * DX / SCALE_X, Y1_V + i * DY,
                    X2_V + j * DX / SCALE_X, Y2_V + i * DY,
                    i * 2 + 1, j * 2,
                    texture, cur_texture, dest);
            vec.push_back(tmp);
            break;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int a = 0;
        int b = 10;
        if (i == 0) {a = 2; b = 8;}
        if (i == 1) {a = 1; b = 9;}
        if (i == 5) {a = 2; b = 8;}
        if (i == 4) {a = 1; b = 9;}
        for (int j = a; j < b; ++j) {
            dest.x = 510 + 20  + j*50*sqrt(3);
            dest.y = 140 + 3*i*50;
            dest.w = 50*sqrt(3);
            dest.h = 80;
            SDL_Texture *texture = gui.get_road(i+j, 0);
            SDL_Texture *cur_texture = gui.get_road(i+j, 1);
            Obj tmp(X1_G + j * DX / SCALE_X, Y1_G + i * DY,
                    X2_G + j * DX / SCALE_X, Y2_G + i * DY,
                    i * 2, 	j * 2 + 1,
                    texture, cur_texture, dest);
            vec.push_back(tmp);
        }
    }
}

Road_arr::~Road_arr() = default;

GUI::~GUI() {
    destroy_textures();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

SDL_Texture *GUI::get_building(int i, int type) {
    if (i == 0) return type ? house : house_cur;
    if (i == 1) return type ? house1 : house1_cur;
    if (i == 2) return type ? house2 : house2_cur;
}

void GUI::add_building(std::pair<int, int> tmp, int player) {
    std::lock_guard<std::mutex> lock(mu);
    for (auto e:buildings->vec) {
        if (tmp.first == e.model_x && tmp.second == e.model_y) {
            e.built++;
            e.texture = build_texture_arr[player];
            e.cur_texture = cur_build_texture_arr[player];
            return;
        }
    }
}


Building_arr::Building_arr(GUI& gui) {
    SDL_Rect dest;
    for (int i = 0; i < 17; ++i) {
        int a = 0;
        int b = 11;
        if (i == 0) {a = 2; b = 8;}
        if (i == 1) {a = 1; b = 9;}
        if (i == 3) {a = 1; b = 9;}
        if (i == 16) {a = 2; b = 8;}
        if (i == 15) {a = 1; b = 9;}
        if (i == 13) {a = 1; b = 9;}
        if (i == 5) {a = 2; b = 8;}
        if (i == 4) {a = 1; b = 10;}
        for (int j = a; j < b; ++j){
            if ((i + 1)%3 == 0) continue;
            if ((i + j) % 2 == 0) continue;
            dest.x = 506 - 30 + j * DX / SCALE_X;
            dest.y = 130 - 40 + i * DY / SCALE_Y;
            dest.w = 100;
            dest.h = 100;
            SDL_Texture *texture = gui.get_building(0, 0);
            SDL_Texture *cur_texture = gui.get_building(0, 1);
            Obj tmp(X1 + j * DX / SCALE_X, Y1 + i * DY / SCALE_Y,
                    X2 + j * DX / SCALE_X, Y2 + i * DY / SCALE_Y,
                    i * 2, 	j * 2 + 1,
                    texture, cur_texture, dest);
            vec.push_back(tmp);
        }
    }
}

Building_arr::~Building_arr() = default;


} // namespace GUI