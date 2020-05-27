#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <cassert>
#include <stdexcept>
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
        Mix_PlayChannel(-1, gui->sfx, 1000);
        for (int i = 0; i < 18000; ++i) {
            if (gui->quit.load()) return;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void Image::render(SDL_Renderer *ren) {
    SDL_RenderCopy(ren, texture, nullptr, &dest);
}

void Image::render(SDL_Renderer *ren, SDL_Rect _dest) {
    dest = _dest;
    render(ren);
}

Image::Image(std::string s, SDL_Renderer *ren){
    texture = IMG_LoadTexture(ren, s.c_str());
}

Image::Image(std::string s, int x, int y, int w, int h, SDL_Renderer *ren) {
    texture = IMG_LoadTexture(ren, s.c_str());
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
}

Image::Image(std::string s, SDL_Rect _dest, SDL_Renderer *ren) {
    texture = IMG_LoadTexture(ren, s.c_str());
    dest = _dest;
}

Image::~Image() {
    SDL_DestroyTexture(texture);
}

void GUI::loadTextures(utility::Random& random, GUI& gui) {
    auto randomResouresAndNumbers = random.generateResourcesAndNumbers();
    std::string s = "image/oct .bmp";
    
    std::vector<SDL_Texture *> v1;

    std::vector<int> sameOrderWithModel = {0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 7, 9, 11, 13, 1, 3, 5};
    for (int k = 0; k < 19; ++k) {
        int i = sameOrderWithModel[k];
        std::string ss = "image/number";
        int resource = randomResouresAndNumbers[i].resource;
        int number = randomResouresAndNumbers[i].number;
        s[9] = resource + '0'; // TODO: Числа в названиях файлов должны соответствовать enum в модели

        ss = ss + std::to_string(number);
        ss = ss + ".bmp";
        SDL_Surface* numberImg = nullptr;

        if (number) numberImg = IMG_Load(ss.c_str()); // TODO: Вместо 2 нужно ставить число number, кроме случая, когда число 0, в этом случае не нужно изображение чилса
        
        //std::cerr << ss;
        SDL_Surface* hex = IMG_Load(s.c_str());
        if (number) {
            SDL_Rect dest;
            dest.x = 1000/2 - static_cast<int>(static_cast<double>(numberImg->w) * 0.5) / 2;
            dest.y = 1154/2 - static_cast<int>(static_cast<double>(numberImg->h) * 0.5) / 2;
            dest.w = static_cast<int>(static_cast<double>(numberImg->w) * 0.5);
            dest.h = static_cast<int>(static_cast<double>(numberImg->h) * 0.5);
            if (SDL_BlitScaled(numberImg, nullptr, hex, &dest) != 0) {
               // std::cout << "Wrong Blit" << std::endl;
            }
        } else {
            robber->x_r.store(robber->vec[k].first + 50);
            robber->y_r.store(robber->vec[k].second + 50);
        }
        field_arr.push_back(SDL_CreateTextureFromSurface(ren, hex));
        SDL_FreeSurface(numberImg);
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

    svitok = IMG_LoadTexture(ren, "image/svitok.bmp");

    tables_arr.push_back(IMG_LoadTexture(ren, "image/1_tables.bmp"));
    tables_arr.push_back(IMG_LoadTexture(ren, "image/2_tables.bmp"));
    tables_arr.push_back(IMG_LoadTexture(ren, "image/3_tables.bmp"));
    tables_arr.push_back(IMG_LoadTexture(ren, "image/4_tables.bmp"));
    back_resourse = IMG_LoadTexture(ren, "image/back_resourse.bmp");

    texture_arr_resourses.push_back(IMG_LoadTexture(ren, "image/sheep.bmp"));
    texture_arr_resourses.push_back(IMG_LoadTexture(ren, "image/stone.bmp"));
    texture_arr_resourses.push_back(IMG_LoadTexture(ren, "image/clay.bmp"));
    texture_arr_resourses.push_back(IMG_LoadTexture(ren, "image/wood.bmp"));
    texture_arr_resourses.push_back(IMG_LoadTexture(ren, "image/wheat.bmp"));

    cur_texture_resourse = IMG_LoadTexture(ren, "image/cur_resourse.bmp");
    texture_resourse_built = IMG_LoadTexture(ren, "image/built_resourse.bmp");

    texture_arr_card.push_back(IMG_LoadTexture(ren, "image/card1.bmp"));
    texture_arr_card.push_back(IMG_LoadTexture(ren, "image/card2.bmp"));
    texture_arr_card.push_back(IMG_LoadTexture(ren, "image/card3.bmp"));
    texture_arr_card.push_back(IMG_LoadTexture(ren, "image/card4.bmp"));
    texture_arr_card.push_back(IMG_LoadTexture(ren, "image/card5.bmp"));




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

    cur_card_texture = IMG_LoadTexture(ren, "image/cur_card.bmp");

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

    players_names.push_back("Player 1");
    players_names.push_back("Player 2");
    players_names.push_back("Player 3");
    players_names.push_back("Player 4");

    players_names.resize(num_players);


    auto font = TTF_OpenFont("image/sample.ttf", 32);
    sfx = nullptr;
    sfx = Mix_LoadWAV("image/music.wav");
    Mix_VolumeChunk(sfx, MIX_MAX_VOLUME / 4);
    button_sound = Mix_LoadWAV("image/button_sound.wav");
    Mix_VolumeChunk(button_sound, MIX_MAX_VOLUME / 4);
    build_sound = Mix_LoadWAV("image/build_sound.wav");
    Mix_VolumeChunk(build_sound, MIX_MAX_VOLUME / 12);
    if (sfx == nullptr)  std::cout << "Hhhh";
    dice_sound = Mix_LoadWAV("image/dice_sound.wav");
    Mix_VolumeChunk(dice_sound, MIX_MAX_VOLUME * 20);

    _build_road = Inscription(gui, 423 + 161, 317 + 136 - 2 * 48, "Build Road");
    _build = Inscription(gui, 423 + 161, (373 + 552) / 2 + 373 - 3*48/2, "Build");
    _settlement = Inscription(gui, 423 + 161, (373 + 552) / 2 + 552 - 3*48/2, "Settlement");
    _next_phase = Inscription(gui, 423 + 161, 610 + 790 - 2 * 48, "Next Phase");
    _buy_card = Inscription(gui, 423 + 161, (373 + 552) - 2*48, "Buy Card");
    _exchange = Inscription(gui, 423 + 161, (317 + 136) / 2  + 136 - 3*48/2, "Exchange");
    _resources = Inscription(gui, 423 + 161, (317 + 136) / 2 + 317 - 3*48/2, "Resources");
    _go_back3 = Inscription(gui, 839 + 1121, 698 + 896 , "Go Back");

    _end_turn = Inscription(gui, 423 + 161, 610 + 790 - 2 * 48, "End Turn");
    //_end_turn = Inscription(gui, 423 + 161, 846 + 1020 - 2 * 48, "End Turn");
    _go_back = Inscription(gui, 423 + 161, 317 + 136 - 2 * 48, "Go Back");
    _go_back2 = Inscription(gui, 423 + 161, 373 + 552 - 2 * 48, "Go Back");
    _ok = Inscription(gui, 423 + 161, 317 + 136 - 2 * 48, "OK");
    _roll_the_dice = Inscription(gui, 423 + 161, 317 + 136 - 2 * 48, "Roll The Dice");

    _play_a_card = Inscription(gui, 423 + 161, 373 + 552 - 2 * 48, "Play A Card");
        
    _local_game = Inscription(gui, 1030, 210, "Local Game");
    _game_on_server = Inscription(gui, 1030, 240, "Game On Server");
    _start_new_game = Inscription(gui, 1030, 210, "Start New Game");
    _join_game = Inscription(gui, 1030, 240, "Join Game");
    _exit = Inscription(gui, 1030, 270, "Exit");
    _2_Players = Inscription(gui, 1030, 210, "2 Players");
    _3_Players = Inscription(gui, 1030, 240, "3 Players");
    _4_Players = Inscription(gui, 1030, 270, "4 Players");
    _type_game_id = Inscription(gui, 1030, 210, "Type Game Id");
    
    SDL_Surface *surf = TTF_RenderText_Blended(font, "You Cant Build Here", color_const_table);
    message.push_back(SDL_CreateTextureFromSurface(ren, surf));
    surf = TTF_RenderText_Blended(font, "You Dont Have This Card", color_const_table);
    message.push_back(SDL_CreateTextureFromSurface(ren, surf));
    surf = TTF_RenderText_Blended(font, "Not Enough Resources", color_const_table);
    message.push_back(SDL_CreateTextureFromSurface(ren, surf));
    surf = TTF_RenderText_Blended(font, "Build road and village", color_const_table);
    message.push_back(SDL_CreateTextureFromSurface(ren, surf));
    SDL_FreeSurface(surf);

    updatePoints(players_points);
    updateResourses(resourses);
    //std::cerr << "EEEEEEEEEEEEEEEE BOY";
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
    assert(ren != nullptr);
    SDL_RenderCopy(ren, back_ground, nullptr, &dest1);
    //if (render_type.load() <= 10)
    SDL_RenderCopy(ren, back, nullptr, &dest1);
}

void GUI::renderCards() {
    if (render_type.load() != 312) return;
    SDL_RenderCopy(ren, back_resourse, nullptr, nullptr);
    SDL_Rect dest;
    dest.x = 150-350;
    dest.y = 50;
    dest.h = 700;
    dest.w = 300;
    for (int i = 0; i < static_cast<int>(texture_arr_card.size()); ++i) {
        dest.x+=350;
        if (!dev_cards_vec[i]) continue;
        auto e = texture_arr_card[i];
        SDL_RenderCopy(ren, e, nullptr, &dest);
        if (i == tmp_card) SDL_RenderCopy(ren, cur_card_texture, nullptr, &dest);
    }
    

}

void GUI::renderDice() {
    // if (render_type.load() == 10) {
    //     end_time_dice = clock() + 60000;
    // }
    // if (clock() > end_time_dice) return;
    SDL_Rect dest;
    dest.x = 1550;
    dest.y = 650;
    dest.w = 100;
    dest.h = 100;
    if (dice1.load() && dice2.load()) SDL_RenderCopy(ren, dice_shadow, nullptr, nullptr);
    SDL_RenderCopy(ren, dice[dice1.load()], nullptr, &dest);
    dest.x += 120;
    SDL_RenderCopy(ren, dice[dice2.load()], nullptr, &dest);
}

void GUI::renderTables() const {
    SDL_Rect dest;
    dest.x = -50;
    dest.y = 50;
    dest.w = 540*1.2;
    dest.h = 960*1.2;

    if (render_type.load() == 0 || render_type.load() == 8)
        SDL_RenderCopy(ren, tables_arr[2], nullptr,&dest);
    if (render_type.load() == 1 || render_type.load() == 2 || render_type.load() == 37)
        SDL_RenderCopy(ren, tables_arr[0], nullptr,&dest);
    if (render_type.load() == 5 || render_type.load() == 6 || render_type.load() == 67)
        SDL_RenderCopy(ren, tables_arr[1], nullptr,&dest);
    if (render_type.load() == 312) {
        dest.x = displayMode.w / 2 - dest.w / 2;
        dest.y = 670;
        SDL_RenderCopy(ren, tables_arr[0], nullptr,&dest);
    }
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
    {
        for (auto& e: roads->vec) {
            if (e.built) {
                SDL_RenderCopy(ren, texture_arr_road[e.type][e.colour], nullptr, &e.dest);
            }

        }
    }
    if (render_type.load() == 1) {
        int e = returnRoad(tmp_road.first.load(), tmp_road.second.load());
        if (e == -1) return;
        // TODO: можно проверить, что дорога ещё не стоит, тогда уже построенные дороги не будут подсвечиваться
        SDL_RenderCopy(ren,  cur_texture_arr_road[roads->vec[e].type], nullptr, &roads->vec[e].dest);
    }
}


void GUI::renderBuildings() {
    {
        for (auto& e: buildings->vec) {
            if (e.built) {
                SDL_RenderCopy(ren, texture_arr_building[e.built][e.colour], nullptr, &e.dest);
            }
        }
    }
    if (render_type.load() == 2) {
        int e = returnBuilding(tmp_building.first.load(), tmp_building.second.load());
        if (e == -1) return;
        // TODO: можно проверить, что здание ещё не стоит
        SDL_RenderCopy(ren, cur_texture_arr_building[buildings->vec[e].built] , nullptr, &buildings->vec[e].dest);
    }
}

void GUI::renderCurPlayer(){
    if (my_player == cur_player) {
        TTF_Font *font = TTF_OpenFont("image/sample.ttf", 64);
        SDL_Color cur_color = { 200, 0, 0, 255 };
        SDL_Surface *surf = TTF_RenderText_Blended(font, "Your Turn", cur_color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
        SDL_FreeSurface(surf);
        TTF_CloseFont(font);
        SDL_Rect dest;
        SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);
        dest.x = 1488;
        dest.y = 55;
        SDL_RenderCopy(ren, texture, nullptr, &dest);
    } else render_type = 92;
}

void GUI::setTable(int i) {
    table_time_type.store(i);
}   

void setTimeTable(GUI *gui) {
    int k = 0;
    int delta = 0;
    while(true) {
        if (gui->quit.load()) return;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (gui->table_time_type.load() != -1 && delta == 0) {
            delta = 1;
            k = 4;
        }
        k-=delta;
        if (k < 0)  {
            gui->table_time_type.store(-1);
            delta = 0;
        }
        
    }
}

void GUI::renderTablesTime() const {
    if (table_time_type.load() == -1) return;
    SDL_Rect dest;
    dest.x = displayMode.w/2 - (1920.0 * 0.75)/2.0;
    dest.y = 0;
    dest.w = 1920.0 * 0.75;
    dest.h = 200.0 * 0.75;
    SDL_RenderCopy(ren, table_time, nullptr, &dest);
    int iW, iH;
    SDL_QueryTexture(message[table_time_type.load()], nullptr, nullptr, &iW, &iH);
    dest.x = (2 * dest.x + dest.w - iW)/2;
    dest.y = (dest.h - iH)/2;
    dest.w = iW;
    dest.h = iH;
    SDL_RenderCopy(ren, message[table_time_type.load()], nullptr, &dest);
}

int GUI::returnResourses(int x, int y) const {
    
    for (int i = 0; i < static_cast<int>(buildings->vec.size()); ++i)
        if (resourses_img->vec[i].is(x, y)) return (render_type.load() != 67) || (i < 5) ? i : -1;
    return -1;
}

void GUI::renderResourses() const {
    if (!(render_type.load() == 6 || render_type.load() == 67)) return;
    SDL_RenderCopy(ren, back_resourse, nullptr, nullptr);
    int e = returnResourses(tmp_resours.first.load(), tmp_resours.second.load());
    if (e != -1) SDL_RenderCopy(ren, cur_texture_resourse , nullptr, &resourses_img->vec[e].dest);
    int it = 0;
    for (auto& t: resourses_img->vec) {
        if (render_type.load() == 67 && it > 4) break;
       if (t.built) {
            SDL_RenderCopy(ren, texture_resourse_built, nullptr, &t.dest);
            // TTF_Font *font = TTF_OpenFont("image/sample.ttf", 128);
            // SDL_Surface *surf = TTF_RenderText_Blended(font, std::to_string(t.built).c_str(), color);
            // SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
            // SDL_FreeSurface(surf);
            // TTF_CloseFont(font);
            // SDL_Rect dest;
            // SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);
            // if (it < 5) {
            //     dest.x = t.dest.x + 170;
            //     dest.x+= (170 - dest.w)/2;
            // } else {
            //     dest.x = t.dest.x - 170;
            //     dest.x+= (170 - dest.w)/2;
            // }

            // dest.y = t.dest.y;
            // dest.y+= (170 - dest.h)/2;
            // SDL_RenderCopy(ren, texture, nullptr, &dest);
        }

         SDL_RenderCopy(ren, texture_arr_resourses[t.type], nullptr, &t.dest);
         it ++;

    }
}


SDL_Texture* GUI::Text(const std::string &message) {
    TTF_Font *font = TTF_OpenFont("image/sample.ttf", 32);
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
    if (render_type.load() == 0) {
        SDL_RenderCopy(ren, _build_road.texture, nullptr, &_build_road.dest);
        SDL_RenderCopy(ren, _build.texture, nullptr, &_build.dest);
        SDL_RenderCopy(ren, _settlement.texture, nullptr, &_settlement.dest);
        SDL_RenderCopy(ren, _end_turn.texture, nullptr, &_end_turn.dest);
    }
    if (render_type.load() == 1 || render_type.load() == 2) {
        SDL_RenderCopy(ren, _go_back.texture, nullptr, &_go_back.dest);
    }
    if (render_type.load() == 5) {
        SDL_RenderCopy(ren, _roll_the_dice.texture, nullptr, &_roll_the_dice.dest);
        SDL_RenderCopy(ren, _play_a_card.texture, nullptr, &_play_a_card.dest);
    }
    if (render_type.load() == 6 || render_type.load() == 67) {
        SDL_RenderCopy(ren, _ok.texture, nullptr, &_ok.dest);
        SDL_RenderCopy(ren, _go_back2.texture, nullptr, &_go_back2.dest);
    }
    if (render_type.load() == 8) {
        SDL_RenderCopy(ren, _exchange.texture, nullptr, &_exchange.dest);
        SDL_RenderCopy(ren, _resources.texture, nullptr, &_resources.dest);
        SDL_RenderCopy(ren, _next_phase.texture, nullptr, &_next_phase.dest);
        SDL_RenderCopy(ren, _buy_card.texture, nullptr, &_buy_card.dest);
    }
    if (render_type.load() == 37) {
        SDL_RenderCopy(ren, _ok.texture, nullptr, &_ok.dest);
    }
    if (render_type.load() == 312) {
        SDL_RenderCopy(ren, _go_back3.texture, nullptr, &_go_back3.dest);
    }

}


void GUI::renderConstTable() {
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
    if (render_type.load() == 11) {
        SDL_RenderCopy(ren, tables_arr[1], nullptr,&dest);
        SDL_RenderCopy(ren, _local_game.texture, nullptr, &_local_game.dest);
        SDL_RenderCopy(ren, _game_on_server.texture, nullptr, &_game_on_server.dest);
    } else if (render_type.load() == 12) {
        SDL_RenderCopy(ren, tables_arr[2], nullptr,&dest);
        SDL_RenderCopy(ren, _start_new_game.texture, nullptr, &_start_new_game.dest);
        SDL_RenderCopy(ren, _join_game.texture, nullptr, &_join_game.dest);
        SDL_RenderCopy(ren, _exit.texture, nullptr, &_exit.dest);
    } else if (render_type.load() == 13) {
        SDL_RenderCopy(ren, tables_arr[2], nullptr,&dest);
        SDL_RenderCopy(ren, _2_Players.texture, nullptr, &_2_Players.dest);
        SDL_RenderCopy(ren, _3_Players.texture, nullptr, &_3_Players.dest);
        SDL_RenderCopy(ren, _4_Players.texture, nullptr, &_4_Players.dest);
    } else if (render_type.load() == 14) {
        SDL_RenderCopy(ren, tables_arr[1], nullptr,&dest);
        SDL_RenderCopy(ren, _type_game_id.texture, nullptr, &_type_game_id.dest);
    }
}

void GUI::makeRender(GUI &gui) {
    std::lock_guard<utility::spinlock> lock(mutex_for_render);
    SDL_RenderClear(ren);
    renderBackground();
    renderConstTable();
    renderField();
    renderRoads();
    renderBuildings();
    renderDice();
    robber->render(gui);
    renderCurPlayer();
    renderResourses();
    renderCards();
    if (render_type.load() != 92) renderTables();
    renderTablesTime();
    if (render_type.load() != 92) renderText();
    //renderBeginingMenu();
    SDL_RenderPresent(ren);
}



void upgrade(GUI* g) {
    constexpr int FPS = 30;
    constexpr int frameDelay = 1000 / FPS;

    uint32_t frameStart = 0;
    int frameTime = 0;

    while (true) {
        frameStart = SDL_GetTicks();

        g->makeRender(*g);

        if (g->quit.load()) return;
         frameTime = static_cast<int>(SDL_GetTicks() - frameStart);
         if (frameDelay - frameTime >= 20) {
             std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay - frameTime));
         } else {
             std::this_thread::sleep_for(std::chrono::milliseconds(20));
         }
    }
}

