#include "CommonIncludes.h"
#include "BoardFunctions.h"
#include "MoveFunctions.h"
#include "HeuristicFunctions.h"

// Booleans
static bool isPlayerOne = true;
static bool singlePlayer = true;
static bool aiTurn;

// Vector of Heuristic Values
static std::vector<int> heuristicValue;

/* BOARD LAYOUT:
	1  2  3  4  5  6  7  8  9
A	0  1  2  3  4  5  6  7  8
B	9  10 11 12 13 14 15 16 17
C	18 19 20 21 22 23 24 25 26
D	27 28 29 30 31 32 33 34 35
E	36 37 38 39 40 41 42 43 44

So A maps to [0, 8] range in array, etc.
A move choice of B3 means 11 (B maps to [9, 17] so 9+3-1 = 11 (the cols go from 1-9)
*/


// -- char version of board
char board[MAX_BOARD_SIZE] = {
	'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
	'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
	'G', 'G', 'G', 'G', ' ', 'R', 'R', 'R', 'R',
	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G' };

std::map<int, std::vector<int> > adjacentCells;

//char board[MAX_BOARD_SIZE] = {
//	'R', 'R', 'R', 'R', ' ', 'R', ' ', 'R', ' ',
//	'R', 'R', 'R', ' ', 'R', 'R', 'R', 'R', ' ',
//	' ', ' ', ' ', ' ', 'R', ' ', 'G', ' ', 'G',
//	'G', 'G', 'G', ' ', 'G', 'G', 'G', ' ', ' ',
//	'G', 'G', 'G', ' ', 'G', 'G', 'G', 'G', ' ' };

// Board setting to win in one move
//char board[MAX_BOARD_SIZE] = {
//	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//	'G', 'G', 'G', 'G', ' ', 'R', 'R', 'R', ' ',
//	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
//	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G' };

int main() {

	// Generate the Map for all indices
	for (int i = 0; i < MAX_BOARD_SIZE; i++) {
		BoardFunctions::generateMap(i, MAX_BOARD_SIZE, ROW_LENGTH, adjacentCells);
	}

	BoardFunctions::singleOrMultiplayer(aiTurn, singlePlayer);
	while (!BoardFunctions::winningBoard(board)) {
		
		BoardFunctions::PrintBoard(board);
		MoveFunctions::ProcessMoveRequest(isPlayerOne, aiTurn, singlePlayer, board, adjacentCells);
		isPlayerOne = !isPlayerOne;
		// cin.get();
		cout << "Current Board Heuristic: " << HeuristicFunctions::Heuristic(board) << endl;
	}

	// Post Game Display
	BoardFunctions::PrintBoard(board);
	cout << "Game is over!" << endl;

	cin.get();
}
