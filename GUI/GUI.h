#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <ctime>


#include "game.grpc.pb.h"
#include "Client.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

class View {
public:
    View();
    void update();
    ::game::Event getTurn();
    void endGame();
	void loop();
    void build(int x, int y);

    SDL_DisplayMode displayMode;
    SDL_Renderer *ren;
    SDL_Texture *back_ground;
    SDL_Window *win;
    SDL_Surface *BMP_background;
    SDL_Texture* arr[6];
    SDL_Surface *BMP_road;
    SDL_Surface *BMP_road1;
    SDL_Texture *road1;
    SDL_Surface *BMP_road2;
    SDL_Texture *road2;
    SDL_Rect dest;
    SDL_Texture *road;
    SDL_Surface *BMP_back;
    SDL_Texture *back;

    const int FPS = 30;
    const int frameDelay = 1000 / FPS;

    uint32_t frameStart = 0;
    int frameTime = 0;
	static int qwe;
};


inline void View::loop() {
	frameStart = SDL_GetTicks();

	SDL_RenderPresent(ren);
	//std::cout << 6 << std::endl;

	frameTime = SDL_GetTicks() - frameStart;
	if (frameDelay > frameTime) {
        SDL_Delay(frameDelay - frameTime);
    }
}


inline void View::build(int x, int y) {
    std::cout << x << ' ' << y << '\n';
    for (int i = 0; i < 5; ++i){
        int a = 0;
        int b = 11;
        if (i == 0) {a = 1; b = 10;}
        if (i == 4) {a = 1; b = 10;}
        for (int j = a; j < b; ++j){
            if ((i+j) % 2 != 0) continue;
            if (x > 487+ j*50*sqrt(3) && x <  515+j*50*sqrt(3) && y > 199 + 3*i*50 && y < 294 + 3*i*50) {
                dest.x = 460 + j*50*sqrt(3);
                dest.y = 150 + 50 + 3*i*50;
                dest.w = 80;//60
                dest.h = 100;//100
                SDL_RenderCopy(ren, road, NULL, &dest);
                break;
            }
        }
    }



    for (int i = 0; i < 6; ++i){
		        	int a = 0;
		        	int b = 10;
		        	if (i == 0) {a = 2; b = 8;}
		        	if (i == 1) {a = 1; b = 9;}
		        	if (i == 5) {a = 2; b = 8;}
		        	if (i == 4) {a = 1; b = 9;}
		        	for (int j = a; j < b; ++j){
		        		if ((i+j) % 2 == 0) {
				        if (x > 499+ j*50*sqrt(3) && x <  570+j*50*sqrt(3) && y > 160 + 3*i*50 && y < 195 + 3*i*50) {
				        	dest.x = 500 + j*50*sqrt(3);//470 + 2*100*sqrt(3);
							dest.y = 140 + 3*i*50;//150 + 50;
							dest.w = 50*sqrt(3);//60
							dest.h = 80;//100
							SDL_RenderCopy(ren, road1, NULL, &dest); //Копируем в рендер персонажа
							break;
						}}
						else {
							if (x > 499+ j*50*sqrt(3) && x <  570+j*50*sqrt(3) && y > 160 + 3*i*50 && y < 195 + 3*i*50) {
				        	dest.x = 500 + j*50*sqrt(3);//470 + 2*100*sqrt(3);
							dest.y = 140 + 3*i*50;//150 + 50;
							dest.w = 50*sqrt(3);//60
							dest.h = 80;//100
							SDL_RenderCopy(ren, road2, NULL, &dest); //Копируем в рендер персонажа
							break;
						}
						}
			        }
			    }
    //SDL_RenderPresent(ren);
}


