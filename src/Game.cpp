#include "Game.h"
#include <iostream>
#include <vector>
#include <cmath>

Game::Game(): window(nullptr), renderer(nullptr), isRunning(false), lastFrameTicks(0), playerX(48), playerY(0), texStand(nullptr), texStep(nullptr), texJump(nullptr), velocityY(0),animTimer(0.0),isFacingLeft(false){
	for(int i = 0; i < TILE_COUNT; i++){
		tileTextures[i] = nullptr;
	} 
}

//static enum colorsToIgnore = {"ff80c0"}

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

	for(int i = 0; i < TILE_COUNT; i++){
		if(tileFiles[i] == nullptr) continue;
		SDL_Surface* surf = SDL_LoadBMP(tileFiles[i]);
		if(surf == nullptr){
			std::cerr << "Failed to load tile " << i << ": " << SDL_GetError() << std::endl;
			continue;
		}
		SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0xFF, 0x80, 0xC0));
		tileTextures[i] = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
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

	int sh; //standing height of sprite
	SDL_QueryTexture(texStand, NULL, NULL, NULL, &sh);
	this->floorY = (720.f - TILE_SIZE) - sh;
	this->playerY = this->floorY;

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
	float speed = 400.f;
	this->velocityY += 800.f * dt;
	playerY += this->velocityY * dt;
	if(playerY > this->floorY){
		playerY = this->floorY;
		this->velocityY = 0;
	}
	if (keys[SDL_SCANCODE_LEFT]){
		if(this->isFacingLeft == false){
			this->isFacingLeft = true;
		}
		playerX -= speed * dt;
		this->animTimer += dt;
	}
	if (keys[SDL_SCANCODE_RIGHT]){
		if(this->isFacingLeft == true){
			this->isFacingLeft = false;
		}
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

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	int standW, standH, stepW, stepH, jumpW, jumpH;
	SDL_QueryTexture(texStand, NULL, NULL, &standW, &standH);
	SDL_QueryTexture(texStep, NULL, NULL, &stepW, &stepH);
	SDL_QueryTexture(texJump, NULL, NULL, &jumpW, &jumpH);
	int drawW = standW;
	int drawH = standH;
	SDL_Texture* currentTex = this->texStand;
	bool useStepSprite = fmod(animTimer, 0.3f) > 0.15f;
	
	if(useStepSprite){
		currentTex = this->texStep;
		drawW = stepW;
		drawH = stepH;
	}
	if (playerY < floorY) {
		currentTex = this->texJump;
		drawW = jumpW;
		drawH = jumpH;
	}
	SDL_Rect player = { (int)playerX, (int)playerY, drawW, drawH };


	SDL_RendererFlip flip = isFacingLeft ?  SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	SDL_RenderCopyEx(renderer, currentTex, NULL, &player, 0.0, NULL, flip);
	SDL_RenderPresent(this->renderer);
}
Game::~Game(){
	this->isRunning = false;
	if (this->renderer != nullptr) {
		for(int i = 0; i < TILE_COUNT; i++){
			if(tileTextures[i] != nullptr) SDL_DestroyTexture(tileTextures[i]);
		}
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
