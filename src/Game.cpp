#include "Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

Game::Game(): window(nullptr), renderer(nullptr), isRunning(false), lastFrameTicks(0){
	for(int i = 0; i < TILE_COUNT; i++){
		tileTextures[i] = nullptr;
	} 
}

//static enum colorsToIgnore = {"#ff80c0"}

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

	SDL_Surface* iconSurf = IMG_Load("app.png");
	if (iconSurf != nullptr) {
		SDL_SetWindowIcon(this->window, iconSurf);
		SDL_FreeSurface(iconSurf);
	} else {
		std::cerr << "Failed to load app.png: " << IMG_GetError() << std::endl;
	}

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == nullptr) {
		std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(this->window);
		this->window = nullptr;
		SDL_Quit();
		return false;
	}

	level.level_1_1();

	static const char* tileFiles[TILE_COUNT] = {};
	tileFiles[STONE_LEFT]        = "public/images/tiles/mainTileLeftEdge.bmp";
	tileFiles[STONE_INNER]       = "public/images/tiles/mainTileInner.bmp";
	tileFiles[STONE_RIGHT]       = "public/images/tiles/mainTileRightEdge.bmp";
	tileFiles[STONE_LEFT_RISER]  = "public/images/tiles/mainTileLeftSide.bmp";
	tileFiles[STONE_INNER_RISER] = "public/images/tiles/mainTileCenterNotTop.bmp";
	tileFiles[STONE_RIGHT_RISER] = "public/images/tiles/mainTileRightSide.bmp";
	tileFiles[PATTERN_BLOCK] = "public/images/tiles/patternBoxReg.bmp";
	tileFiles[CHECKERBUSH_INNER] = "public/images/tiles/checkerBushInner.bmp";
	tileFiles[CHECKERBUSH_LEFT_EDGE] = "public/images/tiles/checkerBushLeftEdge.bmp";
	tileFiles[CHECKERBUSH_RIGHT_EDGE] = "public/images/tiles/checkerBushRightEdge.bmp";
	tileFiles[CHECKERBUSH_TOP_LEFT] = "public/images/tiles/checkerBushTopLeft.bmp";
	tileFiles[CHECKERBUSH_TOP_RIGHT] = "public/images/tiles/checkerBushTopRight.bmp";
	tileFiles[CHECKERBUSH_TOP_LEFT_WITH_BEHIND] = "public/images/tiles/checkerBushTopLeftWithBehind.bmp";
	tileFiles[CHECKERBUSH_TOP_RIGHT_WITH_BEHIND] = "public/images/tiles/checkerBushTopRightWithBehind.bmp";
	tileFiles[BUSH_ALONE] = "public/images/tiles/smallBushSingleton.bmp";
	tileFiles[QUESTION_BLOCK_1] = "public/images/tiles/itemBlock1.bmp";
	tileFiles[PINK_PLATFORM_LEFT_TOP_CORNER]     = "public/images/tiles/pinkPlatformLeftTopCorner.bmp";
	tileFiles[PINK_PLATFORM_LEFT_EDGE]           = "public/images/tiles/pinkPlatformLeftEdge.bmp";
	tileFiles[PINK_PLATFORM_LEFT_BOTTOM_CORNER]  = "public/images/tiles/pinkPlatformLeftBottomCorner.bmp";
	tileFiles[PINK_PLATFORM_TOP_INNER]           = "public/images/tiles/pinkPlatformTopInner.bmp";
	tileFiles[PINK_PLATFORM_INNER]               = "public/images/tiles/pinkPlatformInner.bmp";
	tileFiles[PINK_PLATFORM_BOTTOM_INNER]        = "public/images/tiles/pinkPlatformBottomInner.bmp";
	tileFiles[PINK_PLATFORM_RIGHT_TOP_CORNER]    = "public/images/tiles/pinkPlatformRightTopCorner.bmp";
	tileFiles[PINK_PLATFORM_RIGHT_EDGE]          = "public/images/tiles/pinkPlatformRightEdge.bmp";
	tileFiles[PINK_PLATFORM_RIGHT_BOTTOM_CORNER] = "public/images/tiles/pinkPlatformRightBottomCorner.bmp";
	tileFiles[PINK_PLATFORM_SHADOW_TOP]          = "public/images/tiles/pinkPlatformShadowTop.bmp";
	tileFiles[PINK_PLATFORM_SHADOW_EDGE]         = "public/images/tiles/pinkPlatformShadowEdge.bmp";
	tileFiles[PINK_PLATFORM_SHADOW_BOTTOM]       = "public/images/tiles/pinkPlatformShadowBottom.bmp";
	tileFiles[BLUE_PLATFORM_LEFT_TOP_CORNER]     = "public/images/tiles/bluePlatformLeftTopCorner.bmp";
	tileFiles[BLUE_PLATFORM_LEFT_EDGE]           = "public/images/tiles/bluePlatformLeftEdge.bmp";
	tileFiles[BLUE_PLATFORM_LEFT_BOTTOM_CORNER]  = "public/images/tiles/bluePlatformLeftBottomCorner.bmp";
	tileFiles[BLUE_PLATFORM_TOP_INNER]           = "public/images/tiles/bluePlatformTopInner.bmp";
	tileFiles[BLUE_PLATFORM_INNER]               = "public/images/tiles/bluePlatformInner.bmp";
	tileFiles[BLUE_PLATFORM_BOTTOM_INNER]        = "public/images/tiles/bluePlatformBottomInner.bmp";
	tileFiles[BLUE_PLATFORM_RIGHT_TOP_CORNER]    = "public/images/tiles/bluePlatformRightTopCorner.bmp";
	tileFiles[BLUE_PLATFORM_RIGHT_EDGE]          = "public/images/tiles/bluePlatformRightEdge.bmp";
	tileFiles[BLUE_PLATFORM_RIGHT_BOTTOM_CORNER] = "public/images/tiles/bluePlatformRightBottomCorner.bmp";
	tileFiles[BLUE_PLATFORM_SHADOW_TOP]          = "public/images/tiles/bluePlatformShadowTop.bmp";
	tileFiles[BLUE_PLATFORM_SHADOW_EDGE]         = "public/images/tiles/bluePlatformShadowEdge.bmp";
	tileFiles[BLUE_PLATFORM_SHADOW_BOTTOM]       = "public/images/tiles/bluePlatformShadowBottom.bmp";
	tileFiles[GREEN_PLATFORM_LEFT_TOP_CORNER]     = "public/images/tiles/greenPlatformLeftTopCorner.bmp";
	tileFiles[GREEN_PLATFORM_LEFT_EDGE]           = "public/images/tiles/greenPlatformLeftEdge.bmp";
	tileFiles[GREEN_PLATFORM_LEFT_BOTTOM_CORNER]  = "public/images/tiles/greenPlatformLeftBottomCorner.bmp";
	tileFiles[GREEN_PLATFORM_TOP_INNER]           = "public/images/tiles/greenPlatformTopInner.bmp";
	tileFiles[GREEN_PLATFORM_INNER]               = "public/images/tiles/greenPlatformInner.bmp";
	tileFiles[GREEN_PLATFORM_BOTTOM_INNER]        = "public/images/tiles/greenPlatformBottomInner.bmp";
	tileFiles[GREEN_PLATFORM_RIGHT_TOP_CORNER]    = "public/images/tiles/greenPlatformRightTopCorner.bmp";
	tileFiles[GREEN_PLATFORM_RIGHT_EDGE]          = "public/images/tiles/greenPlatformRightEdge.bmp";
	tileFiles[GREEN_PLATFORM_RIGHT_BOTTOM_CORNER] = "public/images/tiles/greenPlatformRightBottomCorner.bmp";
	tileFiles[GREEN_PLATFORM_SHADOW_TOP]          = "public/images/tiles/greenPlatformShadowTop.bmp";
	tileFiles[GREEN_PLATFORM_SHADOW_EDGE]         = "public/images/tiles/greenPlatformShadowEdge.bmp";
	tileFiles[GREEN_PLATFORM_SHADOW_BOTTOM]       = "public/images/tiles/greenPlatformShadowBottom.bmp";
	tileFiles[WHITE_PLATFORM_LEFT_TOP_CORNER]     = "public/images/tiles/whitePlatformLeftTopCorner.bmp";
	tileFiles[WHITE_PLATFORM_LEFT_EDGE]           = "public/images/tiles/whitePlatformLeftEdge.bmp";
	tileFiles[WHITE_PLATFORM_LEFT_BOTTOM_CORNER]  = "public/images/tiles/whitePlatformLeftBottomCorner.bmp";
	tileFiles[WHITE_PLATFORM_TOP_INNER]           = "public/images/tiles/whitePlatformTopInner.bmp";
	tileFiles[WHITE_PLATFORM_INNER]               = "public/images/tiles/whitePlatformInner.bmp";
	tileFiles[WHITE_PLATFORM_BOTTOM_INNER]        = "public/images/tiles/whitePlatformBottomInner.bmp";
	tileFiles[WHITE_PLATFORM_RIGHT_TOP_CORNER]    = "public/images/tiles/whitePlatformRightTopCorner.bmp";
	tileFiles[WHITE_PLATFORM_RIGHT_EDGE]          = "public/images/tiles/whitePlatformRightEdge.bmp";
	tileFiles[WHITE_PLATFORM_RIGHT_BOTTOM_CORNER] = "public/images/tiles/whitePlatformRightBottomCorner.bmp";
	tileFiles[WHITE_PLATFORM_SHADOW_TOP]          = "public/images/tiles/whitePlatformShadowTop.bmp";
	tileFiles[WHITE_PLATFORM_SHADOW_EDGE]         = "public/images/tiles/whitePlatformShadowEdge.bmp";
	tileFiles[WHITE_PLATFORM_SHADOW_BOTTOM]       = "public/images/tiles/whitePlatformShadowBottom.bmp";
	tileFiles[CLOUD_MULTI_HEAD]           = "public/images/tiles/eyeCloudMultiInnerHead.bmp";
	tileFiles[CLOUD_MULTI_LEFT_SHOULDER]  = "public/images/tiles/eyeCloudMultiTopLeft.bmp";
	tileFiles[CLOUD_MULTI_RIGHT_SHOULDER] = "public/images/tiles/eyeCloudMultiTopRight.bmp";
	tileFiles[CLOUD_MULTI_BOTTOM_LEFT]    = "public/images/tiles/eyeCloudMultiBottomLeft.bmp";
	tileFiles[CLOUD_MULTI_BOTTOM_CENTER]  = "public/images/tiles/eyeCloudMultiBottomCenter.bmp";
	tileFiles[CLOUD_MULTI_BOTTOM_RIGHT]   = "public/images/tiles/eyeCloudMultiBottomRight.bmp";
	tileFiles[PIPE_TOP_LEFT]   = "public/images/tiles/pipeGreenBlackTopLeft.bmp";
	tileFiles[PIPE_TOP_RIGHT]  = "public/images/tiles/pipeGreenBlackTopRight.bmp";
	tileFiles[PIPE_LEFT_EDGE]  = "public/images/tiles/pipeGreenBlackBottomLeft.bmp";
	tileFiles[PIPE_RIGHT_EDGE] = "public/images/tiles/pipeGreenBlackBottomRight.bmp";
	tileFiles[SHADOW_OVER_TRANSPARENT_BL]        = "public/images/tiles/shadowBottomLeft.bmp";
	tileFiles[SHADOW_OVER_TRANSPARENT_BHE]        = "public/images/tiles/shadowBottom.bmp";
	tileFiles[SHADOW_OVER_TRANSPARENT_BR]        = "public/images/tiles/shadowBottomRight.bmp";
	tileFiles[SHADOW_OVER_TRANSPARENT_RIGHTEDGE] = "public/images/tiles/shadowRightEdge.bmp";
	tileFiles[SHADOW_OVER_TRANSPARENT_TOP_RIGHT_CORNER] = "public/images/tiles/shadowRightCorner.bmp";

	for(int i = 0; i < TILE_COUNT; i++){
		if(tileFiles[i] == nullptr){
			continue;
		}
		SDL_Surface* surf = SDL_LoadBMP(tileFiles[i]);
		if(surf == nullptr){
			std::cerr << "Failed to load tile " << i << ": " << SDL_GetError() << std::endl;
			continue;
		}
		SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0xFF, 0x80, 0xC0));
		tileTextures[i] = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
	}

	if(!player.loadTextures(this->renderer)){
		return false;
	}
	const float floorY = (720.f - TILE_SIZE) - player.getStandHeight();
	player.spawn(48.f, floorY);

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
	player.handleInput(keys);
	player.update(dt,level);
}
void Game::render(){
	SDL_SetRenderDrawColor(this->renderer, 0xAF, 0xF9, 0xF0, 0xFF);
	SDL_RenderClear(this->renderer);

	const std::vector<std::vector<int>>& tiles = level.getTiles();
	for(int row = 0; row < (int)tiles.size(); row++){
		for(int col = 0; col < (int)tiles[row].size(); col++){
			int id = tiles[row][col];
			if(id == BLUESKY || tileTextures[id] == nullptr) continue;
			SDL_Rect dst = { col * TILE_SIZE, 720 - TILE_SIZE * (row + 1), TILE_SIZE, TILE_SIZE };
			SDL_RenderCopy(renderer, tileTextures[id], NULL, &dst);
		}
	}

	player.render(this->renderer);
	SDL_RenderPresent(this->renderer);
}
Game::~Game(){
	this->isRunning = false;
	if (this->renderer != nullptr) {
		player.destroyTextures();
		for(int i = 0; i < TILE_COUNT; i++){
			if(tileTextures[i] != nullptr) SDL_DestroyTexture(tileTextures[i]);
		}
		SDL_DestroyRenderer(renderer);
		this->renderer = nullptr;
	}
	if (this->window != nullptr) {
		SDL_DestroyWindow(window);
		this->window = nullptr;
	}
	SDL_Quit();
}
