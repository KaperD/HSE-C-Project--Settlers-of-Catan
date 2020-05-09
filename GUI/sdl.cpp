#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <cassert>
#include <random.h>
#include "sdl.h"

namespace GUI {

namespace {

class Limiter {
public:
    void storeStartTime() {
        frameStart = SDL_GetTicks();
    }

    void delay() {
        frameTime = static_cast<int>(SDL_GetTicks() - frameStart);
        if (frameDelay > frameTime) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay - frameTime));
        }
    }

private:
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    uint32_t frameStart = 0;
    int frameTime = 0;
};

} // namespace



using ::game::Event;
using ::game::EventType;

void play_music(GUI* gui) {
    while (!gui->quit){
        std::cout << "MUSIC" << std::endl;
        Mix_PlayChannel(-1, gui->sfx, 0);
        for (int i = 0; i < 18000; ++i) {
            if (gui->quit) return;
            SDL_Delay(1000);
        }
    }
}

void GUI::load_textures(utility::Random& random) {
    auto randomResouresAndNumbers = random.generateResourcesAndNumbers();
    std::string s = "image/oct .bmp";

    for (int i = 0; i < 19; ++i) {
        int resource = randomResouresAndNumbers[i].resource;
        int number = randomResouresAndNumbers[i].number;
        s[9] = resource + '0'; // TODO: Числа в названиях файлов должны соответствовать enum в модели


        SDL_Surface* numberImg = IMG_Load("image/number2.bmp"); // TODO: Вместо 2 нужно ставить число number, кроме случая, когда число 0, в этом случае не нужно изображение чилса

        SDL_Surface* hex = IMG_Load(s.c_str());
        SDL_Rect dest;
        dest.x = numberImg->w / 3 + 36;
        dest.y = numberImg->h / 3 + 78;
        dest.w = static_cast<int>(static_cast<double>(numberImg->w) * 0.5);
        dest.h = static_cast<int>(static_cast<double>(numberImg->h) * 0.5);
        if (SDL_BlitScaled(numberImg, NULL, hex, &dest) != 0) {
            std::cout << "Wrong Blit" << std::endl;
        }
        field_arr[i] = SDL_CreateTextureFromSurface(ren, hex);
        SDL_FreeSurface(hex);
    }

    back_ground = IMG_LoadTexture(ren, "image/back_ground.bmp");
    back = IMG_LoadTexture(ren, "image/back.bmp");
    
    (cur_texture_arr_road).push_back(IMG_LoadTexture(ren, "image/cur_road.bmp"));
    (cur_texture_arr_road).push_back(IMG_LoadTexture(ren, "image/cur_road1.bmp"));
    (cur_texture_arr_road).push_back(IMG_LoadTexture(ren, "image/cur_road2.bmp"));

    table = IMG_LoadTexture(ren, "image/table.bmp");
    table_1 = IMG_LoadTexture(ren, "image/table_1.bmp");
    table_2 = IMG_LoadTexture(ren, "image/table_1.bmp");
    table_time = IMG_LoadTexture(ren, "image/table_time.bmp");
    (cur_texture_arr_building).push_back(IMG_LoadTexture(ren, "image/cur_house.bmp"));
    (cur_texture_arr_building).push_back(IMG_LoadTexture(ren, "image/cur_house1.bmp"));
   
    (texture_arr_building[0]).push_back(IMG_LoadTexture(ren, "image/house_yellow.bmp"));
    (texture_arr_building[0]).push_back(IMG_LoadTexture(ren, "image/house_red.bmp"));
    (texture_arr_building[0]).push_back(IMG_LoadTexture(ren, "image/house_green.bmp"));
    (texture_arr_building[0]).push_back(IMG_LoadTexture(ren, "image/house_blue.bmp"));

    (texture_arr_building[1]).push_back(IMG_LoadTexture(ren, "image/house1_yellow.bmp"));
    (texture_arr_building[1]).push_back(IMG_LoadTexture(ren, "image/house1_red.bmp"));
    (texture_arr_building[1]).push_back(IMG_LoadTexture(ren, "image/house1_green.bmp"));
    (texture_arr_building[1]).push_back(IMG_LoadTexture(ren, "image/house1_blue.bmp"));

    (texture_arr_road[0]).push_back(IMG_LoadTexture(ren, "image/road_yellow.bmp"));
    (texture_arr_road[0]).push_back(IMG_LoadTexture(ren, "image/road_red.bmp"));
    (texture_arr_road[0]).push_back(IMG_LoadTexture(ren, "image/road_green.bmp"));
    (texture_arr_road[0]).push_back(IMG_LoadTexture(ren, "image/road_blue.bmp"));

    (texture_arr_road[1]).push_back(IMG_LoadTexture(ren, "image/road1_yellow.bmp"));
    (texture_arr_road[1]).push_back(IMG_LoadTexture(ren, "image/road1_red.bmp"));
    (texture_arr_road[1]).push_back(IMG_LoadTexture(ren, "image/road1_green.bmp"));
    (texture_arr_road[1]).push_back(IMG_LoadTexture(ren, "image/road1_blue.bmp"));

    (texture_arr_road[2]).push_back(IMG_LoadTexture(ren, "image/road2_yellow.bmp"));
    (texture_arr_road[2]).push_back(IMG_LoadTexture(ren, "image/road2_red.bmp"));
    (texture_arr_road[2]).push_back(IMG_LoadTexture(ren, "image/road2_green.bmp"));
    (texture_arr_road[2]).push_back(IMG_LoadTexture(ren, "image/road2_blue.bmp"));

    


//    sfx = nullptr;
//    sfx = Mix_LoadWAV("image/music.wav");
//    button_sound = Mix_LoadWAV("image/button_sound.wav");
//    build_sound = Mix_LoadWAV("image/build_sound.wav");
//    if (sfx == nullptr)  std::cout << "Hhhh";
}