GUI::GUI(int player, int numberOfPlayers) :num_players(numberOfPlayers), my_player(player) {
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Init(SDL_INIT_AUDIO);
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    SDL_GetDesktopDisplayMode(0,&displayMode);
    win = SDL_CreateWindow("Settlers of Catan", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_FULLSCREEN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetWindowIcon(win, SDL_LoadBMP("image/robber.bmp"));
    players_points.resize(num_players, 0);
    resourses = {2, 0, 4, 4, 2};
    table_time_type.store(-1);
    auto back = IMG_LoadTexture(ren, "image/standby.bmp");
    SDL_RenderCopy(ren, back, nullptr, nullptr);
    SDL_RenderPresent(ren);
}


void GUI::getCoorsRoad() {
    Mix_PlayChannel(-1, build_sound, 0);
    int old_render_type = render_type.load();
    render_type.store(1);
    //SDL_Rect dest;
    SDL_Event e;

    Limiter limit;

    while (!quit.load()) {

        limit.storeStartTime(); // засекает начало итерации
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
                tmp_road.first.store(x);
                tmp_road.second.store(y);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                if (x > 200 && x < 480 && y > 98- 48 && y < 280- 48) {
                    render_type.store(old_render_type);
                    return;
                }
                tmp_coors = returnRoad(x, y);
                //std::cout << x << ' ' << y << '\n';
                if (tmp_coors != -1) return;
            }
        }

        limit.delay(); // Ждет, если операции выполнились слишком быстро

    }
}