inline void View::update() {
    SDL_RenderClear(ren); //Очистка рендера
    SDL_Rect dest1;
        dest1.x = 0;
        dest1.y = 0;
        dest1.w = displayMode.w;
        dest1.h = displayMode.h;
    SDL_RenderCopy(ren,back_ground,NULL,&dest1); //Копируем в рендер фон

        dest1.x = 0;
        dest1.y = 0;
        dest1.w = displayMode.w;
        dest1.h = displayMode.h;
    SDL_RenderCopy(ren,back,NULL,&dest1); //Копируем в рендер фон
    SDL_Rect dest;
    dest.x = displayMode.w / 2 - 265*sqrt(3);
    dest.y = displayMode.h / 2 - 540;
    dest.w = 100*sqrt(3);
    dest.h = 200;
    int k = 3;
    for (int i = 0; i < 5; ++i){
        dest.y += 150;
        for (int i = 0; i < k; ++i){
            dest.x += 100*sqrt(3);
            SDL_RenderCopy(ren, arr[rand() % 6], NULL, &dest); //Копируем в рендер персонажа
        }
        dest.x-= 100*sqrt(3)*k;
        if (i < 2) {k+=1;dest.x-= 50*sqrt(3);}
        else {k-=1;dest.x+= 50*sqrt(3);}
    }
    SDL_RenderPresent(ren);
	
}

inline ::game::Event View::getTurn() {
    SDL_Event e;
   
    bool quit = false;
    //SDL_RenderPresent(ren);
    SDL_PumpEvents();
    SDL_FlushEvents(0, UINT32_MAX);
    
	while (!quit)
	{
		//SDL_RenderPresent(ren);
		// Обработка событий
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
                ::game::Event ret;
                ret.set_type(::game::EventType::ENDGAME);
                return ret;
			}
			/*if (e.type == SDL_KEYDOWN)
			{
				quit = true;
			}*/
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
		        int x, y;
		        SDL_GetMouseState(&x, &y); // Получить координаты мыши
		        
		        for (int i = 0; i < 5; ++i){
		        	int a = 0;
		        	int b = 11;
		        	if (i == 0) {a = 1; b = 10;}
		        	if (i == 4) {a = 1; b = 10;}
		        	for (int j = a; j < b; ++j){
		        		if ((i+j) % 2 != 0) continue;
				        if (x > 487+ j*50*sqrt(3) && x <  515+j*50*sqrt(3) && y > 199 + 3*i*50 && y < 294 + 3*i*50) {
				        	dest.x = 460 + j*50*sqrt(3);
							dest.y = 150 + 50 + 3*i*50;
							dest.w = 80;//60
							dest.h = 100;//100


							::game::Event ret;
                            ret.set_type(::game::EventType::BUILD);
                            auto xy = ret.mutable_buildinfo();
                            xy->set_x(x);
                            xy->set_y(y);
                            return ret;
						}
			        }
			    }



			    for (int i = 0; i < 6; ++i){
		        	int a = 0;
		        	int b = 10;
		        	if (i == 0) {a = 2; b = 8;}
		        	if (i == 1) {a = 1; b = 9;}
		        	if (i == 5) {a = 2; b = 8;}
		        	if (i == 4) {a = 1; b = 9;}
		        	for (int j = a; j < b; ++j){
		        		if ((i+j) % 2 == 0) {
				        if (x > 499+ j*50*sqrt(3) && x <  570+j*50*sqrt(3) && y > 160 + 3*i*50 && y < 195 + 3*i*50) {
				        	dest.x = 500 + j*50*sqrt(3);//470 + 2*100*sqrt(3);
							dest.y = 140 + 3*i*50;//150 + 50;
							dest.w = 50*sqrt(3);//60
							dest.h = 80;//100


                            ::game::Event ret;
                            ret.set_type(::game::EventType::BUILD);
                            auto xy = ret.mutable_buildinfo();
                            xy->set_x(x);
                            xy->set_y(y);
                            return ret;
						}}
						else {
							if (x > 499+ j*50*sqrt(3) && x <  570+j*50*sqrt(3) && y > 160 + 3*i*50 && y < 195 + 3*i*50) {
				        	dest.x = 500 + j*50*sqrt(3);//470 + 2*100*sqrt(3);
							dest.y = 140 + 3*i*50;//150 + 50;
							dest.w = 50*sqrt(3);//60
							dest.h = 80;//100


							::game::Event ret;
                            ret.set_type(::game::EventType::BUILD);
                            auto xy = ret.mutable_buildinfo();
                            xy->set_x(x);
                            xy->set_y(y);
                            return ret;
						}
						}
			        }
			    }
                if (x < 100 && y < 100) {
                    //update();
                    ::game::Event ret;
                    ret.set_type(::game::EventType::ENDTURN);
                    return ret;
                }
                //SDL_RenderPresent(ren);

	     	}
		}
		loop();

		// Отображение сцены
		// SDL_RenderPresent(ren); //Погнали!!

	}
    ::game::Event ret;
    ret.set_type(::game::EventType::ENDTURN);
    return ret;
}



