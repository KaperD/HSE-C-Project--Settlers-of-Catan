#include <SDL2/SDL.h>
#include <SFML/System.hpp>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <random>
#include <time.h>
class GUI {
public:
	std::vector<std::pair<int, int>> roades_vec;
	std::vector<std::pair<int, int>> hous_vec;
	std::pair<int, int> tmp_rode;
	Mix_Chunk *sfx, *button_sound, *build_sound;
	std::pair<int, int> tmp_hous;
	std::pair<SDL_Texture*, int> cur_table;
	SDL_Texture *back, *back_ground, *road, *road1, *road2,
	 *oct, *cur_road, *cur_road1, *cur_road2, *table, *table_1, *table_2, *table_time, *hous, *hous1, *hous2, *hous_cur;
	SDL_Texture* arr[6];
	bool quit = false;
	int field_arr[19];
	int tmp_sound = 0;
	int render_type;
	SDL_Renderer *ren;
	SDL_DisplayMode displayMode;
	SDL_Window *win;
	sf::Thread thread_music(&play_music);
	sf::Thread thread(&upgrade);

	GUI();
	~GUI();
	void play_music();
	void load_textures();
	void destroy_textures();
	void render_background();
	void render_tables();
	void render_field();
	void road_type(std::pair<int, int> e, SDL_Texture * fst_road, SDL_Texture * snd_road, SDL_Texture * thrd_road);
	void render_roades();
	void hous_type(std::pair<int, int> e, SDL_Texture * hous);
	void render_houses();
	void render_tables_time();
	void make_render();
	void upgrade();
	void get_coors_road();
	void get_coors_building();
	void main_menu();
};

void play_music() {
	while (!quit){
		std::cout << 1;
		Mix_PlayChannel(-1, sfx, 0);
		for (int i = 0; i < 18000; ++i) {
			if (quit) return;
			SDL_Delay(10);
		}
	}
}

void load_textures() {
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
	table_1 = IMG_LoadTexture(ren, "image/table_1.bmp");
	table_2 = IMG_LoadTexture(ren, "image/table_1.bmp");
	table_time = IMG_LoadTexture(ren, "image/table_time.bmp");
	hous = IMG_LoadTexture(ren, "image/hous.bmp");
	hous_cur = IMG_LoadTexture(ren, "image/hous_cur.bmp");
	hous1 = IMG_LoadTexture(ren, "image/hous1.bmp");
	hous2 = IMG_LoadTexture(ren, "image/hous2.bmp");
	sfx = NULL;
	sfx = Mix_LoadWAV("image/music.wav");
	button_sound = Mix_LoadWAV("image/button_sound.wav");
	build_sound = Mix_LoadWAV("image/build_sound.wav");
	if (sfx == NULL)  std::cout << "HUY";
}

void destroy_textures() {
	for (int i = 0; i < 6; ++i) {
		SDL_DestroyTexture(arr[i]);
	}
	Mix_CloseAudio();
	SDL_DestroyTexture(road);
    SDL_DestroyTexture(road1);
    SDL_DestroyTexture(road2);
	SDL_DestroyTexture(back_ground);
	SDL_DestroyTexture(back);
}

void render_background() {
	SDL_Rect dest1;
	dest1.x = 0;
	dest1.y = 0;
	dest1.w = displayMode.w;
	dest1.h = displayMode.h;
	SDL_RenderCopy(ren,back_ground,NULL,&dest1); //Копируем в рендер фон
	SDL_RenderCopy(ren,back,NULL,&dest1); //Копируем в рендер фон
}

void render_tables() {
	SDL_Rect desttt;
	desttt.x = 200;
	desttt.y = 98;
	desttt.w = 480 - 200;
	desttt.h = 280 - 98;
	if (render_type == 0)
		SDL_RenderCopy(ren, table, NULL,&desttt); 
	if (render_type)
		SDL_RenderCopy(ren, table_1, NULL,&desttt); 
	desttt.x = 200;
	desttt.y = 300;
	desttt.w = 480 - 200;
	desttt.h = 280 - 98;
	if (render_type == 0)
		SDL_RenderCopy(ren, table_2, NULL,&desttt); 
}


