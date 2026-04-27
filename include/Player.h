#pragma once
#include <SDL2/SDL_image.h>
#include <vector>

class Player{
	private:
		float playerX;
		float playerY;
		float floorY;
		float velocityY;
		float animTimer;
		SDL_Texture* texStand;
		SDL_Texture* texStep;
		SDL_Texture* texJump;
		bool isFacingLeft;
		int standW, standH, stepW, stepH, jumpW, jumpH;
	public:
		Player(float playerX, float playerY, float floorY);
		void handleInput(const Uint8* keys);
		void update(float dt, const Level& level);
		void render(SDL_Renderer* r);

	}