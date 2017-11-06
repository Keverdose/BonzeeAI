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
void attacking(Move, char*);
vector<Move> getAllMoves(char *tempBoard, bool player);
int minSearch(int, char*, bool);
int maxSearch(int, char*, bool);
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

// Check the possible moves at given index
std::vector<int> checkPossibleMoves(int index, char* updatedBoard) {

	std::vector<int> moves;

	for (int i = 0; i < adjacentCells[index].size(); i++) {
		if (updatedBoard[adjacentCells[index].at(i)] == ' ')
			moves.push_back(adjacentCells[index].at(i));
	}

	return moves;
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
			attacking(aiMove, board);

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
						attacking(userMove, board);

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

// Loops through the array to add all possible move given the player's token into a vector.
vector<Move> getAllMoves(char *tempBoard, bool player){

	char token;
	vector<Move> allMoves;

	// Define the token color based on player's turn
	if (player) {
		token = 'G';
	}
	else {
		token = 'R';
	}

	if (!BoardFunctions::winningBoard(tempBoard)) {
		// Loops through array to check if the token is the current player
		for (int i = 0; i < MAX_BOARD_SIZE; i++) {
			if (tempBoard[i] == token) {

				// Checks all adjacents
				Move temp;
				temp.start = i;	// Setting the starting point of Move 

				// Check all possible valid moves that AI can make on this specific board configuration
				vector<int> possibleMovesDestination = checkPossibleMoves(i, tempBoard);

				// Loops through all available adjacents and stores all valid moves
				for (int j = 0; j < possibleMovesDestination.size(); j++ ) { 
					temp.destination = possibleMovesDestination[j];
					allMoves.push_back(temp);
				}
			}
		}
	}
	return allMoves;
}

// Implements minimax, and returns the best move the ai should take accordingly. 
Move getAiMove(int depth, char* board, bool playerMax) {

	vector<Move> allMoves = getAllMoves(board, playerMax);
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
			attacking(allMoves[i], tempBoard); // Perform attack on each valid move
			int value = maxSearch(depth - 1, tempBoard, !playerMax); // Value contains the best heuristic value 

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

			attacking(allMoves[i], tempBoard);	// Perform attack on each valid move
			int value = minSearch(depth - 1, tempBoard, !playerMax); // Value contains the best heuristic value 

			if (value < lowestValue) {
				cout << "Move updated because last move's heuristic: " << lowestValue << ", and current new move's heuristic: " << value << endl;
				lowestValue = value;
				aiMove = allMoves[i];
			}
		}
	}

	return aiMove; // Returns the ai's move.
}

// Minimize the win condition (Recursion)
int minSearch(int depth, char* board, bool player) {

	// Base case
	if (depth == 0 || getAllMoves(board, player).empty()) {
		return HeuristicFunctions::Heuristic(board);
	}
	else {
		vector<Move> allMoves = getAllMoves(board, player);
		char tempBoard[MAX_BOARD_SIZE];
		int minValue = 999999999;
		for (int i = 0; i < allMoves.size(); i++) {
			int comparedValue;
			for (int k = 0; k < MAX_BOARD_SIZE; k++) {
				tempBoard[k] = board[k];
			}
			attacking(allMoves[i], tempBoard);
			comparedValue = maxSearch(depth - 1, tempBoard, !player); 
			if (comparedValue < minValue) {
				minValue = comparedValue;
			}
		}
		return minValue;
	}

}

// Maximize the win condition (Recursion)
int maxSearch(int depth, char* board, bool player) {
	if (depth == 0 || getAllMoves(board, player).empty()) {
		return HeuristicFunctions::Heuristic(board);
	}
	else {
		vector<Move> allMoves = getAllMoves(board, player);
		char tempBoard[MAX_BOARD_SIZE];
		int maxValue = -9999999;
		for (int i = 0; i < allMoves.size(); i++) {
			for (int k = 0; k < MAX_BOARD_SIZE; k++) {
				tempBoard[k] = board[k];
			}
			attacking(allMoves[i], tempBoard);
			int comparedValue;
			comparedValue = minSearch(depth - 1, tempBoard, !player);
			if (comparedValue > maxValue) {
				maxValue = comparedValue;
			}
		}
		return maxValue;
	}
}

// Method to process the attacking
void attacking(Move inputMove, char* currentBoard) {

	// Get the move indices
	int dest = inputMove.destination;
	int pos = inputMove.start;

	// Move attacking token forward/backward 
	char playerToken = currentBoard[pos];
	currentBoard[dest] = currentBoard[pos];
	currentBoard[pos] = ' ';

	// Forward Attack 
	int direction    = dest - pos;
	int tempPosition = dest;
	int target       = dest + direction;
	char targetColor = currentBoard[target];

	// Backward attack: Check If destination is on board edge AND If target cell is empty or same color as initial position token
	if (target < 0 || target > MAX_BOARD_SIZE || targetColor == ' ' || targetColor == playerToken || (dest % ROW_LENGTH == 0 && (direction == -1 || direction == -(ROW_LENGTH + 1) || direction == (ROW_LENGTH - 1) )) || (dest % ROW_LENGTH == (ROW_LENGTH - 1) && (direction == 1 || direction == (ROW_LENGTH + 1) || direction == -(ROW_LENGTH - 1)))) {
		direction *= -1;
		tempPosition = pos;
		target       = pos + direction;
		targetColor  = currentBoard[target];
	}
	
	// Begin attack loop
	if (currentBoard[pos] != targetColor) {
		while (target < MAX_BOARD_SIZE && target > -1 && (targetColor == currentBoard[target]) && MoveFunctions::adjacent(tempPosition, target, adjacentCells) && currentBoard[target] != ' ' && currentBoard[target] != playerToken) {
			currentBoard[target] = ' ';
			tempPosition = target;
			target += direction;
		}
	}
}