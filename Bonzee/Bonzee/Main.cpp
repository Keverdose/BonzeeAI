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

// -- Function definitions
void ProcessMoveRequest();
Move getAiMove(int, char*, bool);

int main() {

	// Generate the Map for all indices
	for (int i = 0; i < MAX_BOARD_SIZE; i++) {
		BoardFunctions::generateMap(i, MAX_BOARD_SIZE, ROW_LENGTH, adjacentCells);
	}

	BoardFunctions::singleOrMultiplayer(aiTurn, singlePlayer);
	while (!BoardFunctions::winningBoard(board)) {
		
		BoardFunctions::PrintBoard(board);
		ProcessMoveRequest();
		isPlayerOne = !isPlayerOne;
		// cin.get();
		cout << "Current Board Heuristic: " << HeuristicFunctions::Heuristic(board) << endl;
	}

	// Post Game Display
	BoardFunctions::PrintBoard(board);
	cout << "Game is over!" << endl;

	cin.get();
}

// Proceed the move input
void ProcessMoveRequest() {

	bool completedTurn = false;
	string answer;

	do {
		// Check if current turn is ai's turn, and it's in singleplayer mode
		if (isPlayerOne == aiTurn && singlePlayer) {

			// Start Timer before AI checks for best move
			clock_t start = clock();

			// Get the best move from the ai (Via recursion)
			Move aiMove = getAiMove(DEPTH, board, aiTurn);

			// Display and Input the AI move
			cout << "The ai chooses: " << HeuristicFunctions::indexToBoard(aiMove.start) << " " << HeuristicFunctions::indexToBoard(aiMove.destination) << endl;
			MoveFunctions::attacking(aiMove, board, adjacentCells);

			// End Timer after AI inputted best move
			clock_t end = clock();
			double elapsedTime = double(end - start) / CLOCKS_PER_SEC;
			cout << "Elapsed time: " << elapsedTime << " seconds " << endl;

			completedTurn = true;
		}

		// else let the player play
		else {
			if (isPlayerOne)
				cout << "Player One's Turn (Green). \n  Please enter move: ";
			else
				cout << "Player Two's Turn (Red). \n  Please enter move: ";

			getline(cin, answer);
			transform(answer.begin(), answer.end(), answer.begin(), ::toupper); // Transforms input into lowercase 

			// Input Validation
			if (answer.length() != 5)
				cout << "Invalid String given, please try again." << endl;


			else if (answer.at(2) != ' ')
				cout << "Invalid String given, please try again." << endl;

			// Process Input
			else {
				// Transforms the answer into two coordinates
				int choIndex, destIndex;
				string choice = { answer.at(0), answer.at(1) };
				string destination = { answer.at(3), answer.at(4) };

				cout << "\nPlayer Input: " << endl;
				cout << "  Position: " << choice << "; Destination: " << destination << endl;

				// Checks if the two coordinates are within the array, if so then continue. else, prompt again
				if (MoveFunctions::IsValidChoice(choice) && MoveFunctions::IsValidChoice(destination)) {
					choIndex = MoveFunctions::BoardToIndex(choice);
					destIndex = MoveFunctions::BoardToIndex(destination);


					// Checks if selected token is valid, if there's available move for choindex, and if the destination is valid.
					if (BoardFunctions::isValid(isPlayerOne, board[choIndex]) && MoveFunctions::destinationCheck(choIndex, destIndex, board, adjacentCells)) {

						Move userMove;
						userMove.start = choIndex;
						userMove.destination = destIndex;

						// Execute Attack Algorithm
						MoveFunctions::attacking(userMove, board, adjacentCells);

						completedTurn = true;
					}

					else
						cout << "This is an invalid move. Please try again." << endl;
				}
				else
					cout << "Invalid positions, please try again." << endl;
			}

		}
	} while (!completedTurn);
}

// Implements minimax, and returns the best move the ai should take accordingly. 
Move getAiMove(int depth, char* board, bool playerMax) {

	vector<Move> allMoves = MoveFunctions::getAllMoves(board, playerMax, adjacentCells);
	Move aiMove;

	// Make new temp board and copy everything from previous board
	char tempBoard[MAX_BOARD_SIZE];
	for (int k = 0; k < MAX_BOARD_SIZE; k++) {
		tempBoard[k] = board[k];
	}

	// If AI is Maximizing (Max - Green Token)
	if (playerMax) {
		int highestValue = - 9999999;
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