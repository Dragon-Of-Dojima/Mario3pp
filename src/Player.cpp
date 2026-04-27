#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player()
	: playerX(0), playerY(0), floorY(0),
	  velocityX(0), velocityY(0), animTimer(0),
	  texStand(nullptr), texStep(nullptr), texJump(nullptr),
	  isFacingLeft(false),
	  standW(0), standH(0), stepW(0), stepH(0), jumpW(0), jumpH(0) {}

Player::~Player(){
	destroyTextures();
}

void Player::destroyTextures(){
	if(texStand != nullptr){ SDL_DestroyTexture(texStand); texStand = nullptr; }
	if(texStep  != nullptr){ SDL_DestroyTexture(texStep);  texStep  = nullptr; }
	if(texJump  != nullptr){ SDL_DestroyTexture(texJump);  texJump  = nullptr; }
}

bool Player::loadTextures(SDL_Renderer* renderer){
	const char* standPath = "public/images/mariosprites/smallmario/smallMarioStanding.bmp";
	const char* stepPath  = "public/images/mariosprites/smallmario/smallMarioStep.bmp";
	const char* jumpPath  = "public/images/mariosprites/smallmario/smallMarioJump.bmp";

	SDL_Surface* spriteStand = SDL_LoadBMP(standPath);
	SDL_Surface* spriteStep  = SDL_LoadBMP(stepPath);
	SDL_Surface* spriteJump  = SDL_LoadBMP(jumpPath);
	if(spriteStand == nullptr || spriteStep == nullptr || spriteJump == nullptr){
		std::cerr << "Player::loadTextures SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
		if(spriteStand) SDL_FreeSurface(spriteStand);
		if(spriteStep)  SDL_FreeSurface(spriteStep);
		if(spriteJump)  SDL_FreeSurface(spriteJump);
		return false;
	}

	SDL_SetColorKey(spriteStand, SDL_TRUE, SDL_MapRGB(spriteStand->format, 0xFF, 0x80, 0xC0));
	SDL_SetColorKey(spriteStep,  SDL_TRUE, SDL_MapRGB(spriteStep->format,  0xFF, 0x80, 0xC0));
	SDL_SetColorKey(spriteJump,  SDL_TRUE, SDL_MapRGB(spriteJump->format,  0xFF, 0x80, 0xC0));

	texStand = SDL_CreateTextureFromSurface(renderer, spriteStand);
	texStep  = SDL_CreateTextureFromSurface(renderer, spriteStep);
	texJump  = SDL_CreateTextureFromSurface(renderer, spriteJump);

	SDL_FreeSurface(spriteStand);
	SDL_FreeSurface(spriteStep);
	SDL_FreeSurface(spriteJump);

	SDL_QueryTexture(texStand, NULL, NULL, &standW, &standH);
	SDL_QueryTexture(texStep,  NULL, NULL, &stepW,  &stepH);
	SDL_QueryTexture(texJump,  NULL, NULL, &jumpW,  &jumpH);
	return true;
}

void Player::spawn(float x, float floor){
	playerX = x;
	floorY  = floor;
	playerY = floor;
	velocityX = 0.f;
	velocityY = 0.f;
}

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
	(void)level; // collision against tiles will use this soon
	velocityY += 800.f * dt;
	playerX   += velocityX * dt;
	playerY   += velocityY * dt;
	if (playerY > floorY) {
		playerY   = floorY;
		velocityY = 0.f;
	}
	if (velocityX != 0.f){
		animTimer += dt;
	} else {
		animTimer = 0.f;
	}
}

void Player::render(SDL_Renderer* renderer){
	SDL_Texture* currentTex = texStand;
	int drawW = standW;
	int drawH = standH;

	bool useStepSprite = fmod(animTimer, 0.3f) > 0.15f;
	if(useStepSprite){
		currentTex = texStep;
		drawW = stepW;
		drawH = stepH;
	}
	if (playerY < floorY) {
		currentTex = texJump;
		drawW = jumpW;
		drawH = jumpH;
	}

	SDL_Rect dst = { (int)playerX, (int)playerY, drawW, drawH };
	SDL_RendererFlip flip = isFacingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	SDL_RenderCopyEx(renderer, currentTex, NULL, &dst, 0.0, NULL, flip);
}
