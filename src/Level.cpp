#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Level.h"
#include "Tiles.h"
#include "Shapes.h"
#include <algorithm>
#include <iostream>

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

void Level::placeRectangularShape(const TileMatrix& platfOrCloud, int Xpos, int Ypos){
	// platf is row-major: platf[row(Y)][col(X)]. row 0 is the bottom row.
	// -1 means "leave whatever is already there" (unfilled cell from buildScrewPlatform).
	for(size_t row = 0; row < platfOrCloud.size(); row++){
		for(size_t col = 0; col < platfOrCloud[row].size(); col++){
			if(platfOrCloud[row][col] == -1){
				continue;
			}
			int ty = Ypos + (int)row;
			int tx = Xpos + (int)col;
			if(ty < 0 || ty >= (int)levelTiles.size()){
				continue;
			}
			if(tx < 0 || tx >= (int)levelTiles[ty].size()){
				continue;
			}
			levelTiles[ty][tx] = platfOrCloud[row][col];
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

void Level::level_1_1(){
	static int LEVEL1_1_WIDTH = 176;
	static int LEVEL1_1_HEIGHT = 27;
	levelTiles.assign(LEVEL1_1_HEIGHT, std::vector<int>(LEVEL1_1_WIDTH, BLUESKY));
	buildGround(0, 0, 39, 1);
	buildGround(39, 0, 29, 2);
	for(size_t row = 0; row < shapes::bushSetOne.size(); row++){
		for(size_t col = 0; col < shapes::bushSetOne[row].size(); col++){
			levelTiles[1 + row][1 + col] = shapes::bushSetOne[row][col];
		}
	}
	for(size_t r = 0; r < shapes::bushSetTwo.size(); r++){
		for(size_t c = 0; c < shapes::bushSetTwo[r].size(); c++){
			if(shapes::bushSetTwo[r][c] != BLUESKY) levelTiles[1 + r][5 + c] = shapes::bushSetTwo[r][c];
		}
	}
	placeSingleTileObject(BUSH_ALONE,10,1);
	placeSingleTileObject(BUSH_ALONE,11,1);
	placeSingleTileObject(BUSH_ALONE,12,1);
	placeSingleTileObject(BUSH_ALONE,42,2);
	placeSingleTileObject(BUSH_ALONE,43,2);
	placeSingleTileObject(BUSH_ALONE,44,2);
	placeSingleTileObject(BUSH_ALONE,45,2);
	placeSingleTileObject(BUSH_ALONE,46,2);
	placeSingleTileObject(BUSH_ALONE,47,2);
	placeSingleTileObject(QUESTION_BLOCK_1,11,4);
	placeSingleTileObject(QUESTION_BLOCK_1,12,4);
	placeSingleTileObject(QUESTION_BLOCK_1,14,7);
	placeSingleTileObject(QUESTION_BLOCK_1,15,7);
	placeSingleTileObject(QUESTION_BLOCK_1,26,6);
	placeSingleTileObject(QUESTION_BLOCK_1,41,2);
	placeSingleTileObject(QUESTION_BLOCK_1,44,4);

	TileMatrix pinkPlatform = shapes::buildScrewPlatform("PINK", 1, 1);
	shapes::shadowfy(pinkPlatform,"ground","BLUE");
	TileMatrix bluePlatform = shapes::buildScrewPlatform("BLUE", 1, 3);
	placeRectangularShape(bluePlatform, 17, 1);
	placeRectangularShape(pinkPlatform, 15, 1); // bottom-left corner lands at (X=15, Y=2)
	TileMatrix firstCloud = shapes::buildMultiFaceCloud(2);
	placeRectangularShape(firstCloud, 7, 8);
	TileMatrix firstPipe = shapes::buildPipe(1);
	placeRectangularShape(firstPipe,22, 1);
	TileMatrix firstSingleCloud = shapes::oneFaceCloudA;
	placeRectangularShape(firstSingleCloud,20, 9);
	TileMatrix greenPlatform = shapes::buildScrewPlatform("GREEN",3,1);
	TileMatrix secondPinkPlatform = shapes::buildScrewPlatform("PINK",2,3);
	TileMatrix firstWhitePlatform = shapes::buildScrewPlatform("WHITE",2,5);
	placeRectangularShape(firstWhitePlatform,32,1);
	placeRectangularShape(secondPinkPlatform,29,1);
	placeRectangularShape(greenPlatform,25,1);
	TileMatrix secondGreenPlatform = shapes::buildScrewPlatform("GREEN",4,0);
	placeRectangularShape(secondGreenPlatform,32,1);
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
