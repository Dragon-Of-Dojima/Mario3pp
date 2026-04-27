#pragma once
#include <SDL2/SDL.h>
#include "Level.h"

class Player{
	private:
		float playerX;
		float playerY;
		float floorY;
		float velocityX;
		float velocityY;
		float animTimer;
		SDL_Texture* texStand;
		SDL_Texture* texStep;
		SDL_Texture* texJump;
		bool isFacingLeft;
		int standW, standH, stepW, stepH, jumpW, jumpH;
	public:
		Player();
		~Player();
		bool loadTextures(SDL_Renderer* renderer);
		void destroyTextures();
		void spawn(float x, float floorY);
		int getStandHeight() const { return standH; }
		void handleInput(const Uint8* keys);
		void update(float dt, const Level& level);
		void render(SDL_Renderer* renderer);
};
