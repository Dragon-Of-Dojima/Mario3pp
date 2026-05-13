#pragma once
#include <SDL2/SDL.h>
#include "Level.h"
#include "Tiles.h"
#include "Player.h"

class Game{
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event event;
		static const int GAME_WIDTH_DEBUG = 1600;
		static const int GAME_WIDTH_PROD = 1286;
		bool isRunning;
		Uint32 lastFrameTicks; //remembers SDL_GetTicks() value from start of prev frame
		static const int MIN_FRAME_TIME = 16;
		Player player;
		Level level;
		SDL_Texture* tileTextures[TILE_COUNT]; //if I used a scoped enum (meaning it was also a class), the TC would need Tiles::
		bool isDebug;
		int gameWidth;
	public:
		bool init();
		void run();
		void handleEvents();
		void update(float dt);
		void render();
		Game();
		~Game();
};