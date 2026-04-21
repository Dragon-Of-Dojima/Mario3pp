#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Level.h"
#include "Tiles.h"
#include <algorithm>
#include <iostream>

// Tile ID legend (see Tiles.h). Re-sync these numbers if the enum is reordered.
//   0 = BLUESKY
//   8 = PIPE_TOP_LEFT                      9 = PIPE_TOP_RIGHT
//  10 = PIPE_LEFT_EDGE                    11 = PIPE_RIGHT_EDGE
//  95 = CHECKERBUSH_LEFT_EDGE             96 = CHECKERBUSH_RIGHT_EDGE
//  97 = CHECKERBUSH_INNER
//  98 = CHECKERBUSH_TOP_LEFT_WITH_BEHIND  99 = CHECKERBUSH_TOP_RIGHT_WITH_BEHIND
// 100 = CHECKERBUSH_TOP_LEFT             101 = CHECKERBUSH_TOP_RIGHT
std::vector<std::vector<int>> bushSetOne = {
	{95,97,97,97,96},
	{95,98,99,97,96},
	{100,99,96,100,101},
	{0,100,101,0,0}
};
std::vector<std::vector<int>> bushSetTwo = { //[0][0] is 101 when standalone
	{98,99,97,96},
	{0,95,98,101},
	{0,100,101,0}
};
std::vector<std::vector<int>> bushSetTwoHalf = { //[0][0] is 101 when standalone
	{101,99,97,96},
	{0,95,98,101},
	{0,100,101,0}
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
void Level::placeSingleTileObject(Tiles t, int Xpos, int Ypos){ //passing a pointer for T would be bigger bc an int is 4B, pointer 8B
	levelTiles[Ypos][Xpos] = t;
}
std::vector<std::vector<int>> Level::buildScrewPlatform(std::string color, int tilesXAxisBtwn, int tilesYAxisBtwn){
	
	// int tilesToUse[] = {}; bad bc you can't reassign a raw C array in C++
	std::vector<int> tilesToUse = {};
	// Row-major to match levelTiles: outer = Y (rows), inner = X (cols).
	std::vector<std::vector<int>> platformMatrix(tilesYAxisBtwn + 2, std::vector<int>(tilesXAxisBtwn + 2,-1));
	if(color == "PINK"){
		tilesToUse = {
			PINK_PLATFORM_LEFT_TOP_CORNER,    PINK_PLATFORM_TOP_INNER,       PINK_PLATFORM_RIGHT_TOP_CORNER,
			PINK_PLATFORM_LEFT_EDGE,          PINK_PLATFORM_INNER,           PINK_PLATFORM_RIGHT_EDGE,
			PINK_PLATFORM_LEFT_BOTTOM_CORNER, PINK_PLATFORM_BOTTOM_INNER,    PINK_PLATFORM_RIGHT_BOTTOM_CORNER,
			PINK_PLATFORM_SHADOW_TOP,         PINK_PLATFORM_SHADOW_EDGE,     PINK_PLATFORM_SHADOW_BOTTOM
		};
	}else if(color == "BLUE"){
		tilesToUse = {
			BLUE_PLATFORM_LEFT_TOP_CORNER,    BLUE_PLATFORM_TOP_INNER,       BLUE_PLATFORM_RIGHT_TOP_CORNER,
			BLUE_PLATFORM_LEFT_EDGE,          BLUE_PLATFORM_INNER,           BLUE_PLATFORM_RIGHT_EDGE,
			BLUE_PLATFORM_LEFT_BOTTOM_CORNER, BLUE_PLATFORM_BOTTOM_INNER,    BLUE_PLATFORM_RIGHT_BOTTOM_CORNER,
			BLUE_PLATFORM_SHADOW_TOP,         BLUE_PLATFORM_SHADOW_EDGE,     BLUE_PLATFORM_SHADOW_BOTTOM
		};
	}else if(color == "WHITE"){
		tilesToUse = {
			WHITE_PLATFORM_LEFT_TOP_CORNER,    WHITE_PLATFORM_TOP_INNER,      WHITE_PLATFORM_RIGHT_TOP_CORNER,
			WHITE_PLATFORM_LEFT_EDGE,          WHITE_PLATFORM_INNER,          WHITE_PLATFORM_RIGHT_EDGE,
			WHITE_PLATFORM_LEFT_BOTTOM_CORNER, WHITE_PLATFORM_BOTTOM_INNER,   WHITE_PLATFORM_RIGHT_BOTTOM_CORNER,
			WHITE_PLATFORM_SHADOW_TOP,         WHITE_PLATFORM_SHADOW_EDGE,    WHITE_PLATFORM_SHADOW_BOTTOM
		};
	}else{
		tilesToUse = {
			GREEN_PLATFORM_LEFT_TOP_CORNER,    GREEN_PLATFORM_TOP_INNER,      GREEN_PLATFORM_RIGHT_TOP_CORNER,
			GREEN_PLATFORM_LEFT_EDGE,          GREEN_PLATFORM_INNER,          GREEN_PLATFORM_RIGHT_EDGE,
			GREEN_PLATFORM_LEFT_BOTTOM_CORNER, GREEN_PLATFORM_BOTTOM_INNER,   GREEN_PLATFORM_RIGHT_BOTTOM_CORNER,
			GREEN_PLATFORM_SHADOW_TOP,         GREEN_PLATFORM_SHADOW_EDGE,    GREEN_PLATFORM_SHADOW_BOTTOM
		};
	}
	// i = X (column), j = Y (row). platformMatrix is [row][col] == [j][i].
	for(int j = 0; j < platformMatrix.size(); j++){           // rows (Y)
		for(int i = 0; i < platformMatrix[0].size(); i++){    // cols (X)
			if(j == 0){ //bottom row
				if(i == 0){ //left corner
					platformMatrix[j][i] = tilesToUse[6];
				}else if(i == platformMatrix[0].size() - 1){ //right corner
					platformMatrix[j][i] = tilesToUse[8];
				}
				else{
					if(tilesXAxisBtwn > 0){
						platformMatrix[j][i] = tilesToUse[7];
					}
				}
			}
			else if(j < platformMatrix.size() - 1){
				//middle rows (if any)
					if(i == 0){ //left edge
						platformMatrix[j][i] = tilesToUse[3];
					}
					else if(i == platformMatrix[0].size() - 1){// right edge
						platformMatrix[j][i] = tilesToUse[5];
					}else{
						if(tilesXAxisBtwn > 0){
							platformMatrix[j][i] = tilesToUse[4];
						}
				}
			}
			else if(j == platformMatrix.size() - 1){ //top row
				if(i == 0){ //left corner
					platformMatrix[j][i] = tilesToUse[0];
				}
				else if(i == platformMatrix[0].size() - 1){ //right corner
					platformMatrix[j][i] = tilesToUse[2];
				}else{
					if(tilesXAxisBtwn > 0){
						platformMatrix[j][i] = tilesToUse[1];
					}
				}
			}
		}
	} 
	
	std::cout << "platformMatrix (" << platformMatrix.size()
	          << " x " << (platformMatrix.empty() ? 0 : platformMatrix[0].size())
	          << "):\n";
	for (size_t row = 0; row < platformMatrix.size(); ++row) {
		std::cout << "  [";
		for (size_t col = 0; col < platformMatrix[row].size(); ++col) {
			std::cout << platformMatrix[row][col];
			if (col + 1 < platformMatrix[row].size()) std::cout << ", ";
		}
		std::cout << "]\n";
	}
	std::cout.flush();

	return platformMatrix;
}
std::vector<std::vector<int>> Level::buildMultiFaceCloud(int faces){
	std::vector<std::vector<int>> cloudToRet(faces,std::vector<int>(faces+2,-1)); 
}
void Level::placeScrewPlatform(const std::vector<std::vector<int>>& platf, int Xpos, int Ypos){
	// platf is row-major: platf[row(Y)][col(X)]. row 0 is the bottom row.
	// -1 means "leave whatever is already there" (unfilled cell from buildScrewPlatform).
	for(int row = 0; row < platf.size(); row++){
		for(int col = 0; col < platf[row].size(); col++){
			if(platf[row][col] == -1){
				continue;
			}
			int ty = Ypos + row;
			int tx = Xpos + col;
			if(ty < 0 || ty >= (int)levelTiles.size()){
				continue;
			} 
			if(tx < 0 || tx >= (int)levelTiles[ty].size()){
				continue;
			} 
			levelTiles[ty][tx] = platf[row][col];
		}
	}
}
void Level::level_1_1(){
	static int LEVEL1_1_WIDTH = 176;
	static int LEVEL1_1_HEIGHT = 27;
	levelTiles.assign(LEVEL1_1_HEIGHT, std::vector<int>(LEVEL1_1_WIDTH, BLUESKY));
	buildGround(0, 0, 39, 1);
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
	placeSingleTileObject(BUSH_ALONE,10,1);
	placeSingleTileObject(BUSH_ALONE,11,1);
	placeSingleTileObject(BUSH_ALONE,12,1);
	placeSingleTileObject(QUESTION_BLOCK_1,11,4);
	placeSingleTileObject(QUESTION_BLOCK_1,12,4);
	placeSingleTileObject(QUESTION_BLOCK_1,14,7);
	placeSingleTileObject(QUESTION_BLOCK_1,15,7);

	std::vector<std::vector<int>> pinkPlatform = buildScrewPlatform("PINK", 1, 1);
	std::vector<std::vector<int>> bluePlatform = buildScrewPlatform("BLUE",1, 3);
	placeScrewPlatform(bluePlatform, 17, 1);
	placeScrewPlatform(pinkPlatform, 15, 1); // bottom-left corner lands at (X=15, Y=2)
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
