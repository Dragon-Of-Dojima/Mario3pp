#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Level.h"
#include "Tiles.h"
#include <algorithm>

Level::Level() {}
Level::~Level() {}
////for flat, first level is 176 tiles by 27 tiles. iWidth is how many tiles wide a row is, iHeight how tall. X and Y are where on map the sequence starts, in terms of tiles.
void Level::buildGround(int X, int Y, int gWidth, int gHeight) {  
	for(int i = 0; i < gWidth; i++) {
		for(int j = 0; j < gHeight; j++) {
			Tiles tile;
			if(j == gHeight - 1) {
				if(i == 0){
					tile = STONE_LEFT;
				}
				else if(i == gWidth - 1){
					tile = STONE_RIGHT;
				} 
				else{
					tile = STONE_INNER;
				}
			} else {
				if(i == 0){
					tile = STONE_LEFT_RISER;
				}
				else if(i == gWidth - 1){
					tile = STONE_RIGHT_RISER;
				} 
				else{
					tile = STONE_INNER_RISER;
				}
			}
			levelTiles[Y + j][X + i] = tile; //[Y + i][X + j]to account for what's already at X and Y
		}
	}
}
void Level::level_1_1(){
	static int LEVEL1_1_WIDTH = 176;
	static int LEVEL1_1_HEIGHT = 27;
	levelTiles.assign(LEVEL1_1_HEIGHT, std::vector<int>(LEVEL1_1_WIDTH, BLUESKY));
	buildGround(0, 0, 29, 1);
	buildGround(29, 0, 29, 2);
	buildSteps(30, 2, 8, true);

}
//below is what Jakowski uses for the triangles of blocks. bDir is true for acuse, false for obtuse.
//X is where on map it starts, y is where on Y axis it starts, 4 is height in tiles at end 
void Level::buildSteps(int X, int Y, int iSize, bool accOrObt) { //for sloped
	
	for(int i = 0, k = 1; i < iSize; i++) {
		for(int j = 0; j < k; j++) {
			if(accOrObt){
				levelTiles[Y + j][X + i] = PATTERN_BLOCK;
			}else{
				levelTiles[Y + j][X + iSize - 1 - i] = PATTERN_BLOCK;
			}
		}
		++k;
	}
	 
}
// is identical to structGND but uses different tiles
// void Map::structGND2(int X, int Y, int iWidth, int iHeight) {
// 	for(int i = 0; i < iWidth; i++) {
// 		for(int j = 0; j < iHeight; j++) {
// 			lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 25 : iLevelType == 3 ? 167 : 27);
// 		}
// 	}
// }