void GUI::destroy_textures() { // TODO: Удалять всё, а не только часть
    for (auto & i : field_arr) {
        SDL_DestroyTexture(i);
    }
    Mix_CloseAudio();
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
    assert(ren != nullptr); // TODO: гонка данных --- ren
    SDL_RenderCopy(ren, back_ground, nullptr, &dest1); //Копируем в рендер фон // TODO: гонка данных --- ren
    SDL_RenderCopy(ren, back, nullptr, &dest1); //Копируем в рендер фон // TODO: гонка данных --- ren
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
            SDL_RenderCopy(ren, field_arr[it++], nullptr, &dest); //Копируем в рендер персонажа
        }
        dest.x-= 100*sqrt(3)*k;
        if (i < 2) {k+=1;dest.x-= 50*sqrt(3);}
        else {k-=1;dest.x+= 50*sqrt(3);}
    }

}

void GUI::render_roads() {
    std::lock_guard<std::mutex> lock(mutex_for_roads);
    for (auto e: roads->vec) {
        if (e.built) {
            SDL_RenderCopy(ren, texture_arr_road[e.type][e.colour], nullptr, &e.dest);
        }

    }
    if (render_type == 1) {
        int e = return_road(tmp_road.first, tmp_road.second);
        if (e == -1) return;
        SDL_RenderCopy(ren,  cur_texture_arr_road[roads->vec[e].type], nullptr, &roads->vec[e].dest);
    }
}


