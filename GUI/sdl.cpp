#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <cassert>
#include <random.h>
#include "sdl.h"

namespace GUI {

namespace {

void Limiter::storeStartTime() {
    frameStart = SDL_GetTicks();
}

void Limiter::delay() {
    frameTime = static_cast<int>(SDL_GetTicks() - frameStart);
    if (frameDelay > frameTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay - frameTime));
    }
}

} // namespace



using ::game::Event;
using ::game::EventType;

void playMusic(GUI* gui) {
    while (!gui->quit.load()) {
        std::cout << "MUSIC" << std::endl;
        Mix_PlayChannel(-1, gui->sfx, 0);
        for (int i = 0; i < 18000; ++i) {
            if (gui->quit.load()) return;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void GUI::loadTextures(utility::Random& random, GUI& gui) {
    auto randomResouresAndNumbers = random.generateResourcesAndNumbers();
    std::string s = "image/oct .bmp";
    
    std::vector<SDL_Texture *> v1;
    std::vector<SDL_Texture *> v2;
    std::vector<SDL_Texture *> v3;

    for (int i = 0; i < 19; ++i) {
        std::string ss = "image/number";
        int resource = randomResouresAndNumbers[i].resource;
        int number = randomResouresAndNumbers[i].number;
        s[9] = resource + '0'; // TODO: Числа в названиях файлов должны соответствовать enum в модели

        ss = ss + std::to_string(number);
        ss = ss + ".bmp";
        SDL_Surface* numberImg = nullptr;

        if (number) numberImg = IMG_Load(ss.c_str()); // TODO: Вместо 2 нужно ставить число number, кроме случая, когда число 0, в этом случае не нужно изображение чилса
        
        std::cerr << ss;
        SDL_Surface* hex = IMG_Load(s.c_str());
        if (number) {
            SDL_Rect dest;
            dest.x = 1000/2 - static_cast<int>(static_cast<double>(numberImg->w) * 0.5) / 2;
            dest.y = 1154/2 - static_cast<int>(static_cast<double>(numberImg->h) * 0.5) / 2;
            dest.w = static_cast<int>(static_cast<double>(numberImg->w) * 0.5);
            dest.h = static_cast<int>(static_cast<double>(numberImg->h) * 0.5);
            if (SDL_BlitScaled(numberImg, nullptr, hex, &dest) != 0) {
                std::cout << "Wrong Blit" << std::endl;
            }
        }   
        if (i < 14) {
            if (i % 2 == 0) v1.push_back(SDL_CreateTextureFromSurface(ren, hex));
            else v2.push_back(SDL_CreateTextureFromSurface(ren, hex));
        } else {
            v3.push_back(SDL_CreateTextureFromSurface(ren, hex));
        }
        SDL_FreeSurface(hex);
    }
    field_arr = v1;
    for (auto e: v3) {
        field_arr.push_back(e);
    }
    for (int i = 4; i < static_cast<int>(v2.size()); ++i) {
        field_arr.push_back(v2[i]);
    }
    for (int i = 0; i < 4; ++i) {
        field_arr.push_back(v2[i]);
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

    svitok = IMG_LoadTexture(ren, "image/svitok.bmp");

    tables_arr.push_back(IMG_LoadTexture(ren, "image/1_tables.bmp"));
    tables_arr.push_back(IMG_LoadTexture(ren, "image/2_tables.bmp"));
    tables_arr.push_back(IMG_LoadTexture(ren, "image/3_tables.bmp"));
    tables_arr.push_back(IMG_LoadTexture(ren, "image/4_tables.bmp"));

    (cur_texture_arr_building).push_back(IMG_LoadTexture(ren, "image/cur_house.bmp"));
    (cur_texture_arr_building).push_back(IMG_LoadTexture(ren, "image/cur_house1.bmp"));
    (cur_texture_arr_building).push_back(nullptr);

    (texture_arr_building[0]).push_back(IMG_LoadTexture(ren, "image/house_yellow.bmp"));
    (texture_arr_building[0]).push_back(IMG_LoadTexture(ren, "image/house_red.bmp"));
    (texture_arr_building[0]).push_back(IMG_LoadTexture(ren, "image/house_green.bmp"));
    (texture_arr_building[0]).push_back(IMG_LoadTexture(ren, "image/house_blue.bmp"));

    (texture_arr_building[1]).push_back(IMG_LoadTexture(ren, "image/house_yellow.bmp"));
    (texture_arr_building[1]).push_back(IMG_LoadTexture(ren, "image/house_red.bmp"));
    (texture_arr_building[1]).push_back(IMG_LoadTexture(ren, "image/house_green.bmp"));
    (texture_arr_building[1]).push_back(IMG_LoadTexture(ren, "image/house_blue.bmp"));

    (texture_arr_building[2]).push_back(IMG_LoadTexture(ren, "image/house1_yellow.bmp"));
    (texture_arr_building[2]).push_back(IMG_LoadTexture(ren, "image/house1_red.bmp"));
    (texture_arr_building[2]).push_back(IMG_LoadTexture(ren, "image/house1_green.bmp"));
    (texture_arr_building[2]).push_back(IMG_LoadTexture(ren, "image/house1_blue.bmp"));

    (texture_arr_road[0]).push_back(IMG_LoadTexture(ren, "image/road_red.bmp"));
    (texture_arr_road[0]).push_back(IMG_LoadTexture(ren, "image/road_yellow.bmp"));
    (texture_arr_road[0]).push_back(IMG_LoadTexture(ren, "image/road_green.bmp"));
    (texture_arr_road[0]).push_back(IMG_LoadTexture(ren, "image/road_blue.bmp"));

    (texture_arr_road[1]).push_back(IMG_LoadTexture(ren, "image/road1_red.bmp"));
    (texture_arr_road[1]).push_back(IMG_LoadTexture(ren, "image/road1_yellow.bmp"));
    (texture_arr_road[1]).push_back(IMG_LoadTexture(ren, "image/road1_green.bmp"));
    (texture_arr_road[1]).push_back(IMG_LoadTexture(ren, "image/road1_blue.bmp"));

    (texture_arr_road[2]).push_back(IMG_LoadTexture(ren, "image/road2_red.bmp"));
    (texture_arr_road[2]).push_back(IMG_LoadTexture(ren, "image/road2_yellow.bmp"));
    (texture_arr_road[2]).push_back(IMG_LoadTexture(ren, "image/road2_green.bmp"));
    (texture_arr_road[2]).push_back(IMG_LoadTexture(ren, "image/road2_blue.bmp"));

    dice.push_back(nullptr);
    dice.push_back(IMG_LoadTexture(ren, "image/dice1.bmp"));
    dice.push_back(IMG_LoadTexture(ren, "image/dice2.bmp"));
    dice.push_back(IMG_LoadTexture(ren, "image/dice3.bmp"));
    dice.push_back(IMG_LoadTexture(ren, "image/dice4.bmp"));
    dice.push_back(IMG_LoadTexture(ren, "image/dice5.bmp"));
    dice.push_back(IMG_LoadTexture(ren, "image/dice6.bmp"));

    dice_shadow = IMG_LoadTexture(ren, "image/dice_shadow.bmp");
    table_shadow.push_back(nullptr);
    table_shadow.push_back(IMG_LoadTexture(ren, "image/table_shadow1.bmp"));
    table_shadow.push_back(IMG_LoadTexture(ren, "image/table_shadow2.bmp"));
    table_shadow.push_back(IMG_LoadTexture(ren, "image/table_shadow3.bmp"));

    players_names.push_back("LOL");
    players_names.push_back("KEK");
    players_names.push_back("ALLAH");
    players_names.push_back("AKBAR");

    svitok_up = nullptr;
    svitok_down = nullptr;
    svitok_up = SDL_LoadBMP("image/svitok_up.bmp");
    svitok_down = SDL_LoadBMP("image/svitok_down.bmp");
    svitok_up1 = SDL_LoadBMP("image/svitok_up.bmp");
    svitok_down1 = SDL_LoadBMP("image/svitok_down.bmp");

    TTF_Font *font = TTF_OpenFont("sample.ttf", 32);
    
    SDL_Surface *surf = TTF_RenderText_Blended(font, "WOOL", color_const_table);
    
    SDL_Rect dest1={150, 200, 0, 0};

    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    surf = TTF_RenderText_Blended(font, "WOOD", color_const_table);
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    surf = TTF_RenderText_Blended(font, "BRICKS", color_const_table);
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    surf = TTF_RenderText_Blended(font, "CORN", color_const_table);
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    surf = TTF_RenderText_Blended(font, "ORE", color_const_table);
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);

    dest1.y = 30;
    for (auto e: players_names) {
        surf = TTF_RenderText_Blended(font, e.c_str(), color_const_table);
        SDL_BlitSurface(surf, nullptr, svitok_down, &dest1);
        dest1.y += 30;
    }

    TTF_CloseFont(font);

    font = TTF_OpenFont("sample.ttf", 32);
    sfx = nullptr;
    sfx = Mix_LoadWAV("image/music.wav");
    Mix_VolumeChunk(sfx, MIX_MAX_VOLUME / 4);
    button_sound = Mix_LoadWAV("image/button_sound.wav");
    Mix_VolumeChunk(button_sound, MIX_MAX_VOLUME / 4);
    build_sound = Mix_LoadWAV("image/build_sound.wav");
    Mix_VolumeChunk(build_sound, MIX_MAX_VOLUME / 12);
    if (sfx == nullptr)  std::cout << "Hhhh";

    _build_road = Inscription(gui, 423 + 161, 317 + 136, "Build Road");
    _build = Inscription(gui, 423 + 161, (373 + 552) / 2 + 373 , "Build");
    _settlement = Inscription(gui, 423 + 161, (373 + 552) / 2 + 552, "Settlement");

    _end_turn = Inscription(gui, 423 + 161, 610 + 790, "End Turn");
    _go_back = Inscription(gui, 423 + 161, 317 + 136, "Go Back");
    _roll_the_dice = Inscription(gui, 423 + 161, 317 + 136, "Roll The Dice");

    _play_a_card = Inscription(gui, 423 + 161, 373 + 552, "Play A Card");
        
    _local_game = Inscription(gui, 1030, 210, "Local Game");
    _game_on_server = Inscription(gui, 1030, 240, "Game On Server");
    _start_new_game = Inscription(gui, 1030, 210, "Start New Game");
    _join_game = Inscription(gui, 1030, 240, "Join Game");
    _exit = Inscription(gui, 1030, 270, "Exit");
    _2_Players = Inscription(gui, 1030, 210, "2 Players");
    _3_Players = Inscription(gui, 1030, 240, "3 Players");
    _4_Players = Inscription(gui, 1030, 270, "4 Players");
    _type_game_id = Inscription(gui, 1030, 210, "Type Game Id");

    std::cerr << "EEEEEEEEEEEEEEEE";
    SDL_BlitSurface(svitok_up, nullptr, svitok_up1, nullptr);
    SDL_BlitSurface(svitok_down, nullptr, svitok_down1, nullptr);

    makeTextureConstTable(resourses, *svitok_up1, texture_svitok_up, 0);
    makeTextureConstTable(players_points, *svitok_down1, texture_svitok_down, 1);
    std::cerr << "EEEEEEEEEEEEEEEE BOY";
    TTF_CloseFont(font);
}

void GUI::destroyTextures() { // TODO: Удалять всё, а не только часть
    for (auto & i : field_arr) {
        SDL_DestroyTexture(i);
    }
    Mix_CloseAudio();
    SDL_DestroyTexture(back_ground);
    SDL_DestroyTexture(back);
}

void GUI::renderBackground() const {
    SDL_Rect dest1;
    dest1.x = 0;
    dest1.y = 0;
    dest1.w = displayMode.w;
    dest1.h = displayMode.h;
    assert(ren != nullptr); // TODO: гонка данных --- ren
    SDL_RenderCopy(ren, back_ground, nullptr, &dest1); //Копируем в рендер фон // TODO: гонка данных --- ren
    if (render_type <= 10)
        SDL_RenderCopy(ren, back, nullptr, &dest1); //Копируем в рендер фон // TODO: гонка данных --- ren
}

void GUI::renderDice() {
    // if (render_type == 10) {
    //     end_time_dice = clock() + 60000;
    // }
    // if (clock() > end_time_dice) return;
    SDL_Rect dest;
    dest.x = 1550;
    dest.y = 650;
    dest.w = 100;
    dest.h = 100;
    if (dice1 && dice2) SDL_RenderCopy(ren, dice_shadow, nullptr, nullptr);
    SDL_RenderCopy(ren, dice[dice1], nullptr, &dest);
    dest.x += 120;
    SDL_RenderCopy(ren, dice[dice2], nullptr, &dest);
}

void GUI::renderTables() const {
    SDL_Rect dest;
    dest.x = -50;
    dest.y = 98;
    dest.w = 540*1.2;
    dest.h = 960*1.2;

    if (render_type == 0)
        SDL_RenderCopy(ren, tables_arr[2], nullptr,&dest);
    if (render_type == 1 || render_type == 2)
        SDL_RenderCopy(ren, tables_arr[0], nullptr,&dest);
    if (render_type == 5)
        SDL_RenderCopy(ren, tables_arr[1], nullptr,&dest);
}


void GUI::renderField() {
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

void GUI::renderRoads() {
    std::lock_guard<std::mutex> lock(mutex_for_roads);
    for (auto e: roads->vec) {
        if (e.built) {
            SDL_RenderCopy(ren, texture_arr_road[e.type][e.colour], nullptr, &e.dest);
        }

    }
    if (render_type == 1) {
        int e = returnRoad(tmp_road.first, tmp_road.second);
        if (e == -1) return;
        SDL_RenderCopy(ren,  cur_texture_arr_road[roads->vec[e].type], nullptr, &roads->vec[e].dest);
    }
}


void GUI::renderBuildings() {
    std::lock_guard<std::mutex> lock(mutex_for_roads);
    for (auto e: buildings->vec) {
        if (e.built) {
            SDL_RenderCopy(ren, texture_arr_building[e.built][e.colour], nullptr, &e.dest);
        }
    }
    if (render_type == 2) {
        int e = returnBuilding(tmp_building.first, tmp_building.second);
        if (e == -1) return;
        SDL_RenderCopy(ren, cur_texture_arr_building[buildings->vec[e].built] , nullptr, &buildings->vec[e].dest);
    }
}

void GUI::renderTablesTime() const {
    SDL_Rect (dest);
    dest.x = 200 + 500;
    dest.y = 98 - 20;
    dest.w = 480 - 200;
    dest.h = 280 - 98;
    if (clock() < cur_table.second) SDL_RenderCopy(ren, cur_table.first, nullptr, &dest);
}





SDL_Texture* GUI::Text(const std::string &message) {   
        TTF_Font *font = TTF_OpenFont("sample.ttf", 32);
        SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
        SDL_FreeSurface(surf);
        TTF_CloseFont(font);
        return texture;
}

Inscription::Inscription(GUI& gui, int _x, int _y, const std::string &s) {
    texture = gui.Text(s);
    int iW, iH;
    SDL_QueryTexture(texture, nullptr, nullptr, &iW, &iH);
    dest.x = (_x - iW)/2;
    dest.y = (_y - iH)/2;
    dest.w = iW;
    dest.h = iH;
}


void GUI::renderText() const {
    if (render_type == 0) {
        SDL_RenderCopy(ren, _build_road.texture, nullptr, &_build_road.dest);
        SDL_RenderCopy(ren, _build.texture, nullptr, &_build.dest);
        SDL_RenderCopy(ren, _settlement.texture, nullptr, &_settlement.dest);
        SDL_RenderCopy(ren, _end_turn.texture, nullptr, &_end_turn.dest);
    }
    if (render_type == 1 || render_type == 2) {
        SDL_RenderCopy(ren, _go_back.texture, nullptr, &_go_back.dest);
    }
    if (render_type == 5) {
        SDL_RenderCopy(ren, _roll_the_dice.texture, nullptr, &_roll_the_dice.dest);
        SDL_RenderCopy(ren, _play_a_card.texture, nullptr, &_play_a_card.dest);
    }
}


void GUI::renderConstTable(){
    std::lock_guard<std::mutex> lock(mutex_for_table);
    SDL_Rect dest;
    dest.x = 1380;
    dest.y = 0;
    dest.w = 540;
    dest.h = 480;
    SDL_RenderCopy(ren, texture_svitok_up, nullptr, &dest);
    dest.y += 240 + 150;
    dest.h += 240 - 150;
    SDL_RenderCopy(ren, texture_svitok_down, nullptr, &dest);
}

void GUI::renderBeginingMenu(){
    SDL_Rect dest;
    dest.x = 1000;
    dest.y = 98;
    dest.w = 540*1.2;
    dest.h = 960*1.2;
    if (render_type == 11) {
        SDL_RenderCopy(ren, tables_arr[1], nullptr,&dest);
        SDL_RenderCopy(ren, _local_game.texture, nullptr, &_local_game.dest);
        SDL_RenderCopy(ren, _game_on_server.texture, nullptr, &_game_on_server.dest);
    } else if (render_type == 12) {
        SDL_RenderCopy(ren, tables_arr[2], nullptr,&dest);
        SDL_RenderCopy(ren, _start_new_game.texture, nullptr, &_start_new_game.dest);
        SDL_RenderCopy(ren, _join_game.texture, nullptr, &_join_game.dest);
        SDL_RenderCopy(ren, _exit.texture, nullptr, &_exit.dest);
    } else if (render_type == 13) {
        SDL_RenderCopy(ren, tables_arr[2], nullptr,&dest);
        SDL_RenderCopy(ren, _2_Players.texture, nullptr, &_2_Players.dest);
        SDL_RenderCopy(ren, _3_Players.texture, nullptr, &_3_Players.dest);
        SDL_RenderCopy(ren, _4_Players.texture, nullptr, &_4_Players.dest);
    } else if (render_type == 14) {
        SDL_RenderCopy(ren, tables_arr[1], nullptr,&dest);
        SDL_RenderCopy(ren, _type_game_id.texture, nullptr, &_type_game_id.dest);
    }
}

void GUI::makeRender() { // TODO: гонка данных --- ren
    SDL_RenderClear(ren);
    renderBackground();
    renderConstTable();
    if (render_type <= 10) {
        renderField();
        renderRoads();
        renderBuildings();
        renderTables();
        renderTablesTime();
        renderDice();
        renderText();
        renderText();
    } else {
        renderBeginingMenu();
    }

    SDL_Rect dest;
    dest.x = 1380;
    dest.y = 0;
    dest.w = 540;
    dest.h = 960;

}



void upgrade(GUI* g) {
    constexpr int FPS = 60;
    constexpr int frameDelay = 1000 / FPS;

    uint32_t frameStart = 0;
    int frameTime = 0;

    while (true) {
        frameStart = SDL_GetTicks();

        g->makeRender();
        SDL_RenderPresent(g->ren);
        if (g->quit.load()) return;

        frameTime = static_cast<int>(SDL_GetTicks() - frameStart);
        if (frameDelay > frameTime) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay - frameTime));
        }
    }
}

GUI::GUI(int player, int numberOfPlayers) : cur_player(player), num_players(numberOfPlayers) {
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Init(SDL_INIT_AUDIO);
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    SDL_GetDesktopDisplayMode(0,&displayMode);
    win = SDL_CreateWindow("Settlers of Catan", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    players_points.resize(num_players, 0);
    resourses = {2, 4, 4, 0, 2};
    //std::cout << "Sync " << SDL_GL_SetSwapInterval(1) << std::endl;
}


void GUI::getCoorsRoad() {
    Mix_PlayChannel(-1, build_sound, 0);
    int old_render_type = render_type;
    render_type = 1;
    //SDL_Rect dest;
    SDL_Event e;
    clock_t begin_time = clock();

    Limiter limit;

    while (!quit.load()) {

        limit.storeStartTime(); // засекает начало итерации

        clock_t end_time = clock();
        if (end_time - begin_time > CLOCKS_PER_SEC * 30) {
            cur_table = std::make_pair(table_time, clock() + 5*CLOCKS_PER_SEC);
            render_type = old_render_type;
            return;
        }
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit.store(true);
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit.store(true);
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
                tmp_coors = returnRoad(x, y);
                std::cout << x << ' ' << y << '\n';
                if (tmp_coors != -1) return;
            }
        }

        limit.delay(); // Ждет, если операции выполнились слишком быстро

    }
}