void render_field() {
	SDL_Rect dest;
	dest.x = displayMode.w / 2 - 250*sqrt(3);
	dest.y = displayMode.h / 2 - 550;
	dest.w = 100*sqrt(3);
	dest.h = 200;
	int k = 3;
	int it = 0;
	for (int i = 0; i < 5; ++i){
		dest.y += 150;
		for (int i = 0; i < k; ++i){
			dest.x += 100*sqrt(3);
			SDL_RenderCopy(ren, arr[field_arr[it++]], NULL, &dest); //Копируем в рендер персонажа
		}
		dest.x-= 100*sqrt(3)*k;
		if (i < 2) {k+=1;dest.x-= 50*sqrt(3);}
		else {k-=1;dest.x+= 50*sqrt(3);}
	}

}

void render_roades() {
	SDL_Rect dest;
	for (auto e: roades_vec) {
		if (e.builded)
			SDL_RenderCopy(ren, e.texture, NULL, &e.dest);
	}
	if (render_type == 1) {
		SDL_RenderCopy(ren, .cur_texture, NULL, &e.dest);
	}
}

void hous_type(std::pair<int, int> e, SDL_Texture * hous) {
	int x, y;
	x = e.first;
	y = e.second;
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
			if (x > 506 + j*50*sqrt(3) && x < 546 + j*50*sqrt(3) && y > 130 + i*50 && y < 168 + i*50) {
				dest.x = 506 -30+ j*50*sqrt(3);
				dest.y = 130 -40+ i*50;
				dest.w = 100;//60
				dest.h = 100;//100
				SDL_RenderCopy(ren, hous, NULL, &dest); //Копируем в рендер персонажа
				break;
			}		
		}
	}
}

void render_houses() {
	SDL_Rect dest;
	for (auto e: hous_vec) {
		hous_type(e, hous);
	}
	if (render_type == 2) {
		hous_type(tmp_hous, hous_cur);
	}
}

void render_tables_time() {
	SDL_Rect (desttt);
	desttt.x = 200 + 500;
	desttt.y = 98 - 20;
	desttt.w = 480 - 200;
	desttt.h = 280 - 98;
	if (clock() < cur_table.second) SDL_RenderCopy(ren, cur_table.first, NULL, &desttt);
}



void make_render() {
	 SDL_RenderClear(ren);
	 render_background();
	 render_field();
	 render_roades();
	 render_houses();
	 render_tables();
	 render_tables_time();

}

void upgrade() {
	pop:
	SDL_Delay(20);
	make_render();
	SDL_RenderPresent(ren); 
	if (quit) return;
	goto pop;
} 


GUI::GUI() {
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    SDL_GetDesktopDisplayMode(0,&displayMode);
    win = SDL_CreateWindow("Hello World!", 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_FULLSCREEN);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	load_textures();
	
    thread.launch();
    thread_music.launch();
}


void get_coors_road () {
	Mix_PlayChannel(-1, build_sound, 0);
	int old_render_type = render_type;
	render_type = 1;
	SDL_Rect dest;
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
		        tmp_rode = std::make_pair(x,y);
		    }
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				Mix_PlayChannel(-1, button_sound, 0);
				int x, y;
		    	SDL_GetMouseState(&x, &y); // Получить координаты мыши
		    	std::cout << x << ' ' << y << '\n';
				if (x > 200 && x < 480 && y > 98 && y < 280) {
					render_type = old_render_type;
					return;
				}
				tmp_ = chek(std::make_pair(x, y));
				if (tmp_ != -1) return;
			}
		}
	}
}

