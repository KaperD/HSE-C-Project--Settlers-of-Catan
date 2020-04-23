class Object {
public:
	virtual std::pair<int, int> get_coor();
	virtual std::pair<int, int> convert_coor();
private:	
}

class Button{
    private:
    SDL_Rect box;  // атрибуты кнопки (местоположение)
    SDL_Rect* clip;  // часть спрайта которая будет показана
 
    public:
    Button( int x, int y, int w, int h );  // конструктор
    void handle_events();  // обработка событий
    void show();  // показывает "кнопку" на экране
};

class Render {
public:
	Render();
	~Render();
	SDL_Renderer *ren;
private:
	SDL_Window *win;
}

class Event {
public: 
	Event();
private:
	int type;
	int x;
	int y;
}

int Event::Event () {

}

class Field {

}

class Road {
public:

private:
}

class Building {

}

class Robber {

}


class GUI {
	void main_loop();
	void mode_rode();
	void mode_building();
	void mode_robber();
}
//поставить дорогу
//поставить поселение
//потавить разбойника

/* вызываем запрос события, получаем какое то событие, смотрим следует ли его передать модели или нет, вызываем соответствующие функции