void GUI::getCoorsBuilding () {
    Mix_PlayChannel(-1, build_sound, 0);
    int old_render_type = render_type.load();
    render_type.store(2);
    //SDL_Rect dest;
    SDL_Event e;

    Limiter limit;

    while (!quit.load()) {

        limit.storeStartTime();

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
                tmp_building.first.store(x);
                tmp_building.second.store(y);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                if (x > 200 && x < 480 && y > 98 - 48 && y < 280 - 48) {
                    render_type.store(old_render_type);
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
    render_type.store(0);

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
                if (x > 161 && x < 423 && y > 136- 48  && y < 317- 48) { // дорога
                    getCoorsRoad();
                    if (render_type.load() == 0) continue;
                    //std::lock_guard<utility::spinlock> lock(mutex_for_render);
                    auto p = roads->vec[tmp_coors].get_model_coors();
                    Event event;
                    event.set_type(EventType::BUILD);
                    auto q = event.mutable_buildinfo();
                    q->set_buildingtype(3);
                    q->set_x(p.first);
                    q->set_y(p.second);
                    return event;
                }
                if (x > 161 && x < 423 && y > 373- 48 && y < 552- 48) { // derevnia
                    getCoorsBuilding();
                    if (render_type.load() == 0) continue;
                    //std::lock_guard<utility::spinlock> lock(mutex_for_render);
                    auto p = buildings->vec[tmp_coors].get_model_coors();
                    Event event;
                    event.set_type(EventType::BUILD);
                    auto q = event.mutable_buildinfo();
                    q->set_buildingtype(buildings->vec[tmp_coors].built + 1);
                    q->set_x(p.first);
                    q->set_y(p.second);
                    return event;
                }
                if (x > 161 && x < 423 && y > 610- 48 && y < 790- 48) { // деревня
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



Event GUI::SecondStage () {
    SDL_Event e;
    render_type.store(8);
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
                //std::cout << x << ' ' << y << '\n';
                if (x > 161 && x < 423 && y > 136- 48  && y < 317- 48) { // дорога
                    getCoorsResourses();
                    if (render_type.load() == 0) continue;
                    Event event;
                    event.set_type(EventType::MARKET);
                    auto q = event.mutable_marketinfo();
                    q->set_ownedresource(tmp_resourses.first);
                    q->set_requiredresource(tmp_resourses.second);
                    return event;
                }
                if (x > 161 && x < 423 && y > 373- 48 && y < 552- 48) { // derevnia
                    //getCoorsCard();
                    if (render_type.load() == 0) continue;
                    Event event;
                    event.set_type(EventType::BUILD);
                    auto info = event.mutable_buildinfo();
                    info->set_buildingtype(4);
                    info->set_x(0);
                    info->set_y(0);
                    return event;
                }
                if (x > 161 && x < 423 && y > 610- 48 && y < 790- 48) { // derevnia
                    Event event;
                    event.set_type(EventType::NEXTPHASE);
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
    render_type.store(5);
    SDL_Event e;

    Limiter limit;

    while(true){
        limit.storeStartTime();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > 200 && x < 480 && y > 98- 48 && y < 280- 48) {
                    Mix_PlayChannel(-1, dice_sound, 0);
                    Event event;
                    event.set_type(EventType::DICE);
                    auto nums = event.mutable_diceinfo();
                    nums->set_number1(0);
                    nums->set_number1(0);
                    return event;
                }
                if (x > 200 && x < 480 && y > 300 - 48 && y < 482- 48) {
                    getCoorsCard();
                    if (render_type.load() == 0) continue;
                    Event event;
                    event.set_type(EventType::CARD);
                    auto q = event.mutable_cardinfo();
                    q->set_cardtype(cur_card);
                    return event;
                }
            }
        }
        limit.delay();
    }
}


void GUI::addDice(int x, int y) {
    dice1.store(x);
    dice2.store(y);
    render_type.store(10);
}

bool Obj::is(int x, int y) const {
    return x <= gui_x2 && x >= gui_x1 && y <= gui_y2 && y >= gui_y1;
}

std::pair<int, int> Obj::get_model_coors() {
    return std::make_pair(model_x, model_y);
}

void GUI::addRoad(std::pair<int, int> tmp, int player) {
    std::lock_guard<utility::spinlock> lock(mutex_for_render);
    for (auto& e : roads->vec) {
        if (tmp.first == e.model_x && tmp.second == e.model_y) {
            e.built++;
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
    std::lock_guard<utility::spinlock> lock(mutex_for_render);
    for (auto& e:buildings->vec) {
        if (tmp.first == e.model_x && tmp.second == e.model_y) {
            e.built++;
            //e.texture = build_texture_arr[player];
            //e.cur_texture = cur_build_texture_arr[player];
            // TODO: можно сделать, чтобы для уже построеных зданий cur_texture был пустым, чтобы они не подсвечивались при наведении
            e.colour = (Colour)player;
            if (e.built > 2) e.built = 2;
            return;
        }
    }
    throw (std::runtime_error("Can't add building"));
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
            //std::cout << t << ' ' << j*2  << '\n';
            vec.push_back(tmp);
        }
    }
}

Building_arr::~Building_arr() = default;

void GUI::updatePlayer(int x) {
    cur_player = x;
}

void GUI::updateDevCards (std::vector<bool> vec) {
    dev_cards_vec = vec;
}

void GUI::updatePoints(std::vector<int> vec) {
    std::lock_guard<utility::spinlock> lock(mutex_for_render);
    players_points = vec;

    SDL_Surface* svitok_down = SDL_LoadBMP("image/svitok_down.bmp");
   if (svitok_down == nullptr) {
       throw (std::runtime_error("null"));
   }

    TTF_Font *font = TTF_OpenFont("image/sample.ttf", 32);
    if (font == nullptr) {
        throw (std::runtime_error("null"));
    }

    SDL_Rect dest1={150, 200, 0, 0};

    dest1.y = 30;
    for (auto& e: players_names) {
        SDL_Surface* surf = TTF_RenderText_Blended(font, e.c_str(), color_const_table);
        if (surf == nullptr) {
            throw (std::runtime_error("null"));
        }
        SDL_BlitSurface(surf, nullptr, svitok_down, &dest1);
        dest1.y += 30;
        SDL_FreeSurface(surf);
    }

    TTF_CloseFont(font);

    makeTextureConstTable(players_points, svitok_down, texture_svitok_down, 1);
    SDL_FreeSurface(svitok_down);
}

void GUI::updateResourses(std::vector<int> v) {
    std::lock_guard<utility::spinlock> lock(mutex_for_render);
    resourses = std::move(v);

    SDL_Surface* svitok_up = SDL_LoadBMP("image/svitok_up.bmp");
    if (svitok_up == nullptr) {
        throw (std::runtime_error("null"));
    }

    TTF_Font *font = TTF_OpenFont("image/sample.ttf", 32);
    if (font == nullptr) {
        throw (std::runtime_error("null"));
    }


    SDL_Surface *surf = TTF_RenderText_Blended(font, "WOOL", color_const_table);
    if (surf == nullptr) {
        throw (std::runtime_error("null"));
    }
    SDL_Rect dest1={150, 200, 0, 0};

    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    SDL_FreeSurface(surf);
    surf = TTF_RenderText_Blended(font, "ORE", color_const_table);
    if (surf == nullptr) {
        throw (std::runtime_error("null"));
    }
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    SDL_FreeSurface(surf);
    surf = TTF_RenderText_Blended(font, "BRICKS", color_const_table);
    if (surf == nullptr) {
        throw (std::runtime_error("null"));
    }
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    SDL_FreeSurface(surf);
    surf = TTF_RenderText_Blended(font, "WOOD", color_const_table);
    if (surf == nullptr) {
        throw (std::runtime_error("null"));
    }
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    dest1.y += 30;
    SDL_FreeSurface(surf);
    surf = TTF_RenderText_Blended(font, "WHEAT", color_const_table);
    if (surf == nullptr) {
        throw (std::runtime_error("null"));
    }
    SDL_BlitSurface(surf, nullptr, svitok_up, &dest1);
    TTF_CloseFont(font);

    makeTextureConstTable(resourses, svitok_up, texture_svitok_up, 0);
    SDL_FreeSurface(surf);
    SDL_FreeSurface(svitok_up);
}



void GUI::makeTextureConstTable(std::vector<int>& vec, SDL_Surface* buff, SDL_Texture *&ans, int type) {
    TTF_Font *font = TTF_OpenFont("image/sample.ttf", 32);
    if (font == nullptr) {
        throw (std::runtime_error("null"));
    }
    SDL_Rect dest;
    dest.x = 280 + 90;
    dest.y = 200;
    dest.h = 100;
    dest.w = 100;
    if (type) dest.y = 30;
    for (auto& e: vec) {
        SDL_Surface *surf = TTF_RenderText_Blended(font, std::to_string(e).c_str(), color_const_table);
        if (surf == nullptr) {
            throw (std::runtime_error("null"));
        }
        SDL_BlitSurface(surf, nullptr, buff, &dest);
        dest.y += 30;
        SDL_FreeSurface(surf);
    }
    ans = nullptr;
    ans = SDL_CreateTextureFromSurface(ren, buff);
    TTF_CloseFont(font);
    if (ans == nullptr) {
        throw (std::runtime_error("Can't CreateTextureFromSurface"));
    }
}

void GUI::addPlayerName(int x, std::string s) {
    players_names[x] = std::move(s);
}

int GUI::getPlaceOfGame() {
    render_type.store(11);
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
    render_type.store(12);
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
    render_type.store(13);
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
    render_type.store(14);
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
    } else if (gameStage == 1) {
        event = SecondStage();
        if (event.type() == EventType::NEXTPHASE) gameStage++;
        return event;
    } else {
        event = ThirdStage();
        if (event.type() == EventType::ENDTURN) gameStage = 0;
        return event;
    }
}


void Robber_arr::render(GUI &gui){
    SDL_Rect dest;
    dest.w = 100*sqrt(3);
    dest.h = 200;
    if (gui.render_type == 37) {
        std::pair<int, int> tmp = get_coors(0, gui);
        if (tmp.first!=-1 && tmp.second!= -1) {
            dest.x = tmp.first;
            dest.y = tmp.second;
            SDL_RenderCopy(gui.ren, texture_oct, nullptr, &dest);
        } //else std::cerr << "(";

    }
    std::pair<int, int> tmp = get_coors(1, gui);
    dest.x = tmp.first;
    dest.y = tmp.second;
    if (tmp.first!=-1 && tmp.second!= -1) {
        SDL_RenderCopy(gui.ren, texture_robber, nullptr, &dest);
    } //else std::cerr << ")";    
}

std::pair<int, int> Robber_arr::get_coors(int type, GUI &gui) {
    int tx = gui.displayMode.w / 2 - 250*sqrt(3)/2;
    int ty = gui.displayMode.h / 2 - 550;
    int k = 3;
    int x, y;
    if (type) {x = x_r.load(); y = y_r.load();}
    else {x = x_tmp.load(); y = y_tmp.load();}
    for (int i = 0; i < 5; ++i){
        ty += 150;
        for (int q = 0; q < k; ++q){
            tx += 100*sqrt(3);
            for (int p = 0; p < static_cast<int>(vec.size()); ++p){
                auto e = vec[p];
                if (x > e.first && x < e.first + (100*sqrt(3)) ) {
                    double cur_x = x - e.first;
                    double cur_y = y - e.second;
                    cur_x/= sqrt(3);    
                    if (cur_y < cur_x + 150 && cur_y < 250 - cur_x && cur_y > 50 - cur_x && cur_y > cur_x - 50) {
                        std::vector<int> sameOrderWithModel = {0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 7, 9, 11, 13, 1, 3, 5};
                        number = sameOrderWithModel[p];
                        return e;
                    }
                }
            }
        }
        tx-= 100*sqrt(3)*k;
        if (i < 2) {k+=1;tx-= 50*sqrt(3);}
        else {k-=1;tx+= 50*sqrt(3);}
    }
    
    
    return std::make_pair(-1, -1);
}

void GUI::setRobber(int x) {
    robber->set(x);
}

void Robber_arr::set(int x){
    std::vector<int> sameOrderWithModel = {0, 16, 1, 17, 2, 18, 3, 12, 4, 13, 5, 14, 6, 15, 7, 8, 9, 10, 11};
    number = sameOrderWithModel[x];
    x_r.store(vec[number].first + 50);
    y_r.store(vec[number].second + 50);
}

Robber_arr::Robber_arr (GUI &gui) {
    int tx = gui.displayMode.w / 2 - 250*sqrt(3);
    int ty = gui.displayMode.h / 2 - 550;
    int k = 3;
    for (int i = 0; i < 5; ++i){
        ty += 150;
        for (int q = 0; q < k; ++q){
            tx += 100*sqrt(3);
            vec.push_back(std::make_pair(tx, ty));
        }
        tx-= 100*sqrt(3)*k;
        if (i < 2) {k+=1;tx-= 50*sqrt(3);}
        else {k-=1;tx+= 50*sqrt(3);}
    }
    texture_oct = IMG_LoadTexture(gui.ren, "image/cur_oct.bmp");
    texture_robber = IMG_LoadTexture(gui.ren, "image/robber.bmp");
}

int GUI::getCoorsRobber(GUI &gui) {
    int old_render_type = render_type.load();
    render_type.store(37);
    //SDL_Rect dest;
    SDL_Event e;

    Limiter limit;

    while (!quit.load()) {

        limit.storeStartTime();
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
                robber->x_tmp.store(x);
                robber->y_tmp.store(y);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                if (x > 200 && x < 480 && y > 98 && y < 280) {
                    render_type.store(old_render_type);
                    return robber->number;
                }
                std::pair<int, int> tmp = std::make_pair(robber->x_r.load(), robber->y_r.load());
                robber->x_r.store(x);
                robber->y_r.store(y);
                if (std::make_pair(-1, -1) == robber->get_coors(1, gui)) {
                    robber->x_r.store(tmp.first);
                    robber->y_r.store(tmp.second);
                }
            }
        }

        limit.delay();

    }
    return -1;
}

