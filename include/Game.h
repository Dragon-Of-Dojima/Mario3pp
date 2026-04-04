#pragma once
#include <SDL2/SDL.h>

class Game{
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event event;
		long frameTime;
		bool isRunning;
		static const int MIN_FRAME_TIME = 16;
	public:
		bool init();
		void run();
		void handleEvents();
		void update();
		void render();
		Game(void);
		~Game(void);
};