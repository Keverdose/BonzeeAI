#include "HeuristicFunctions.h"

int HeuristicFunctions::getRowIndex(int index)
{
	float val = index / ROW_LENGTH;

	// A = 1, B = 2, C = 3, D = 4, E = 5
	if (val < 0 || val > 5) {
		return -1;
	}
	else {
		return val + 1;
	}
}

int HeuristicFunctions::getColumnIndex(int index)
{
	return (index % ROW_LENGTH) + 1;
}

string HeuristicFunctions::indexToBoard(int index)
{
	int charVal = index / 9;
	int columnValue = index % 9 + 1;
	char rowValue;
	// A = 0, B = 1, C = 2, D = 3, E = 4
	if (charVal == 0) {
		rowValue = 'a';
	}
	else if (charVal == 1) {
		rowValue = 'b';
	}
	else if (charVal == 2) {
		rowValue = 'c';
	}
	else if (charVal == 3) {
		rowValue = 'd';
	}
	else if (charVal == 4) {
		rowValue = 'e';
	}
	string temp = "";
	temp += rowValue;
	temp += std::to_string(index % 9 + 1);

	return temp;
}

int HeuristicFunctions::Heuristic(char* tempBoard)
{
	/*
	THE NAIVE HEURISTIC IS:
	e(board) =
	100 x (SUM[1 to # green tokens left](horizontal index for the current green token visited))
	+ 50 x (SUM[1 to # green tokens left](vertical index for the current green token visited))
	- 100 x (SUM[1 to # red tokens left](horizontal index for the current red token visited))
	- 50 x (SUM[1 to # red tokens left](vertical index for the current red token visited))
	*/

	// Vertical/Horizontal Indexes values for each color
	int verticalGreenVal = 0, verticalRedVal = 0, horizontalGreenVal = 0, horizontalRedVal = 0;

	for (int i = 0; i < MAX_BOARD_SIZE; i++) {
		if (tempBoard[i] == 'G') {
			/*visitedGreen--;*/
			verticalGreenVal += HeuristicFunctions::getColumnIndex(i);
			horizontalGreenVal += HeuristicFunctions::getRowIndex(i);
		}
		if (tempBoard[i] == 'R') {
			//visitedRed--;
			verticalRedVal += HeuristicFunctions::getColumnIndex(i);
			horizontalRedVal += HeuristicFunctions::getRowIndex(i);
		}
	}

	// Return the heuristic value
	return (HEURISTIC_MULTIPLIER_100 * horizontalGreenVal) + (HEURISTIC_MULTIPLIER_50 * verticalGreenVal) - (HEURISTIC_MULTIPLIER_100 * horizontalRedVal) - (HEURISTIC_MULTIPLIER_50 * verticalRedVal);
}

int HeuristicFunctions::maxSearch(int depth, char* board, bool player, std::map<int, std::vector<int>>& adjacentCells)
{
	if (depth == 0 || MoveFunctions::getAllMoves(board, player, adjacentCells).empty()) {
		return HeuristicFunctions::Heuristic(board);
	}
	else {
		vector<Move> allMoves = MoveFunctions::getAllMoves(board, player, adjacentCells);
		char tempBoard[MAX_BOARD_SIZE];
		int maxValue = -9999999;
		for (int i = 0; i < allMoves.size(); i++) {
			for (int k = 0; k < MAX_BOARD_SIZE; k++) {
				tempBoard[k] = board[k];
			}
			MoveFunctions::attacking(allMoves[i], tempBoard, adjacentCells);
			int comparedValue;
			comparedValue = HeuristicFunctions::minSearch(depth - 1, tempBoard, !player, adjacentCells);
			if (comparedValue > maxValue) {
				maxValue = comparedValue;
			}
		}
		return maxValue;
	}
}

int HeuristicFunctions::minSearch(int depth, char* board, bool player, std::map<int, std::vector<int>>& adjacentCells)
{

	// Base case
	if (depth == 0 || MoveFunctions::getAllMoves(board, player, adjacentCells).empty()) {
		return HeuristicFunctions::Heuristic(board);
	}
	else {
		vector<Move> allMoves = MoveFunctions::getAllMoves(board, player, adjacentCells);
		char tempBoard[MAX_BOARD_SIZE];
		int minValue = 999999999;
		for (int i = 0; i < allMoves.size(); i++) {
			int comparedValue;
			for (int k = 0; k < MAX_BOARD_SIZE; k++) {
				tempBoard[k] = board[k];
			}
			MoveFunctions::attacking(allMoves[i], tempBoard, adjacentCells);
			comparedValue = HeuristicFunctions::maxSearch(depth - 1, tempBoard, !player, adjacentCells);
			if (comparedValue < minValue) {
				minValue = comparedValue;
			}
		}
		return minValue;
	}
}

Move HeuristicFunctions::getAiMove(int depth, char* board, bool playerMax, std::map<int, std::vector<int>>& adjacentCells)
{
	vector<Move> allMoves = MoveFunctions::getAllMoves(board, playerMax, adjacentCells);
	Move aiMove;

	// Make new temp board and copy everything from previous board
	char tempBoard[MAX_BOARD_SIZE];
	for (int k = 0; k < MAX_BOARD_SIZE; k++) {
		tempBoard[k] = board[k];
	}

	// If AI is Maximizing (Max - Green Token)
	if (playerMax) {
		int highestValue = -9999999;
		for (int i = 0; i < allMoves.size(); i++) {
			// Get Fresh copy of previous board
			for (int k = 0; k < MAX_BOARD_SIZE; k++) {
				tempBoard[k] = board[k];
			}
			MoveFunctions::attacking(allMoves[i], tempBoard, adjacentCells); // Perform attack on each valid move
			int value = HeuristicFunctions::maxSearch(depth - 1, tempBoard, !playerMax, adjacentCells); // Value contains the best heuristic value 

			if (value > highestValue) {
				cout << "Move updated because last move's heuristic: " << highestValue << ", and current new move's heuristic: " << value << endl;
				highestValue = value;
				aiMove = allMoves[i];
			}
		}
	}

	// If AI is Minimizing (Min - Red Token)
	else if (!playerMax) {
		int lowestValue = 9999999;

		for (int i = 0; i < allMoves.size(); i++) {
			// Get Fresh copy of the board
			for (int k = 0; k < MAX_BOARD_SIZE; k++) {
				tempBoard[k] = board[k];
			}

			MoveFunctions::attacking(allMoves[i], tempBoard, adjacentCells);	// Perform attack on each valid move
			int value = HeuristicFunctions::minSearch(depth - 1, tempBoard, !playerMax, adjacentCells); // Value contains the best heuristic value 

			if (value < lowestValue) {
				cout << "Move updated because last move's heuristic: " << lowestValue << ", and current new move's heuristic: " << value << endl;
				lowestValue = value;
				aiMove = allMoves[i];
			}
		}
	}

	return aiMove; // Returns the ai's move.
}