Resourses_arr::Resourses_arr() {
    SDL_Rect dest = {170, 170, 170, 170};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j){
            dest.x = 570 + i * 550; 
            dest.y = j * 170;
            Obj tmp(570 + i * 550, j * 170,
                    570 + i * 550 + 170, j * 170 + 170,
                   	i,  j, 
                    NONE, dest, j);
            vec.push_back(tmp);
        }
    }
}

void GUI::getCoorsResourses() {
    int old_render_type = render_type.load();
    render_type.store(6);
    SDL_Event e;
    Limiter limit;

    while (!quit.load()) {

        limit.storeStartTime();
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
                tmp_resours.first.store(x);
                tmp_resours.second.store(y);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                if (x > 161 && x < 423 && y > 136 && y < 317) { 
                    return;
                }
                if (x > 161 && x < 423 && y > 373 && y < 552) { // derevnia
                    render_type.store(old_render_type);
                    return;
                }
                int it = 0;
                for (int j = 0; j < static_cast<int>(resourses_img->vec.size()); ++j) {
                    auto &e = resourses_img->vec[j];
                    if (e.is(x,y)) {
                        int tmp = e.built;
                        for (int i = (it/5)*5; i < 5 + 5*(it/5); ++i){
                            resourses_img->vec[i].built = 0;
                        }
                        e.built = tmp + 1;
                        if (j/5) {
                            tmp_resourses.second = j - 5;
                            tmp_resourses_num.second = e.built;
                        } else {
                            tmp_resourses.first = j;
                            tmp_resourses_num.first = e.built;
                        }
                        
                        resourses_img->vec[(j+5)%10].built = 0;
                        break;
                    }
                    it ++;
                } 
            }
        }

        limit.delay();

    }
}


