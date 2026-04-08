#include "Game.h"
#include <iostream>
#include <vector>
#include <cmath>

Game::Game(): window(nullptr), renderer(nullptr), isRunning(false), lastFrameTicks(0), playerX(48), playerY(531.f), texStand(nullptr), texStep(nullptr), texJump(nullptr), velocityY(0),animTimer(0.0){}

//static enum colorsToIgnore = {"4491be","ff80c0","5edb92"}

bool Game::init(){
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return false;
	}	
	this->window = SDL_CreateWindow(
		"Mario 3++",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	if (this->window == nullptr) {
		std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == nullptr) {
		std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(this->window);
		this->window = nullptr;
		SDL_Quit();
		return false;
	}

	SDL_Surface* spriteStand = SDL_LoadBMP("public/images/mariosprites/smallmario/smallMarioStanding.bmp");
	SDL_Surface* spriteStep = SDL_LoadBMP("public/images/mariosprites/smallmario/smallMarioStep.bmp");
	SDL_Surface* spriteJump = SDL_LoadBMP("public/images/mariosprites/smallmario/smallMarioJump.bmp");
	std::vector<SDL_Surface*> sprites = {spriteStand,spriteStep,spriteJump};
	for(SDL_Surface* sprite : sprites){
		if (sprite == nullptr) {
			std::cerr << "SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
			return false;
		}
	}
	SDL_SetColorKey(spriteStand, SDL_TRUE, SDL_MapRGB(spriteStand->format, 0xFF, 0x80, 0xC0));
	SDL_SetColorKey(spriteStep, SDL_TRUE, SDL_MapRGB(spriteStep->format, 0xFF, 0x80, 0xC0));
	SDL_SetColorKey(spriteJump, SDL_TRUE, SDL_MapRGB(spriteJump->format, 0xFF, 0x80, 0xC0));
	this->texStand = SDL_CreateTextureFromSurface(this->renderer, spriteStand);
	this->texStep = SDL_CreateTextureFromSurface(this->renderer, spriteStep);
	this->texJump = SDL_CreateTextureFromSurface(this->renderer, spriteJump);
	
	SDL_FreeSurface(spriteStand);
	SDL_FreeSurface(spriteStep);
	SDL_FreeSurface(spriteJump);
	return true;
	
	
}
void Game::run(){
	this->isRunning = true;

	while (this->isRunning) {
		const Uint32 frameStart = SDL_GetTicks();

		float dt = 0.f; //seconds since last frame
		if (this->lastFrameTicks != 0) {
			dt = (frameStart - this->lastFrameTicks) / 1000.f;
		}
		this->lastFrameTicks = frameStart;

		this->handleEvents();
		this->update(dt);
		this->render();

		const Uint32 elapsed = SDL_GetTicks() - frameStart;
		if (elapsed < static_cast<Uint32>(MIN_FRAME_TIME)) {
			SDL_Delay(static_cast<Uint32>(MIN_FRAME_TIME) - elapsed);
		}
	}
}
void Game::handleEvents(){
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				this->isRunning = false;
				break;
			default:
				break;
		}
		//key presses/controller will be here
		
	}
}
void Game::update(float dt){
	//this->animTimer = 0.f;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	float speed = 200.f;
	this->velocityY += 800.f * dt;
	playerY += this->velocityY * dt;
	if(playerY > this->floorY){
		playerY = this->floorY;
		this->velocityY = 0;
	}
	if (keys[SDL_SCANCODE_LEFT]){
		playerX -= speed * dt;
		this->animTimer += dt;
	}
	if (keys[SDL_SCANCODE_RIGHT]){
		playerX += speed * dt;
		this->animTimer += dt;
	} 
	if (keys[SDL_SCANCODE_SPACE] && playerY >= this->floorY){
		this->velocityY = -400.f;
	}
	if (!keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT]) {
		this->animTimer = 0.f;
	 } 
	// if (keys[SDL_SCANCODE_DOWN])  playerY += speed * dt;
}
void Game::render(){
	SDL_SetRenderDrawColor(this->renderer, 92, 148, 252, 255);
	SDL_RenderClear(this->renderer);
	SDL_Rect player = { (int)playerX, (int)playerY, 36, 45 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Texture* currentTex = this->texStand;
	bool useStepSprite = fmod(animTimer, 0.3f) > 0.15f;
	if(useStepSprite){
		currentTex = this->texStep;
	}
	if (playerY < floorY) {
		currentTex = this->texJump;
	}
	

	SDL_RenderCopy(renderer, currentTex, NULL, &player);
	SDL_RenderPresent(this->renderer);
}
Game::~Game(){
	this->isRunning = false;
	if (this->renderer != nullptr) {
		SDL_DestroyTexture(texJump);
		SDL_DestroyTexture(texStand);
		SDL_DestroyTexture(texStep);
		SDL_DestroyRenderer(renderer);
		this->renderer = nullptr;
	}
	if (this->window != nullptr) {
		SDL_DestroyWindow(window);
		this->window = nullptr;
	}
	SDL_Quit();
}
