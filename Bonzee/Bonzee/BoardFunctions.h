#pragma once

#include "CommonIncludes.h"

class BoardFunctions
{
public:
	static void PrintBoard(char*);
	static bool winningBoard(char*);
	static bool isValid(bool, char);
	static void generateMap(int, int, int, std::map<int, std::vector<int>>&);
	static void singleOrMultiplayer(bool&, bool&);
};
