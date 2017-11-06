#pragma once

#include "CommonIncludes.h"
#include "BoardFunctions.h"
#include "HeuristicFunctions.h"

class MoveFunctions
{
public:
	static bool IsValidChoice(string);
	static int BoardToIndex(string);

	static bool adjacent(int, int, std::map<int, std::vector<int>>);
	static bool destinationCheck(int, int, char* board, std::map<int, std::vector<int>>);
	static void attacking(Move, char*, std::map<int, std::vector<int>>);
	static vector<int> checkPossibleMoves(int, char*, std::map<int, std::vector<int>>);
	static vector<Move> getAllMoves(char*, bool, std::map<int, std::vector<int>>);
	static void ProcessMoveRequest(bool, bool, bool, char*, std::map<int, std::vector<int>>);
};
