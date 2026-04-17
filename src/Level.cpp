#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Level.h"
#include "Tiles.h"
#include <algorithm>

std::vector<std::vector<int>> bushSetOne = {
	{83,85,85,85,84},
	{83,86,87,85,84},
	{88,87,84,88,89},
	{0,88,89,0,0}
};
std::vector<std::vector<int>> bushSetTwo = { //[0][0] is 89 when standalone
	{86,87,85,84},
	{0,83,86,89},
	{0,88,89,0}
};
std::vector<std::vector<int>> smallestPossiblePipe = {
	{10,11},
	{8,9}
};

Level::Level() {}
Level::~Level() {}
//levelTiles has 27 inner vectors, with each vector holding 176 items.
/*
vect = {
{0...175} //vect[0]
...
{0...175} //vect[size() - 1]
}
*/
////for flat, first level is 176 tiles by 27 tiles. iWidth is how many tiles wide a row is, iHeight how tall. X and Y are where on map the sequence starts, in terms of tiles.
void Level::buildGround(int X, int Y, int gWidth, int gHeight) {  
	for(int i = 0; i < gWidth; i++) { //i goes across the columns, 
		for(int j = 0; j < gHeight; j++) { //j goes up each row
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
	for(int row = 0; row < bushSetOne.size(); row++){
		for(int col = 0; col < bushSetOne[row].size(); col++){
			levelTiles[1 + row][1 + col] = bushSetOne[row][col];
		}
	}
	for(int r = 0; r < bushSetTwo.size(); r++){
		for(int c = 0; c < bushSetTwo[r].size(); c++){
			if(bushSetTwo[r][c] != BLUESKY) levelTiles[1 + r][5 + c] = bushSetTwo[r][c];
		}
	}

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
// void Level::structPipe(int X, int Y, int shaftHeight) {
// 	if(shaftHeight < 2){
// 		//
// 	}
// 	for(int i = 0; i < iHeight; i++) {
// 		lMap[X][Y + i]->setBlockID(iLevelType == 0 ? 20 : iLevelType == 2 ? 97 : iLevelType == 4 ? 112 : iLevelType == 5 ? 136 : iLevelType == 3 ? 176 : iLevelType == 7 ? 172 : 30);
// 		lMap[X + 1][Y + i]->setBlockID(iLevelType == 0 ? 22 : iLevelType == 2 ? 99 : iLevelType == 4 ? 114 : iLevelType == 5 ? 138 : iLevelType == 3 ? 178 : iLevelType == 7 ? 174 : 32);
// 	}

// 	lMap[X][Y + iHeight]->setBlockID(iLevelType == 0 ? 21 : iLevelType == 2 ? 98 : iLevelType == 4 ? 113 : iLevelType == 5 ? 137 : iLevelType == 3 ? 177 : iLevelType == 7 ? 173 : 31);
// 	lMap[X + 1][Y + iHeight]->setBlockID(iLevelType == 0 ? 23 : iLevelType == 2 ? 100 : iLevelType == 4 ? 115 : iLevelType == 5 ? 139 : iLevelType == 3 ? 179 : iLevelType == 7 ? 175 : 33);
// };
