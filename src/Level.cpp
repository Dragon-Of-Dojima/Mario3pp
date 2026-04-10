#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
////for flat, first level is 176 tiles by 27 tiles. iWidth is how many tiles wide a row is, iHeight how tall. X and Y are where on map the sequence starts, in terms of tiles.
void Level::buildGround(int X, int Y, int iWidth, int iHeight) {  
	for(int i = 0; i < iWidth; i++) {
		for(int j = 0; j < iHeight; j++) {
			groundLevelTilesMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 1 : iLevelType == 1 ? 26 : iLevelType == 2 ? 92 : iLevelType == 6 ? 166 : iLevelType == 7 ? 181 : 75);
		}
	}
}
//below is what Jakowski uses for the triangles of blocks. bDir is true for acuse, false for obtuse.
//X is where on map it starts, y is where on Y axis it starts, 4 is height in tiles at end 
// void Map::structGND2(int X, int Y, int iSize, bool bDir) { //for sloped
// 	if(bDir) {
// 		for(int i = 0, k = 1; i < iSize; i++) {
// 			for(int j = 0; j < k; j++) {
// 				lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 25 : iLevelType == 3 ? 167 : 27);
// 			}
// 			++k;
// 		}
// 	} else {
// 		for(int i = 0, k = 1; i < iSize; i++) {
// 			for(int j = 0; j < k; j++) {
// 				lMap[X + iSize - 1 - i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 25 : iLevelType == 3 ? 167 : 27);
// 			}
// 			++k;
// 		}
// 	}
// }
// is identical to structGND but uses different tiles
// void Map::structGND2(int X, int Y, int iWidth, int iHeight) {
// 	for(int i = 0; i < iWidth; i++) {
// 		for(int j = 0; j < iHeight; j++) {
// 			lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 25 : iLevelType == 3 ? 167 : 27);
// 		}
// 	}
// }