void GUI::getCoorsBuilding () {
    Mix_PlayChannel(-1, build_sound, 0);
    int old_render_type = render_type;
    render_type = 2;
    //SDL_Rect dest;
    SDL_Event e;
    clock_t begin_time = clock();

    Limiter limit;

    while (!quit.load()) {

        limit.storeStartTime();

        clock_t end_time = clock();
        if (end_time - begin_time > CLOCKS_PER_SEC * 30) {
            cur_table = std::make_pair(table_time, clock() + 5*CLOCKS_PER_SEC);
            render_type = old_render_type;
            return;
        }
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit.store(true);
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit.store(true);
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
                tmp_coors = returnBuilding(x, y);
                if (tmp_coors != -1) return;
            }
        }

        limit.delay();

    }
}

Event GUI::ThirdStage () {
    SDL_Event e;
    render_type = 0;

    Limiter limit;

    while (!quit.load()) {

        limit.storeStartTime();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit.store(true);
                Event event;
                event.set_type(EventType::ENDGAME);
                return event;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit.store(true);
                Event event;
                event.set_type(EventType::ENDGAME);
                return event;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y);
                std::cout << x << ' ' << y << '\n';
                if (x > 161 && x < 423 && y > 136 && y < 317) { // дорога
                    getCoorsRoad();
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
                if (x > 161 && x < 423 && y > 373 && y < 552) { // derevnia
                    getCoorsBuilding();
                    if (render_type == 0) continue;
                    auto p = buildings->vec[tmp_coors].get_model_coors();
                    Event event;
                    event.set_type(EventType::BUILD);
                    auto q = event.mutable_buildinfo();
                    q->set_buildingtype(buildings->vec[tmp_coors].built + 1);
                    q->set_x(p.first);
                    q->set_y(p.second);
                    return event;
                }
                if (x > 161 && x < 423 && y > 610 && y < 790) { // деревня
                    Event event;
                    event.set_type(EventType::ENDTURN);
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

::game::Event GUI::FirstStage() {
    render_type = 5;
    SDL_Event e;

    Limiter limit;
    Event event;

    while(true){
        limit.storeStartTime();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > 200 && x < 480 && y > 98 && y < 280) {
                    event.set_type(EventType::DICE);
                    auto nums = event.mutable_diceinfo();
                    nums->set_number1(0);
                    nums->set_number1(0);
                    return event;
                }
                if (x > 200 && x < 480 && y > 300 && y < 482) {
                    event.set_type(EventType::CARD); // TODO: нужно сразу здесь вызвать функцию, которая получит вид карты
                    return event;
                }
            }
        }
        limit.delay();
    }
}