int GUI::getCoorsResoursesCards() {
    int old_render_type = render_type.load();
    render_type.store(67);
    SDL_Event e;
    Limiter limit;
    int cur_resourse = -1;

    while (!quit.load()) {

        limit.storeStartTime();
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
                tmp_resours.first.store(x);
                tmp_resours.second.store(y);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;
                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                if (x > 161 && x < 423 && y > 136 && y < 317) { 
                    return cur_resourse;
                }
                if (x > 161 && x < 423 && y > 373 && y < 552) { // derevnia
                    render_type.store(old_render_type);
                    return -1;
                }
                int it = 0;
                for (int j = 0; j < 5; ++j) {
                    auto &e = resourses_img->vec[j];
                    if (e.is(x,y)) {
                        cur_resourse = j;
                        int tmp = e.built;
                        for (int i = (it/5)*5; i < 5 + 5*(it/5); ++i){
                            resourses_img->vec[i].built = 0;
                        }
                        e.built = tmp + 1;
                        if (j/5) {
                            tmp_resourses.second = j - 5;
                            tmp_resourses_num.second = e.built;
                        } else {
                            tmp_resourses.first = j;
                            tmp_resourses_num.first = e.built;
                        }
                        
                        resourses_img->vec[(j+5)%10].built = 0;
                        break;
                    }
                    it ++;
                } 
            }
        }

        limit.delay();

    }
    return -1;
}


void GUI::getCoorsCard() {
    int old_render_type = render_type.load();
    render_type.store(312);
    SDL_Event e;
    Limiter limit;

    while (!quit.load()) {

        limit.storeStartTime();
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
                tmp_card.store(-1);
                if (y > 175 && y < 623) {
                    for (int i = 0; i < 5; ++i) {
                        if (x > 150 + i * 350 && x < 150 + 300 + i *350) {
                            tmp_card.store(i);
                        }
                    }
                } 
                
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayChannel(-1, button_sound, 0);
                int x, y;

                SDL_GetMouseState(&x, &y); // Получить координаты мыши
                if (x > 845 && x < 1107 && y > 707 && y < 887) { 
                    render_type.store(old_render_type);
                    return;
                }
                if (y > 175 && y < 623) {
                    for (int i = 0; i < 5; ++i) {
                        if (!dev_cards_vec[i]) continue;
                        if (x > 150 + i * 350 && x < 150 + 300 + i *350) {
                            cur_card.store(i + 1);
                            return;
                        }
                    }
                   
                }
                 
            }
        }

        limit.delay();

    }

}

} // namespace GUI