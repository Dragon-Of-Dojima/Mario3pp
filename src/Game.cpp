#include "Game.h"
#include <iostream>

Game::Game(void): window(nullptr), renderer(nullptr), isRunning(false){}

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

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				this->isRunning = false;
			}
		}

		SDL_SetRenderDrawColor(this->renderer, 92, 148, 252, 255);
		SDL_RenderClear(this->renderer);
		SDL_RenderPresent(this->renderer);

		const Uint32 elapsed = SDL_GetTicks() - frameStart;
		if (elapsed < static_cast<Uint32>(MIN_FRAME_TIME)) {
			SDL_Delay(static_cast<Uint32>(MIN_FRAME_TIME) - elapsed);
		} 
	}
}
void Game::handleEvents(){

}
void Game::update(){

}
void Game::render(){

}
Game::~Game(void){
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