void GUI::addDice(int x, int y) {
    dice1 = x;
    dice2 = y;
    render_type = 10;
}

bool Obj::is(int x, int y) const {
    return x <= gui_x2 && x >= gui_x1 && y <= gui_y2 && y >= gui_y1;
}

std::pair<int, int> Obj::get_model_coors() {
    return std::make_pair(model_x, model_y);
}

void GUI::addRoad(std::pair<int, int> tmp, int player) {
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

int GUI::returnRoad(int x, int y) const {
    for (int i = 0; i < static_cast<int>(roads->vec.size()); ++i)
        if (roads->vec[i].is(x, y)) return i;
    return -1;
}

int GUI::returnBuilding(int x, int y) const {
    for (int i = 0; i < static_cast<int>(buildings->vec.size()); ++i)
        if (buildings->vec[i].is(x, y)) return i;
    return -1;
}

Road_arr::Road_arr() {
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
    destroyTextures();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void GUI::addBuilding(std::pair<int, int> tmp, int player) {
    std::lock_guard<std::mutex> lock(mutex_for_roads);
    for (auto& e:buildings->vec) {
        if (tmp.first == e.model_x && tmp.second == e.model_y) {
            std::cerr << "kek"; 
            e.built++;
            // TODO: непонятно, зачем ++, можно сделать bool    
            // TODO: добавить зданиям метод, который принимает номер игрока и ставит для себя нужную текстуру, или же делать это здесь
            //e.texture = build_texture_arr[player];
            //e.cur_texture = cur_build_texture_arr[player];
            // TODO: можно сделать, чтобы для уже построеных зданий cur_texture был пустым, чтобы они не подсвечивались при наведении
            e.colour = (Colour)player;
            if (e.built > 2) e.built = 2;
            return;
        }
    }
    std::cerr << "kek"; 
}

Building_arr::Building_arr() {
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
            dest.x = 510 - 30 - 3 + j * DX / SCALE_X;
            dest.y = 135 - 40 - 20 + i * DY / SCALE_Y;
            dest.w = 100;
            dest.h = 110;
            int t = (i+1)/2;
            if (t == 1) t = 0;
            if (t == 3) t = 4;
            if (t > 4) t+=2;
            if (t == 7) t--;
            if (t == 9) t--;
            Obj tmp(X1 + j * DX / SCALE_X, Y1 + i * DY / SCALE_Y,
                    X2 + j * DX / SCALE_X, Y2 + i * DY / SCALE_Y,
                   	t,  j*2 , 
                    NONE, dest, 0);
            //tmp.built = true;
            std::cout << t << ' ' << j*2  << '\n';
            vec.push_back(tmp);
        }
    }
}