inline View::View() {
	std::cout << ++qwe << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
    //srand(time(0));

    if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        return;
    }
    SDL_GetDesktopDisplayMode(0,&displayMode);

    win = SDL_CreateWindow("Hello World!", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
	}



	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return;
	}    


	BMP_background = SDL_LoadBMP("image/back_ground.bmp");
	if (BMP_background == nullptr){
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		return;
	}

	back_ground = SDL_CreateTextureFromSurface(ren, BMP_background);
	SDL_FreeSurface(BMP_background); //Очищение памяти поверхности
	if (back_ground == nullptr){
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return;
	}

	std::string s = "image/oct .bmp";
	for (int i = 0; i < 6; ++i) {
		s[9] = i + '0';
		SDL_Surface *BMP_player1 = SDL_LoadBMP(s.c_str());
		if (BMP_player1 == nullptr){
			std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
			return;
		}

		SDL_Texture *player1 = SDL_CreateTextureFromSurface(ren, BMP_player1);
		SDL_FreeSurface(BMP_player1); //Очищение памяти поверхности
		if (player1 == nullptr){
			std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return;
		}
		arr[i] = player1;
	}

	BMP_road = SDL_LoadBMP("image/road.bmp");
	if (BMP_road == nullptr){
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		return;
	}

	



	BMP_road1 = SDL_LoadBMP("image/road1.bmp");
	if (BMP_road1 == nullptr){
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		return;
	}

	road1 = SDL_CreateTextureFromSurface(ren, BMP_road1);
	SDL_FreeSurface(BMP_road1); //Очищение памяти поверхности
	if (road1 == nullptr){
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return;
	}


	BMP_road2 = SDL_LoadBMP("image/road2.bmp");
	if (BMP_road2 == nullptr){
		std::cout << "SDL_LoadBMP Error: " <<SDL_GetError() << std::endl;
		return;
	}

	road2 = SDL_CreateTextureFromSurface(ren, BMP_road2);
	SDL_FreeSurface(BMP_road2); //Очищение памяти поверхности
	if (road2 == nullptr){
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return;
	}

	road = SDL_CreateTextureFromSurface(ren, BMP_road);
	SDL_FreeSurface(BMP_road); //Очищение памяти поверхности
	if (road == nullptr){
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return;
	}

	BMP_back = SDL_LoadBMP("image/back.bmp");
	if (BMP_back == nullptr){
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		return;
	}

	back = SDL_CreateTextureFromSurface(ren, BMP_back);
	SDL_FreeSurface(BMP_back); //Очищение памяти поверхности
	if (back == nullptr){
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return;
	}

    update();
	//SDL_RenderPresent(ren);
}


inline void View::endGame() {
    std::cout << 41.0/60.0;
    //SDL_DestroyTexture(player);
    SDL_DestroyTexture(road);
    SDL_DestroyTexture(road1);
    SDL_DestroyTexture(road2);
	SDL_DestroyTexture(back_ground);
	SDL_DestroyTexture(back);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();		
}