void GUI::render_buildings() {
    std::lock_guard<std::mutex> lock(mutex_for_roads);
    for (auto e: buildings->vec) {
        if (e.built) {

            SDL_RenderCopy(ren, texture_arr_building[e.built][e.colour], nullptr, &e.dest);
        }
    }
    if (render_type == 2) {
        int e = return_building(tmp_building.first, tmp_building.second);
        if (e == -1) return;
        SDL_RenderCopy(ren, cur_texture_arr_building[buildings->vec[e].built] , nullptr, &buildings->vec[e].dest);
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



void GUI::make_render() { // TODO: гонка данных --- ren
    SDL_RenderClear(ren);
    render_background();
    render_field();
    render_roads();
    render_buildings();
    render_tables();
    render_tables_time();

}

void upgrade(GUI* g) {
    constexpr int FPS = 60;
    constexpr int frameDelay = 1000 / FPS;

    uint32_t frameStart = 0;
    int frameTime = 0;

    while (true) {
        frameStart = SDL_GetTicks();
        // TODO: сделать mutex и захватывать его с помощью std::lock_guard
        g->make_render();
        SDL_RenderPresent(g->ren);
        if (g->quit) return;

        frameTime = static_cast<int>(SDL_GetTicks() - frameStart);
        if (frameDelay > frameTime) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay - frameTime));
        }
    }
}


GUI::GUI() {
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    SDL_GetDesktopDisplayMode(0,&displayMode);
    win = SDL_CreateWindow("Settlers of Catan", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //std::cout << "Sync " << SDL_GL_SetSwapInterval(1) << std::endl;
}


void GUI::get_coors_road () {
    Mix_PlayChannel(-1, build_sound, 0);
    int old_render_type = render_type;
    render_type = 1;
    //SDL_Rect dest;
    SDL_Event e;
    clock_t begin_time = clock();

    Limiter limit;

    while (!quit) {

        limit.storeStartTime(); // засекает начало итерации

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
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                if (x > 200 && x < 480 && y > 98 && y < 280) {
                    render_type = old_render_type;
                    return;
                }
                tmp_coors = return_road(x, y);
                std::cout << x << ' ' << y << '\n';
                if (tmp_coors != -1) return;
            }
        }

        limit.delay(); // Ждет, если операции выполнились слишком быстро

    }
}

void GUI::get_coors_building () {
    Mix_PlayChannel(-1, build_sound, 0);
    int old_render_type = render_type;
    render_type = 2;
    //SDL_Rect dest;
    SDL_Event e;
    clock_t begin_time = clock();

    Limiter limit;

    while (!quit) {

        limit.storeStartTime();

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
                tmp_building = std::make_pair(x, y);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                std::cout << "GGGG" << '\n';
                if (x > 200 && x < 480 && y > 98 && y < 280) {
                    render_type = old_render_type;
                    return;
                }
                tmp_coors = return_building(x, y);
                if (tmp_coors != -1) return;
            }
        }

        limit.delay();

    }
}

Event GUI::getTurn () {
    SDL_Event e;
    render_type = 0;

    Limiter limit;

    while (!quit) {

        limit.storeStartTime();

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
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > 200 && x < 480 && y > 98 && y < 280) { // дорога
                    get_coors_road();
                    if (render_type == 0) continue;
                    auto p = roads->vec[tmp_coors].get_model_coors();
                    Event event;
                    event.set_type(EventType::BUILD);
                    auto q = event.mutable_buildinfo();
                    q->set_buildingtype(3);
                    q->set_x(p.first);
                    q->set_y(p.second);
                    return event;
                }
                if (x > 200 && x < 480 && y > 300 && y < 482) { // derevnia
                    get_coors_building();
                    if (render_type == 0) continue;
                    auto p = buildings->vec[tmp_coors].get_model_coors();
                    Event event;
                    event.set_type(EventType::BUILD);
                    auto q = event.mutable_buildinfo();
                    q->set_buildingtype(1);
                    q->set_x(p.first);
                    q->set_y(p.second);
                    return event;
                }
                if (x > 200 && x < 480 && y > 500 && y < 700) { // деревня
                    Event event;
                    event.set_type(EventType::ENDTURN);
                    auto q = event.mutable_buildinfo();
                    return event;
                }
            }
        }

        limit.delay();

    }
    Event event;
    event.set_type(EventType::ENDGAME);
    return event;
}

