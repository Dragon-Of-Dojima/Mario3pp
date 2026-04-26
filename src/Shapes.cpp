#include "Shapes.h"
#include "Tiles.h"
#include <unordered_map>

// Tile ID legend (see Tiles.h). Re-sync these numbers if the enum is reordered.
//   0 = BLUESKY
//   8 = PIPE_TOP_LEFT                      9 = PIPE_TOP_RIGHT
//  10 = PIPE_LEFT_EDGE                    11 = PIPE_RIGHT_EDGE
//  82 = CLOUD_MULTI_HEAD                  83 = CLOUD_MULTI_LEFT_SHOULDER
//  84 = CLOUD_MULTI_RIGHT_SHOULDER        85 = CLOUD_MULTI_BOTTOM_LEFT
//  86 = CLOUD_MULTI_BOTTOM_CENTER         87 = CLOUD_MULTI_BOTTOM_RIGHT
//  88 = CLOUD_SINGLE_TOP_LEFT             89 = CLOUD_SINGLE_TOP_RIGHT
//  90 = CLOUD_SINGLE_BOTTOM_LEFT          91 = CLOUD_SINGLE_BOTTOM_RIGHT
//  96 = CHECKERBUSH_LEFT_EDGE             97 = CHECKERBUSH_RIGHT_EDGE
//  98 = CHECKERBUSH_INNER
//  99 = CHECKERBUSH_TOP_LEFT_WITH_BEHIND 100 = CHECKERBUSH_TOP_RIGHT_WITH_BEHIND
// 101 = CHECKERBUSH_TOP_LEFT             102 = CHECKERBUSH_TOP_RIGHT

namespace shapes {

	const TileMatrix bushSetOne = {
		{96,98,98,98,97},
		{96,99,100,98,97},
		{101,100,97,101,102},
		{0,101,102,0,0}
	};
	const TileMatrix bushSetTwo = { 
		{99,100,98,97},
		{0,96,99,102},
		{0,101,102,0}
	};
	const TileMatrix bushSetFour = { //basically the same as above but standalone
		{101,100,98,97},
		{0,96,99,102},
		{0,101,102,0}
	};
	const TileMatrix smallestPossiblePipe = {
		{10,11},
		{8,9}
	};
	const TileMatrix oneFaceCloudA = {
		{85,86,87},
		{83,82,84}
	};
	const TileMatrix oneFaceCloudB = {
		{90,91},
		{88,89}
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

	void shadowfy(TileMatrix& platform, std::string type, std::string colorBehind){
		static const std::unordered_map<std::string, std::vector<int>> shadowTiles = {
			{"PINK",	{PINK_PLATFORM_SHADOW_TOP,  PINK_PLATFORM_SHADOW_EDGE,  PINK_PLATFORM_SHADOW_BOTTOM}},
			{"BLUE",	{BLUE_PLATFORM_SHADOW_TOP,  BLUE_PLATFORM_SHADOW_EDGE,  BLUE_PLATFORM_SHADOW_BOTTOM}},
			{"WHITE",	{WHITE_PLATFORM_SHADOW_TOP, WHITE_PLATFORM_SHADOW_EDGE, WHITE_PLATFORM_SHADOW_BOTTOM}},
			{"GREEN",	{GREEN_PLATFORM_SHADOW_TOP, GREEN_PLATFORM_SHADOW_EDGE, GREEN_PLATFORM_SHADOW_BOTTOM}},
			{"TRANSPARENT", {SHADOW_OVER_TRANSPARENT_TOP_RIGHT_CORNER, SHADOW_OVER_TRANSPARENT_RIGHTEDGE, SHADOW_OVER_TRANSPARENT_BR, 
				SHADOW_OVER_TRANSPARENT_BL, SHADOW_OVER_TRANSPARENT_BHE
			}}
		};
		// +1 to span under the right-edge shadow column that gets appended below.
		std::vector<int> shadowBottomRow(platform[0].size() + 1, -1);
		if(type == "insky"){
			const size_t n = shadowBottomRow.size();
			for(size_t q = 0; q < n; q++){
				if(q == 0){
					shadowBottomRow[q] = shadowTiles.at(colorBehind)[3]; // BL
				}
				else if(q < n - 1){
					shadowBottomRow[q] = shadowTiles.at(colorBehind)[4]; // BHE
				}
				else{
					shadowBottomRow[q] = shadowTiles.at(colorBehind)[2]; // BR (corner)
				}
			}
		}
		const size_t numRows = platform.size();
		for(size_t j = 0; j < numRows; j++){
			if(j == 0){ //bottom row
				if(colorBehind != "TRANSPARENT"){
					platform[j].push_back(shadowTiles.at(colorBehind)[2]);
				}
				else{
					platform[j].push_back(shadowTiles.at(colorBehind)[1]);
				}
				
			}
			else if(j < numRows - 1){ //side edge
				platform[j].push_back(shadowTiles.at(colorBehind)[1]);
			}
			else{ // j == numRows - 1, top
				platform[j].push_back(shadowTiles.at(colorBehind)[0]);
			}
		}
		if(type == "insky"){
			platform.insert(platform.begin(),shadowBottomRow);
		}
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
	TileMatrix buildPipe(int levelsGreaterThanMinHeight){
		TileMatrix pipe = smallestPossiblePipe;
		pipe.insert(pipe.begin(), levelsGreaterThanMinHeight, {PIPE_LEFT_EDGE, PIPE_RIGHT_EDGE});
		return pipe;
	}

} // namespace shapes
