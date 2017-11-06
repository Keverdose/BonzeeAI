#pragma once

#include "CommonIncludes.h"
#include "MoveFunctions.h"


class HeuristicFunctions
{
public:
	static int getRowIndex(int);
	static int getColumnIndex(int);
	static string indexToBoard(int);
	static int Heuristic(char*);

	static int maxSearch(int, char*, bool, std::map<int, std::vector<int>>);
	static int minSearch(int, char*, bool, std::map<int, std::vector<int>>);
	static Move getAiMove(int, char*, bool, std::map<int, std::vector<int>>);
	



};