Building_arr::~Building_arr() = default;

void GUI::updatePlayer(int x) {
    cur_player = x;
}

void GUI::updatePoints(std::vector<int> vec) {
    std::lock_guard<std::mutex> lock(mutex_for_table);
    players_points = vec;
    svitok_down = nullptr;
    svitok_down = SDL_LoadBMP("image/svitok_down.bmp");

    TTF_Font *font = TTF_OpenFont("sample.ttf", 32);

    SDL_Rect dest1={150, 200, 0, 0};
    SDL_Surface *surf = nullptr;

    dest1.y = 30;
    for (auto e: players_names) {
        surf = TTF_RenderText_Blended(font, e.c_str(), color_const_table);
        SDL_BlitSurface(surf, nullptr, svitok_down, &dest1);
        dest1.y += 30;
    }

    TTF_CloseFont(font);

    makeTextureConstTable(players_points, *svitok_down, texture_svitok_down, 1);
}

void GUI::updateResourses(std::vector<int> v) {
    std::lock_guard<std::mutex> lock(mutex_for_table);
    resourses = v;

    svitok_up = nullptr;
    svitok_up = SDL_LoadBMP("image/svitok_up.bmp");

    TTF_Font *font = TTF_OpenFont("sample.ttf", 32);
    SDL_Surface *surf = TTF_RenderText_Blended(font, "WOOL", color_const_table);
    SDL_Rect dest1={150, 200, 0, 0};

    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    surf = TTF_RenderText_Blended(font, "WOOD", color_const_table);
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    surf = TTF_RenderText_Blended(font, "BRICKS", color_const_table);
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    surf = TTF_RenderText_Blended(font, "CORN", color_const_table);
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    surf = TTF_RenderText_Blended(font, "ORE", color_const_table);
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    TTF_CloseFont(font);

    makeTextureConstTable(resourses, *svitok_up, texture_svitok_up, 0);
}

