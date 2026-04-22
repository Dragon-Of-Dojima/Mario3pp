#pragma once
#include <vector>
#include <string>

using TileMatrix = std::vector<std::vector<int>>;

namespace shapes {

extern const TileMatrix bushSetOne;
extern const TileMatrix bushSetTwo;
extern const TileMatrix bushSetTwoHalf;
extern const TileMatrix smallestPossiblePipe;
extern const TileMatrix oneFaceCloud;

TileMatrix buildScrewPlatform(const std::string& color, int innerW, int innerH);
TileMatrix buildMultiFaceCloud(int faces);

}