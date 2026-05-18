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

	// Resolve X and Y separately. If we move both at once and resolve Y first, a side wall
	// the player just pushed into looks like a ceiling/floor to the Y resolver, which would
	// snap the player vertically through level geometry.

	// ---- X axis ----
	this->playerX += this->velocityX * dt;
	if (this->velocityX != 0.f) {
		SDL_FRect b = this->getBounds();
		float topX    = b.y;
		float botX    = b.y + b.h;
		float leftX   = b.x;
		float rightX  = b.x + b.w;
		// Use bot-1 so a player resting on a tile boundary doesn't treat that tile as a side wall.
		int bottomRowX = level.rowFromY(botX - 1);
		int topRowX    = level.rowFromY(topX);
		int probeCol;
		float resolvedX;
		if (this->velocityX > 0) {
			probeCol  = level.colFromX(rightX + 1);
			resolvedX = (probeCol * (float)TILE_SIZE) - b.w;
		} else {
			probeCol  = level.colFromX(leftX);
			resolvedX = (probeCol + 1) * (float)TILE_SIZE;
		}
		for (int r = bottomRowX; r <= topRowX; r++) {
			if (isSolid(level.tileAt(probeCol, r))) {
				this->playerX = resolvedX;
				this->velocityX = 0;
				break;
			}
		}
	}

	// ---- Y axis ----
	this->playerY += this->velocityY * dt;
	{
		SDL_FRect b = this->getBounds();
		float topY    = b.y;
		float botY    = b.y + b.h;
		float leftY   = b.x;
		float rightY  = b.x + b.w;
		// Probe botY (no -1) so we land on a tile exactly when bottomEdge == tile top.
		int bottomRowY = level.rowFromY(botY);
		int topRowY    = level.rowFromY(topY);
		int leftColY   = level.colFromX(leftY);
		int rightColY  = level.colFromX(rightY - 1);

		for (int c = leftColY; c <= rightColY; c++){
			for (int r = bottomRowY; r <= topRowY; r++){
				if (isSolid(level.tileAt(c, r))){
					if (this->velocityY > 0){
						this->playerY = (GAME_HEIGHT - TILE_SIZE * (r + 1)) - b.h;
						this->velocityY = 0;
					}
					else if (this->velocityY < 0){
						this->playerY = GAME_HEIGHT - TILE_SIZE * r;
						this->velocityY = 0;
					}
					break;
				}
			}
		}
	}


	// "Grounded" check: probe the row directly below the player's feet so isInAir stays
	// false while resting on a tile (gravity-induced overshoot resolution alone is too jittery).
	SDL_FRect finalBounds = this->getBounds();
	int rowBelow = level.rowFromY(finalBounds.y + finalBounds.h);
	int feetLeftCol  = level.colFromX(finalBounds.x);
	int feetRightCol = level.colFromX(finalBounds.x + finalBounds.w - 1);
	bool grounded = false;
	for (int c = feetLeftCol; c <= feetRightCol; c++){
		if (isSolid(level.tileAt(c, rowBelow))){
			grounded = true;
			break;
		}
	}
	this->isInAir = !grounded;

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