void GUI::makeTextureConstTable(std::vector<int> vec, SDL_Surface& x, SDL_Texture *&ans, int type) {
    TTF_Font *font = TTF_OpenFont("sample.ttf", 32);
    SDL_Surface * buff = new SDL_Surface(x);
    SDL_Rect dest;
    dest.x = 280 + 90;
    dest.y = 200;
    dest.h = 100;
    dest.w = 100;
    if (type) dest.y = 30;
    for (auto e: vec) {
        SDL_Surface *surf = TTF_RenderText_Blended(font, std::to_string(e).c_str(), color_const_table);
        SDL_BlitSurface(surf, nullptr, buff, &dest);
        dest.y += 30;
    }
    ans = nullptr;
    ans = SDL_CreateTextureFromSurface(ren, buff);
    if (ans == nullptr) std::cerr << "PIZDA";
    TTF_CloseFont(font);
}

void GUI::addPlayerName(int x, std::string s) {
    players_names[x] = s;
}

int GUI::getPlaceOfGame() {
    render_type = 11;
    SDL_Event e;
    Limiter limit;
    while (!quit.load()) {
        limit.storeStartTime();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > 1161 && x < 1423 && y > 136 && y < 317) { // дорога
                    return 1;
                }
                if (x > 1161 && x < 1423 && y > 373 && y < 552) { // derevnia
                    return 2;
                }
            }
        }
        limit.delay();
    }
    return 1;
}

