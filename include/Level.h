#pragma once
#include <vector>
#include "Items.h"
class Level{
	private:
		std::vector<std::vector <int>> levelTiles;
	public:
		void buildGround(int X, int Y, int gWidth, int gHeight);
		void buildSteps(int X, int Y, int iSize, bool accOrObt); // true = ACCUTE, false = OBTUSE
		void addPipe(int X, int Y);
		void addItem(Items item, int X, int Y);
		void level_1_1();
		const std::vector<std::vector<int>>& getTiles() const { return levelTiles; }

		// void structGND2(int X, int Y, int iWidth, int iHeight);
		Level();
		~Level();
};