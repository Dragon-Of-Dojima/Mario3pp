#include "Shapes.h"
#include "Tiles.h"

// Tile ID legend (see Tiles.h). Re-sync these numbers if the enum is reordered.
//   0 = BLUESKY
//   8 = PIPE_TOP_LEFT                      9 = PIPE_TOP_RIGHT
//  10 = PIPE_LEFT_EDGE                    11 = PIPE_RIGHT_EDGE
//  95 = CHECKERBUSH_LEFT_EDGE             96 = CHECKERBUSH_RIGHT_EDGE
//  97 = CHECKERBUSH_INNER
//  98 = CHECKERBUSH_TOP_LEFT_WITH_BEHIND  99 = CHECKERBUSH_TOP_RIGHT_WITH_BEHIND
// 100 = CHECKERBUSH_TOP_LEFT             101 = CHECKERBUSH_TOP_RIGHT

namespace shapes {

const TileMatrix bushSetOne = {
	{95,97,97,97,96},
	{95,98,99,97,96},
	{100,99,96,100,101},
	{0,100,101,0,0}
};
const TileMatrix bushSetTwo = { //[0][0] is 101 when standalone
	{98,99,97,96},
	{0,95,98,101},
	{0,100,101,0}
};
const TileMatrix bushSetTwoHalf = { //[0][0] is 101 when standalone
	{101,99,97,96},
	{0,95,98,101},
	{0,100,101,0}
};
const TileMatrix smallestPossiblePipe = {
	{10,11},
	{8,9}
};
const TileMatrix oneFaceCloud = {
	{89,90},
	{87,88}
};

TileMatrix buildScrewPlatform(const std::string& color, int innerW, int innerH){
	std::vector<int> tilesToUse = {};
	// Row-major to match levelTiles: outer = Y (rows), inner = X (cols).
	TileMatrix platformMatrix(innerH + 2, std::vector<int>(innerW + 2, -1));
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
	for(size_t j = 0; j < platformMatrix.size(); j++){           // rows (Y)
		for(size_t i = 0; i < platformMatrix[0].size(); i++){    // cols (X)
			if(j == 0){ //bottom row
				if(i == 0){ //left corner
					platformMatrix[j][i] = tilesToUse[6];
				}else if(i == platformMatrix[0].size() - 1){ //right corner
					platformMatrix[j][i] = tilesToUse[8];
				}
				else{
					if(innerW > 0){
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
						if(innerW > 0){
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
					if(innerW > 0){
						platformMatrix[j][i] = tilesToUse[1];
					}
				}
			}
		}
	}

	return platformMatrix;
}

TileMatrix buildMultiFaceCloud(int faces){
	std::vector<int> tilesToUse = {
		CLOUD_MULTI_LEFT_SHOULDER, CLOUD_MULTI_HEAD, CLOUD_MULTI_RIGHT_SHOULDER,
		CLOUD_MULTI_BOTTOM_LEFT, CLOUD_MULTI_BOTTOM_CENTER, CLOUD_MULTI_BOTTOM_RIGHT
	};
	TileMatrix cloudToRet(2, std::vector<int>(faces + 2, -1));
	for(size_t j = 0; j < cloudToRet.size(); j++){
		for(size_t i = 0; i < cloudToRet[0].size(); i++){
			if(j == 0){
				if(i == 0){
					cloudToRet[j][i] = tilesToUse[3];
				}
				else if(i == cloudToRet[0].size() - 1){
					cloudToRet[j][i] = tilesToUse[5];
				}else{
					cloudToRet[j][i] = tilesToUse[4];
				}
			}
			else{
				if(i == 0){
					cloudToRet[j][i] = tilesToUse[0];
				}
				else if(i == cloudToRet[0].size() - 1){
					cloudToRet[j][i] = tilesToUse[2];
				}else{
					cloudToRet[j][i] = tilesToUse[1];
				}
			}
		}
	}
	return cloudToRet;
}

} // namespace shapes