int GUI::getTypeOfGame() {
    render_type = 12;
    SDL_Event e;
    Limiter limit;
    while (!quit.load()) {
        limit.storeStartTime();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > 1161 && x < 1423 && y > 136 && y < 317) { // дорога
                    return 1;
                }
                if (x > 1161 && x < 1423 && y > 373 && y < 552) { // derevnia
                    return 2;
                }
                if (x > 1161 && x < 1423 && y > 610 && y < 790) { // деревня
                    return 3;
                }
            }
        }
        limit.delay();
    }
    return 1;
}

int GUI::getNumOfPlayers() {
    render_type = 13;
    SDL_Event e;
    Limiter limit;
    while (!quit.load()) {
        limit.storeStartTime();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > 1161 && x < 1423 && y > 136 && y < 317) { // дорога
                    return 1;
                }
                if (x > 1161 && x < 1423 && y > 373 && y < 552) { // derevnia
                    return 2;
                }
                if (x > 1161 && x < 1423 && y > 610 && y < 790) { // деревня
                    return 3;
                }
            }
        }
        limit.delay();
    }
    return 1;
}

int GUI::getGameId() {
    render_type = 14;
    int value = 0;
    SDL_Event e;
    Limiter limit;
    while (!quit.load()) {
        limit.storeStartTime();
        while (SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_0) {
                    value*=10;
                    value+=0;
                } else if (e.key.keysym.sym == SDLK_1) {
                    value*=10;
                    value+=1;
                } else if (e.key.keysym.sym == SDLK_2) {
                    value*=10;
                    value+=2;
                } else if (e.key.keysym.sym == SDLK_3) {
                    value*=10;
                    value+=3;
                } else if (e.key.keysym.sym == SDLK_4) {
                    value*=10;
                    value+=4;
                } else if (e.key.keysym.sym == SDLK_5) {
                    value*=10;
                    value+=5;
                } else if (e.key.keysym.sym == SDLK_6) {
                    value*=10;
                    value+=6;
                } else if (e.key.keysym.sym == SDLK_7) {
                    value*=10;
                    value+=0;
                } else if (e.key.keysym.sym == SDLK_8) {
                    value*=10;
                    value+=8;
                } else if (e.key.keysym.sym == SDLK_9) {
                    value*=10;
                    value+=9;
                } else if (e.key.keysym.sym == SDLK_KP_ENTER) {
                    return value;
                }
            }
        }
        limit.delay();
    }
    return 0;
}

::game::Event GUI::getEvent() {
    static int gameStage = 0;
    Event event;
    if (gameStage == 0) {
        event = FirstStage();
        if (event.type() == EventType::DICE) ++gameStage;
        return event;
//    } else if (gameStage == 1) {

    } else {
        event = ThirdStage();
        if (event.type() == EventType::ENDTURN) gameStage = 0;
        return event;
    }
}

} // namespace GUI