void get_coors_building () {
	Mix_PlayChannel(-1, build_sound, 0);
	int old_render_type = render_type;
	render_type = 2;
	SDL_Rect dest;
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
		        tmp_hous = std::make_pair(x,y);
		    }
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				Mix_PlayChannel(-1, button_sound, 0);
				int x, y;
		    	SDL_GetMouseState(&x, &y); // Получить координаты мыши
		    	std::cout << x << ' ' << y << '\n';
				if (x > 200 && x < 480 && y > 98 && y < 280) {
					render_type = old_render_type;
					return;
				}
				add_hous(x,y);
			}
		}
	}
}

std::pair<int, int> main_menu () {
	SDL_Rect dest;
	SDL_Event e;
	render_type = 0;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
				break;
			}
			
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				Mix_PlayChannel(-1, button_sound, 0);
				int x, y;
		    	SDL_GetMouseState(&x, &y); 
				if (x > 200 && x < 480 && y > 98 && y < 280) get_coors_road();
				if (x > 200 && x < 480 && y > 300 && y < 482) get_coors_building();
	     	}
		}
	}
}

enum Colour {
	YELLLOW,
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
		double y2, int x, int y, 
		SDL_Texture *_texture, SDL_Rect _dest):
		gui_x1(x1), gui_x2(x2), gui_y1(y1), 
		gui_y2(y2), model_x(x), model_y(y),
		texture(_texture), dest(_dest);
		


    double gui_x1, gui_y1, gui_x2, gui_y2;

    int model_x, model_y;

    SDL_Texture *texture;
    SDL_Rect dest;

    bool builded = false;
    Colour colour;

    std::pair<int, int> get_model_coors();
};

bool Obj::is(int x, int y) {
    return x <= gui_x1 && x >= gui_x2 && y <= gui_y1 && y >= gui_y2;
}

std::pair<int, int> Obj::get_model_coors() {
    return std::make_pair(model_x, model_y);
}

class Road_arr {
public:
    Road_arr();
    ~Road_arr();
    std::vector<Obj> vec;
    SDL_Texture *get_road(int x);
	void add_road(std::pair<int, int> tmp);
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
	DX = (int)(double)50*sqrt(3)*100000,
	DY = 3*50,
	SCALE = 100000
};

SDL_Texture *get_road(int x) {
	if (x%2==0) return snd_road;
	return thrd_road;
}

void add_road(std::pair<int, int> tmp) {
	for (auto e:vec) {
		if (tmp.first == e.model_x && tmp.second == e.model_y)
			e.builded = true;
	}
}

int chek(std::pair<int, int> tmp) {
	for (auto e:vec) {
		if (tmp.first < e.gui_x1 && tmp.first > e.gui_x2 &&
			tmp.second < e.gui_y1 && tmp.second > e.gui_y2)
			return i;
	}
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
            dest.x = 470 + 20 + j*50*sqrt(3);
            dest.y = 150 + 50 + 3*i*50;
            dest.w = 80;
            dest.h = 100;
            Obj tmp(X1 + j*DX/SCALE, Y1 + i*DY,
            					X2 + j*DX/SCALE, Y2 + i*DY, 
				            	i * 2 + 1, j * 2,
				            	dest, fst_road);
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
            SDL_Texture cur_texture = get_road(i+j);
        	Obj tmp = new Obj(X1_G + j*DX/SCALE, Y1_G + i*DY,
        					X2_G + j*DX/SCALE, Y2_G + i*DY, 
			            	i * 2, 	j * 2 + 1,
			            	dest, cur_texture);
            vec.push_back(tmp);
        }
    }
}

class Building_arr {
    Building_arr();
    ~Building_arr();
};

GUI::~GUI() {
	destroy_textures();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

int main (int argc, char ** args) {
	for (int i = 0; i < 19; ++i) {
		field_arr[i] = rand()%6;
	}
	begin_gui();
	main_menu();
	end_gui();
    return 0;
};