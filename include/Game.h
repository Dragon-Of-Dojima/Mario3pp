#pragma once
#include <SDL2/SDL.h>

class Game{
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event event;
		bool isRunning;
		float playerX;
		float playerY;
		float floorY = 531.f;
		float velocityY;
		SDL_Texture* texStand;
		SDL_Texture* texStep;
		SDL_Texture* texJump;
		bool isFacingLeft;
		int standW, standH, stepW, stepH, jumpW, jumpH;
		float animTimer;
		Uint32 lastFrameTicks; //remembers SDL_GetTicks() value from start of prev frame
		static const int MIN_FRAME_TIME = 16;
	public:
		bool init();
		void run();
		void handleEvents();
		void update(float dt);
		void render();
		Game();
		~Game();
};