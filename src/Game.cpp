#include "Game.h"
#include <iostream>

Game::Game(): window(nullptr), renderer(nullptr), isRunning(false), lastFrameTicks(0), playerX(48), playerY(432){}

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
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	float speed = 200.f;
	if (keys[SDL_SCANCODE_LEFT])  playerX -= speed * dt;
	if (keys[SDL_SCANCODE_RIGHT]) playerX += speed * dt;
	if (keys[SDL_SCANCODE_UP])    playerY -= speed * dt;
	if (keys[SDL_SCANCODE_DOWN])  playerY += speed * dt;
}
void Game::render(){
	SDL_SetRenderDrawColor(this->renderer, 92, 148, 252, 255);
	SDL_RenderClear(this->renderer);
	SDL_Rect player = { (int)playerX, (int)playerY, 48, 144 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &player);
	SDL_RenderPresent(this->renderer);
}
Game::~Game(){
	this->isRunning = false;
	if (this->renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
		this->renderer = nullptr;
	}
	if (this->window != nullptr) {
		SDL_DestroyWindow(window);
		this->window = nullptr;
	}
	SDL_Quit();
}
