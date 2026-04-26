#pragma once
#include <vector>
#include <string>

using TileMatrix = std::vector<std::vector<int>>;

namespace shapes {

	/*
	extern const makes a non-class-member variable accessible across translation units, 
	by declaring it in the header without allocating memory at the declaration site — 
	the single allocation happens in the one .cpp that contains the definition.
	*/

	extern const TileMatrix bushSetOne;
	extern const TileMatrix bushSetTwo;
	extern const TileMatrix bushSetTwoHalf;
	extern const TileMatrix smallestPossiblePipe;
	extern const TileMatrix oneFaceCloudA;
	extern const TileMatrix oneFaceCloudB;

	TileMatrix buildScrewPlatform(const std::string& color, int innerW, int innerH);
	TileMatrix shadowfy(TileMatrix& platform, std::string type = "ground",std::string colorBehind = "transparent");
	TileMatrix buildMultiFaceCloud(int faces);
	TileMatrix buildPipe(int levelsGreaterThanMinHeight);

}