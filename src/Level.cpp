#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Level.h"
#include "Tiles.h"
#include "Shapes.h"
#include "Constants.h"
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
	this->levelTiles[Ypos][Xpos] = t;
}
int Level::tileAt(int col, int row) const{
	if(row > this->levelTiles.size() - 1 || col > this->levelTiles[row].size() - 1 || row < 0 || col < 0){
		std::cout << "OUT OF BOUNDS" << std::endl;			
		return BLUESKY;
	}
	std::cout << this->levelTiles[row][col] << std::endl;
	return this->levelTiles[row][col];
}
int Level::colFromX(float screenX)const{
	return std::floor(screenX / TILE_SIZE);
}
int Level::rowFromY(float screenY)const{
	// Row r spans screen Y in [GAME_HEIGHT - TILE_SIZE*(r+1), GAME_HEIGHT - TILE_SIZE*r).
	// So r is the largest int with TILE_SIZE*r < GAME_HEIGHT - screenY, i.e. ceil(A/TS) - 1.
	return (int)std::ceil((GAME_HEIGHT - screenY) / (float)TILE_SIZE) - 1;
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
			this->levelTiles[ty][tx] = platfOrCloud[row][col];
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
	buildGround(72, 0, 22, 1);
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
	placeSingleTileObject(BUSH_ALONE,74,1);
	placeSingleTileObject(BUSH_ALONE,75,1);
	placeSingleTileObject(BUSH_ALONE,76,1);
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
	shapes::shadowfy(bluePlatform,"ground","TRANSPARENT");
	placeRectangularShape(bluePlatform, 17, 1);
	placeRectangularShape(pinkPlatform, 15, 1); // bottom-left corner lands at (X=15, Y=2)
	TileMatrix firstCloud = shapes::buildMultiFaceCloud(2);
	placeRectangularShape(firstCloud, 7, 8);
	TileMatrix firstPipe = shapes::buildPipe(1);
	placeRectangularShape(firstPipe,22, 1);
	TileMatrix firstSingleCloud = shapes::oneFaceCloudA;
	placeRectangularShape(firstSingleCloud,20, 9);
	TileMatrix greenPlatform = shapes::buildScrewPlatform("GREEN",3,1);
	shapes::shadowfy(greenPlatform,"ground","PINK");
	TileMatrix secondPinkPlatform = shapes::buildScrewPlatform("PINK",2,3);
	shapes::shadowfy(secondPinkPlatform,"ground","WHITE");
	TileMatrix firstWhitePlatform = shapes::buildScrewPlatform("WHITE",2,5);
	shapes::shadowfy(firstWhitePlatform,"ground","TRANSPARENT");
	placeRectangularShape(firstWhitePlatform,32,1);
	placeRectangularShape(secondPinkPlatform,29,1);
	placeRectangularShape(greenPlatform,25,1);
	TileMatrix secondGreenPlatform = shapes::buildScrewPlatform("GREEN",4,0);
	shapes::shadowfy(secondGreenPlatform,"ground","TRANSPARENT");
	placeRectangularShape(secondGreenPlatform,32,1);
	//pink box in sky is at x=82 tiles, y = 14 tiles 
	TileMatrix pinkPlatformInSky = shapes::buildScrewPlatform("PINK",2,0);
	shapes::shadowfy(pinkPlatformInSky,"insky","TRANSPARENT");
	placeRectangularShape(pinkPlatformInSky,82,14);
	placeRectangularShape(shapes::bushSetThree,51,2);
	// tileAt(5,9); //0
	// tileAt(0,0); //stoneleft
	// tileAt(25,1);
	// tileAt(30,1);
	// tileAt(0,176);
	// tileAt(2,5);
	// tileAt(46,2);
	// tileAt(51,3);
	// tileAt(44,4); //should be a question block!
	// int demo1 = this->rowFromY(704.0f);
	// std::cout << "rowFromY(704)=" << demo1 << " (expect 0)" << std::endl;
	// int demo2 = this->rowFromY(688.0f);
	// std::cout << "rowFromY(688)=" << demo2 << " (expect 1)" << std::endl;
	// int demo3 = this->rowFromY(0.0f);
	// std::cout << "rowFromY(0)="   << demo3 << " (expect 44)" << std::endl;
	// int demo4 = this->colFromX(144.0f);
	// std::cout << "colFromX(144)=" << demo4 << " (expect 9)" << std::endl;
	// int demo5 = this->rowFromY(720.0f);
	// std::cout << "rowFromY(720)=" << demo5 << " (expect -1)" << std::endl;

	// int demo6 = this->colFromX(0.0f);
	// std::cout << "colFromX(0)="    << demo6 << " (expect 0)"   << std::endl;
	// int demo7 = this->colFromX(15.0f);
	// std::cout << "colFromX(15)="   << demo7 << " (expect 0)"   << std::endl;
	// int demo8 = this->colFromX(16.0f);
	// std::cout << "colFromX(16)="   << demo8 << " (expect 1)"   << std::endl;
	// int demo9 = this->colFromX(-1.0f);
	// std::cout << "colFromX(-1)="   << demo9 << " (expect -1)"  << std::endl;
	// int demo10 = this->colFromX(2816.0f);
	// std::cout << "colFromX(2816)=" << demo10 << " (expect 176)" << std::endl;
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
