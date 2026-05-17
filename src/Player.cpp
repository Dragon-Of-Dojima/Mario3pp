#include "Player.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

Player::Player()
	: playerX(0), playerY(0), floorY(0),
	  velocityX(0), velocityY(0), animTimer(0),
	  texStand(nullptr), texStep(nullptr), texJump(nullptr),
	  isFacingLeft(false), isHoldingRun(false), isInAir(false),
	  standW(0), standH(0), stepW(0), stepH(0), jumpW(0), jumpH(0) {}

Player::~Player(){
	destroyTextures();
}

void Player::destroyTextures(){
	if(this->texStand != nullptr){ 
		SDL_DestroyTexture(this->texStand); 
		this->texStand = nullptr; 
	}
	if(this->texStep  != nullptr){ 
		SDL_DestroyTexture(this->texStep);
		this->texStep  = nullptr;
	}
	if(this->texJump  != nullptr){ 
		SDL_DestroyTexture(this->texJump);  
		this->texJump  = nullptr; 
	}
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

	this->texStand = SDL_CreateTextureFromSurface(renderer, spriteStand);
	this->texStep  = SDL_CreateTextureFromSurface(renderer, spriteStep);
	this->texJump  = SDL_CreateTextureFromSurface(renderer, spriteJump);

	SDL_FreeSurface(spriteStand);
	SDL_FreeSurface(spriteStep);
	SDL_FreeSurface(spriteJump);

	SDL_QueryTexture(this->texStand, NULL, NULL, &this->standW, &this->standH);
	SDL_QueryTexture(this->texStep,  NULL, NULL, &this->stepW,  &this->stepH);
	SDL_QueryTexture(this->texJump,  NULL, NULL, &this->jumpW,  &this->jumpH);
	return true;
}

SDL_FRect Player::getBounds() const{
	return {this->playerX, this->playerY, (float)standW, (float)standH};
}

void Player::spawn(float x, float floor){
	this->playerX = x;
	this->floorY  = floor;
	this->playerY = floor;
	this->velocityX = 0.f;
	this->velocityY = 0.f;
}

void Player::handleInput(const Uint8* keys, bool holdingRun){
	float speed = 300.f;
	this->isHoldingRun = holdingRun;
	this->velocityX = 0.f;
	if (keys[SDL_SCANCODE_LEFT]) {
		this->velocityX = -speed;
		this->isFacingLeft = true;
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		this->velocityX = speed;
		this->isFacingLeft = false;
	}
	if (keys[SDL_SCANCODE_SPACE] && !this->isInAir) {
		this->velocityY = -350.f;
	}
}

void Player::update(float dt, const Level& level){
	this->velocityY += 800.f * dt;
	this->isInAir = true;


	// bool isTileUnderSolid = isSolid(level.tileAt(colOfPlayer,rowOfPlayer - 1));
	// bool isTileNextToSolid = isSolid(level.tileAt(colOfPlayer + 1,rowOfPlayer));
	
	this->playerX += this->velocityX * dt;
	this->playerY += velocityY * dt;
	SDL_FRect theBounds = this->getBounds();
	float topEdge = theBounds.y;
	float bottomEdge = theBounds.y + theBounds.h;
	float leftEdge = theBounds.x;
	float rightEdge = theBounds.x + theBounds.w;

	int lowRow = level.rowFromY(bottomEdge - 1); //low (physically high) on screen
	int highRow = level.rowFromY(topEdge); //high (physically low) on screen
	int leftCol = level.colFromX(leftEdge);
	int rightCol = level.colFromX(rightEdge - 1);
	for (int c = leftCol; c <= rightCol; c++){
		for(int r = highRow; r <= lowRow; r++){
			if(isSolid(level.tileAt(c,r))){
				if(this->velocityY > 0){
					this->playerY = (GAME_HEIGHT - TILE_SIZE * (r + 1)) - theBounds.h;
					this->isInAir = false;
					this->velocityY = 0;
				}
				else if(this->velocityY < 0){
					this->playerY = GAME_HEIGHT - TILE_SIZE * r;
					
					this->velocityY = 0;
				}
				
				break;
			}
		}
	}
	for (int c = leftCol; c <= rightCol; c++){
		for(int r = highRow; r <= lowRow; r++){

		}
	}



	if (this->velocityX != 0.f){
		this->animTimer += dt;
	} else {
		this->animTimer = 0.f;
	}
}

void Player::render(SDL_Renderer* renderer){
	SDL_Texture* currentTex = this->texStand;
	int drawW = this->standW;
	int drawH = this->standH;

	bool useStepSprite = fmod(this->animTimer, 0.3f) > 0.15f;
	if(useStepSprite){
		currentTex = this->texStep;
		drawW = this->stepW;
		drawH = this->stepH;
	}
	if (this->isInAir) {
		currentTex = this->texJump;
		drawW = this->jumpW;
		drawH = this->jumpH;
	}

	//SDL_Rect dst = { (int)this->playerX, (int)this->playerY, drawW, drawH };
	SDL_FRect bounds = this->getBounds();
	std::cout << "bounds after spawn: x=" << bounds.x << " y=" << bounds.y << " w=" << bounds.w << " h=" << bounds.h << std::endl;
	SDL_Rect dst = { (int)bounds.x, (int)bounds.y, drawW, drawH }; //dst = destination rectangle
	SDL_RendererFlip flip = this->isFacingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	SDL_RenderCopyEx(renderer, currentTex, NULL, &dst, 0.0, NULL, flip);
	/*
	int SDL_RenderCopyEx(
		SDL_Renderer* renderer,
		SDL_Texture* texture,
		const SDL_Rect* srcrect,     // source rectangle: what part of the texture to copy
		const SDL_Rect* dstrect,     // destination rectangle: where on screen to put it
		double angle,
		const SDL_Point* center,
		SDL_RendererFlip flip
	);
	*/ 
}
