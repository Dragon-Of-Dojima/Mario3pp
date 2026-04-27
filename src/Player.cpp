#include "Player.h"


Player::Player(float playerX, float playerY, float floorY): playerX(48),playerY(0),floorY(0){};
void Player::handleInput(const Uint8* keys){
	
	const float speed = 400.f;
	velocityX = 0.f;
	if (keys[SDL_SCANCODE_LEFT]) {
		velocityX = -speed;
		isFacingLeft = true;
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		velocityX = speed;
		isFacingLeft = false;
	}
	if (keys[SDL_SCANCODE_SPACE] && playerY >= floorY) {
		velocityY = -400.f;
	}
}
void Player::update(float dt, const Level& level){
	velocityY += 800.f * dt;
	playerX   += velocityX * dt;
	playerY   += velocityY * dt;
	if (playerY > floorY) {
		playerY    = floorY;
		velocityY  = 0.f;
	}
	if (velocityX != 0.f){
		animTimer += dt;
	} 
	else{
		animTimer  = 0.f;
	};
}
	