bool Obj::is(int x, int y) const {
    return x <= gui_x2 && x >= gui_x1 && y <= gui_y2 && y >= gui_y1;
}

std::pair<int, int> Obj::get_model_coors() {
    return std::make_pair(model_x, model_y);
}

void GUI::add_road(std::pair<int, int> tmp, int player) {
    std::lock_guard<std::mutex> lock(mutex_for_roads);
    for (auto& e : roads->vec) {
        if (tmp.first == e.model_x && tmp.second == e.model_y) {
            e.built++;
            // TODO: непонятно, зачем ++, можно сделать bool
            // TODO: добавить дорогам метод, который принимает номер игрока и ставит для себя нужную текстуру, или же делать это здесь
            // TODO: можно сделать, чтобы для уже построеных дорог cur_texture был пустым, чтобы они не подсвечивались при наведении
            e.colour = (Colour)player;
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
            dest.x = 475 + 10 + j*50*sqrt(3);
            dest.y = 150 + 40 + 3*i*50;
            dest.w = 80;
            dest.h = 100;
            Obj tmp(X1_V + j * DX / SCALE_X, Y1_V + i * DY,
                    X2_V + j * DX / SCALE_X, Y2_V + i * DY,
                    i * 2 + 1, j * 2,
                    NONE, dest, 0);
            vec.push_back(tmp);
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
            dest.x = 515 + 10  + j*50*sqrt(3);
            dest.y = 130 + 3*i*50;
            dest.w = 50*sqrt(3);
            dest.h = 80;
            int type = 1;
            if ((i + j) % 2 != 0) type = 2;
            Obj tmp(X1_G + j * DX / SCALE_X, Y1_G + i * DY,
                    X2_G + j * DX / SCALE_X, Y2_G + i * DY,
                    i * 2, 	j * 2 + 1,
                    NONE, dest, type);
            vec.push_back(tmp);
//            std::cout << i * 2 << ' ' << j * 2 + 1 << std::endl;
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

void GUI::add_building(std::pair<int, int> tmp, int player) {
    std::lock_guard<std::mutex> lock(mutex_for_roads);
    for (auto& e:buildings->vec) {
        if (tmp.first == e.model_x && tmp.second == e.model_y) {
            e.built++;
            // TODO: непонятно, зачем ++, можно сделать bool
            // TODO: добавить зданиям метод, который принимает номер игрока и ставит для себя нужную текстуру, или же делать это здесь
            //e.texture = build_texture_arr[player];
            //e.cur_texture = cur_build_texture_arr[player];
            // TODO: можно сделать, чтобы для уже построеных зданий cur_texture был пустым, чтобы они не подсвечивались при наведении
            e.colour = (Colour)player;
            if (e.built > 1) e.built = 1;
            return;
        }
    }
}



// void GUI::add_road(std::pair<int, int> tmp, int player) {
//     std::lock_guard<std::mutex> lock(mutex_for_roads);
//     for (auto& e : roads->vec) {
//         if (tmp.first == e.model_x && tmp.second == e.model_y) {
//             e.built++;
//             std::cout << player << std::endl;
//             if (player != 0) {
//                 e.texture = e.texture2;
//             }
//             return;
//         }
//     }
// }

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
            dest.w = 90;
            dest.h = 80;
            Obj tmp(X1 + j * DX / SCALE_X, Y1 + i * DY / SCALE_Y,
                    X2 + j * DX / SCALE_X, Y2 + i * DY / SCALE_Y,
                   	(i + 1)/2,  j*2 , 
                    NONE, dest, 0);
            //tmp.built = true;
            //std::cout << (i + 1)/2 << ' ' << j*2  << '\n';
            vec.push_back(tmp);
        }
    }
}

Building_arr::~Building_arr() = default;


} // namespace GUI

