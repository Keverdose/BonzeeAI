#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <vector>

// -- Namespaces
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::transform;
using std::vector;

// -- Variable definitions
static const int MAX_BOARD_SIZE = 45;
static const int EMPTY_BOARD    = 0;
static const int ROW_LENGTH     = 9;
static const int COLUMN_LENGTH  = 5;
static const int MAX_PIECES_NUM = 22;
static const int ASCII_LETTER_OFFSET = 48;
static const int HEURISTIC_MULTIPLIER_100 = 100;
static const int HEURISTIC_MULTIPLIER_50 = 50;

static const int depth = 4; // Level of recursion

static int greenCounter = MAX_PIECES_NUM;
static int redCounter   = MAX_PIECES_NUM;
static bool isPlayerOne = true;
static bool singlePlayer = true;
static bool aiTurn;
static std::vector<int> heuristicValue;

// Move from start to destination in index
struct Move {
	int start;
	int destination;
};


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

//char board[MAX_BOARD_SIZE] = {
//	'R', 'R', 'R', 'R', ' ', 'R', ' ', 'R', ' ',
//	'R', 'R', 'R', ' ', 'R', 'R', 'R', 'R', ' ',
//	' ', ' ', ' ', ' ', 'R', ' ', 'G', ' ', 'G',
//	'G', 'G', 'G', ' ', 'G', 'G', 'G', ' ', ' ',
//	'G', 'G', 'G', ' ', 'G', 'G', 'G', 'G', ' ' };

std::map<int, std::vector<int> > adjacentCells;

// Board setting to win in one move
//char board[MAX_BOARD_SIZE] = {
//	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
//	'G', 'G', 'G', 'G', ' ', 'R', 'R', 'R', ' ',
//	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
//	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G' };
	
// redCounter = 3; // REMEMBER TO CHANGE RED COUNTER TO 3

// -- Function definitions
void PrintBoard();
void ProcessMoveRequest();
bool availableSpace(int);
bool isValid(char);
bool IsValidChoice(string);
bool destinationCheck(int, int);
int BoardToIndex(string);
void attacking(Move, char*);
bool adjacent(int, int);
void tokenCountUpdate();
int Heuristic(char*);
int getRowIndex(int);
int getColumnIndex(int);
string indexToBoard(int);
bool winningBoard(char*);
vector<Move> getAllMoves(char *tempBoard, bool player);
int minSearch(int, char*, bool);
int maxSearch(int, char*, bool);
Move getAiMove(int, char*, bool);
void generateMap(int, int, int);
void singleOrMultiplayer();


int main() {

	// Generate the Map for all indices
	for (int i = 0; i < MAX_BOARD_SIZE; i++) {
		generateMap(i, MAX_BOARD_SIZE, ROW_LENGTH);
	}

	//// Print out map
	//for (int j = 0; j < MAX_BOARD_SIZE; j++) {

	//	cout << "Map[" << j << "] = ( ";

	//	for (int k = 0; k < adjacentCells[j].size(); k++) {
	//		cout << adjacentCells[j].at(k) << " ";
	//	}

	//	cout << ") " << endl;
	//}


	//std::vector<int> positions = checkPossibleMoves(23, board);

	//for (int o = 0; o < positions.size(); o++) {
	//	cout << "Possible Moves: " << positions.at(o) << endl;
	//}

	singleOrMultiplayer();
	while (!winningBoard(board)) {
		
		PrintBoard();
		ProcessMoveRequest();
		isPlayerOne = !isPlayerOne;
		// cin.get();
		cout << Heuristic(board);
	}

	// Post Game Display
	PrintBoard();
	cout << "Game is over!" << endl;

	cin.get();
}

// Generates the Map of all Adjacent Cells based on possible moves
void generateMap(int index, int boardSize, int row_length) {

	// Insert a new entry into the Map
	adjacentCells.insert(std::pair<int, std::vector<int> >(index, std::vector<int>()));

	// ============ Check Left Bound ============ 
	if (index % row_length != 0) {

		// -- Add Upper Left Diagonal Index
		if ((index > row_length) && (index % 2 == 0)) {
			adjacentCells[index].push_back(index - (row_length + 1));
		}
		// -- Add Lower Left Diagonal Index
		if ((index < (boardSize - row_length)) && (index % 2 == 0)) {
			adjacentCells[index].push_back(index + (row_length - 1));
		}
		// Add Left Index
		 adjacentCells[index].push_back(index - 1);
	}

	// ============ Check Right Bound ============  
	if ((index % (row_length)) != (row_length - 1)) {

		// -- Add Upper Right Diagonal Index
		if ((index > (row_length - 1)) && (index % 2 == 0)) {
			adjacentCells[index].push_back((index - row_length) + 1);
		}
		// -- Add Lower Right Diagonal Index
		if ((index < boardSize - row_length) && (index % 2 == 0)) {
			adjacentCells[index].push_back(index + (row_length + 1));
		}
		// Add Right Index
		adjacentCells[index].push_back(index + 1);
	}

	// Add Top Index
	if (index > (row_length - 1) )
		adjacentCells[index].push_back(index - row_length);
	// Add Bottom Index
	if (index < (boardSize - row_length))
		adjacentCells[index].push_back(index + row_length);
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

// Method to decide whether it's multiplayer or single player
void singleOrMultiplayer() {

	bool modeChosen = false;

	do {
		// Ask and process input
		cout << "Would you like to play singleplayer (vs AI) or multiplayer (vs Human) (S/M): ";
		string answer = "";
		getline(cin, answer);
		transform(answer.begin(), answer.end(), answer.begin(), ::toupper);

		// Input Check
		if (answer.length() != 1 ) {
			cout << "Invalid choice, please try again: ";
		}

		else {
			// If user chooses singleplayer mode, make him choose the AI's Token.
			if (answer.at(0) == 'S') {

				bool aiTokenChosen = false;
				
				do {
					// Ask and process input
					cout << "Singleplayer mode chosen. You will play against a minimax A.I.; \nWhich token will the A.I. play? (R/G): ";
					string aiChoice = "";
					getline(cin, aiChoice);
					transform(aiChoice.begin(), aiChoice.end(), aiChoice.begin(), ::toupper);
					
					// Input Check
					if (aiChoice.length() != 1 || (aiChoice.at(0) != 'R' && aiChoice.at(0) != 'G')) {
						cout << "Invalid token, please try again: ";
					}

					else {
						// AI is Red Token, starts 2nd
						if (aiChoice.at(0) == 'R') {
							aiTurn = false;
						}
						// AI is Green Token, starts 1st
						else {
							aiTurn = true;
						}
						aiTokenChosen = true;
					}
				} while (!aiTokenChosen);
			}

			// If user chooses multiplayer mode, exit normally
			if (answer.at(0) == 'M') {
				cout << "Multiplayer mode chosen." << endl;
				singlePlayer = false;
			}
			modeChosen = true;
		}
	} while (!modeChosen);
}


// Function to Print Current Board Configuration
void PrintBoard() {
	cout << "\n\n ======================== B O N Z E E ======================= " << endl;
	cout << "\n       1     2     3     4     5     6     7     8     9";

	char startLetter = 'A';

	for (auto i = 0; i < MAX_BOARD_SIZE; i++) {
		
		// Prints the Line Letter
		if (i % ROW_LENGTH == 0) {
			cout << "\n\n  " << (char)(startLetter + (i / ROW_LENGTH)) << " ";
		}

		// Prints Content of the Line 
		if (i % 2 == 0)
			cout << " [ " << board[i] << " ]";  // Black Square 
		else
			cout << " ( " << board[i] << " )";  // White Square	
	}

	cout << "\n\n  Black Cells Denoted By: [ ] - White Cells Denoted By: ( )";
	cout << "\n\n ============================================================\n" << endl;

}

// Proceed the move input
void ProcessMoveRequest() {

	bool completedTurn = false;
	string answer;

	do {
		// Check if current turn is ai's turn, and it's in singleplayer mode
		if (isPlayerOne == (singlePlayer && aiTurn) ) {

			// Get the best move from the ai (Via recursion)
			Move aiMove = getAiMove(depth, board, aiTurn);

			// Display and Input the AI move
			cout << "The ai chooses: " << indexToBoard(aiMove.start) << " " << indexToBoard(aiMove.destination) << endl;
			attacking(aiMove, board);
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

				cout << "\nBoard Update: " << endl;
				cout << "  Position: " << choice << "; Destination: " << destination << endl;

				// Checks if the two coordinates are within the array, if so then continue. else, prompt again
				if (IsValidChoice(choice) && IsValidChoice(destination)) {
					choIndex = BoardToIndex(choice);
					destIndex = BoardToIndex(destination);

					// Checks if selected token is valid, if there's available move for choindex, and if the destination is valid.
					if (isValid(board[choIndex]) && destinationCheck(choIndex, destIndex)) {

						Move userMove;
						userMove.start = choIndex;
						userMove.destination = destIndex;

						// Execute Attack Algorithm
						attacking(userMove, board);

						completedTurn = true;
						cout << "  Red: " << redCounter << ", Green: " << greenCounter << endl;
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


//// Function that returns boolean to check whether the selected token has an available move
//bool availableSpace(int currentPosition) {
//
//	// If the current position is out of bound
//	if (currentPosition < EMPTY_BOARD || currentPosition > (MAX_BOARD_SIZE - 1)) 
//		return false;
//
//		
//	// Checks downwards
//	if (currentPosition + ROW_LENGTH < MAX_BOARD_SIZE) {
//		if (board[currentPosition + ROW_LENGTH] == ' ') {
//			return true;
//		}
//	}
//		
//	// Checks upwards
//	if (currentPosition - ROW_LENGTH > -1) {
//		if (board[currentPosition - ROW_LENGTH] == ' ') {
//			return true;
//		}
//	}
//		
//	// Checks right (Makes sure the rightmost column can't go right)
//	if ((currentPosition % ROW_LENGTH + 1) != ROW_LENGTH) {
//		if (board[currentPosition + 1] == ' ') {
//			return true;
//		}
//	}
//		
//	// Checks left (Makes sure the leftmost column can't go left)
//	if ((currentPosition % ROW_LENGTH) != 0) {
//		if (board[currentPosition - 1] == ' ') {
//			return true;
//		}
//	}
//		
//
//	// IF CELL IS BLACK (DIAGONAL CHECKS)
//	if (currentPosition % 2 == 0) {
//
//		// Checks topleft
//		if (currentPosition - (ROW_LENGTH + 1) > 0 && (currentPosition % ROW_LENGTH) != 0) {
//			if (board[currentPosition - (ROW_LENGTH + 1)] == ' ') {
//				return true;
//			}
//		}
//			
//		// Checks topright
//		if (currentPosition - (ROW_LENGTH - 1) > 0 && (currentPosition % ROW_LENGTH + 1) != ROW_LENGTH) {
//			if (board[currentPosition - (ROW_LENGTH - 1)] == ' ') {
//				return true;
//			}
//		}
//			
//		// Checks bottomleft
//		if ((currentPosition % ROW_LENGTH) != 0 && (currentPosition + (ROW_LENGTH - 1) < MAX_BOARD_SIZE)) {
//			if (board[currentPosition + (ROW_LENGTH - 1)] == ' ') {
//				return true;
//			}
//		}
//			
//		// Checks bottomright
//		if ((currentPosition + (ROW_LENGTH + 1) < MAX_BOARD_SIZE) && (currentPosition % ROW_LENGTH + 1) != ROW_LENGTH) {
//			if (board[currentPosition + (ROW_LENGTH + 1)] == ' ') {
//				return true;
//			}
//		}
//
//	}
//
//	return false;
//}


// Checks whether it is a valid destination 
bool destinationCheck(int position, int destination) {

	// If cell occupied, return false;
	if (board[destination] != ' ') 
		return false; 

	//Else check if the cell is adjacent to position.
	else 
		return adjacent(position, destination);
}


// Method to check adjacency of positions
bool adjacent(int position, int destination) {

	// Have to make sure that the 1st column and last column cant go left or right
	if (position % 2 == 0) {
		if (position % ROW_LENGTH == 0)
			return(position + 1 == destination
				|| position + (ROW_LENGTH + 1) == destination
				|| position - ROW_LENGTH == destination
				|| position + ROW_LENGTH == destination
				|| position - (ROW_LENGTH - 1) == destination);

		else if (position % ROW_LENGTH == (ROW_LENGTH - 1))
			return(position - 1 == destination
				|| position - (ROW_LENGTH + 1) == destination
				|| position - ROW_LENGTH == destination
				|| position + ROW_LENGTH == destination
				|| position + (ROW_LENGTH - 1) == destination);

		return(position + 1 == destination
			|| position - 1 == destination
			|| position - (ROW_LENGTH + 1) == destination
			|| position + (ROW_LENGTH + 1) == destination
			|| position - ROW_LENGTH == destination
			|| position + ROW_LENGTH == destination
			|| position - (ROW_LENGTH - 1) == destination
			|| position + (ROW_LENGTH - 1) == destination);
	}
	else {
		if (position % ROW_LENGTH == 0)
			return(position + 1 == destination
				|| position - ROW_LENGTH == destination
				|| position + ROW_LENGTH == destination);

		else if (position % ROW_LENGTH == (ROW_LENGTH - 1))
			return(position - 1 == destination
				|| position - ROW_LENGTH == destination
				|| position + ROW_LENGTH == destination);

		return(position + 1 == destination
			|| position - 1 == destination
			|| position - ROW_LENGTH == destination
			|| position + ROW_LENGTH == destination);
	}
}


// Method to check whether green or red is playing.
bool isValid(char color) {
	if (isPlayerOne)
		return (color == 'G');
	else
		return (color == 'R');
}


// Method to check whether the choice is in the board or not
bool IsValidChoice(string choice) {
	if (choice == "") 
		return false; 

	char inputChar = choice.at(0);

	if (inputChar == 'A' || inputChar == 'B' || inputChar == 'C' || inputChar == 'D' || inputChar == 'E') {
		if ((int)choice.at(1) - ASCII_LETTER_OFFSET > 0 && (int)choice.at(1) - ASCII_LETTER_OFFSET <= ROW_LENGTH)
			return true; 
	}

	return false;
}


// Method to change the coordinates to arrayIndex
int BoardToIndex(string choice) {
	char row = choice.at(0);
	int offset;
	switch (tolower(row)) {
		case 'a': offset = 0;
			break;
		case 'b': offset = 1;
			break;
		case 'c': offset = 2;
			break;
		case 'd': offset = 3;
			break;
		case 'e': offset = 4;
			break;
	}

	return (offset * ROW_LENGTH + (int)choice.at(1) - ASCII_LETTER_OFFSET - 1);
}

// Method to transform an index to Board coordinates  <-- THIS IS FOR THE AI
string indexToBoard(int index) {
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

	if (!winningBoard(tempBoard)) {
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
		return Heuristic(board);
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
		return Heuristic(board);
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

// Check if either player wins.
bool winningBoard(char* board) {
	int gcounter = 0;
	int rcounter = 0;
	for (int i = 0; i < MAX_BOARD_SIZE; i++) {
		if (board[i] == 'G') {
			gcounter++;
		}
		else if (board[i] == 'R') {
			rcounter++;
		}

		if (gcounter > 0 && rcounter > 0)
			return false;
	}
	return (gcounter == 0 || rcounter == 0);
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
		while (target < MAX_BOARD_SIZE && target > -1 && (targetColor == currentBoard[target]) && adjacent(tempPosition, target) && currentBoard[target] != ' ' && currentBoard[target] != playerToken) {
			currentBoard[target] = ' ';
			tempPosition = target;
			target += direction;
			//tokenCountUpdate();
		}
	}
}

// Decrements token count depending on player's turn
void tokenCountUpdate() {
	if (isPlayerOne) 
		redCounter--;    // Decrement red token count
	else 
		greenCounter--;  // Decrement green token count
}

int Heuristic(char* tempBoard) {
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

	for (int i = 0; i < MAX_BOARD_SIZE; i++){
		if (tempBoard[i] == 'G'){
			/*visitedGreen--;*/
			verticalGreenVal += getColumnIndex(i);
			horizontalGreenVal += getRowIndex(i);
		}
		if (tempBoard[i] == 'R'){
			//visitedRed--;
			verticalRedVal += getColumnIndex(i);
			horizontalRedVal += getRowIndex(i);
		}
	}
	
	// Return the heuristic value
	return (HEURISTIC_MULTIPLIER_100 * horizontalGreenVal) + (HEURISTIC_MULTIPLIER_50 * verticalGreenVal) - (HEURISTIC_MULTIPLIER_100 * horizontalRedVal) - (HEURISTIC_MULTIPLIER_50 * verticalRedVal);
}

// Takes in a board index and returns its row index # (i.e. 27 returns 4 since D = 4)
// TODO make this into a struct to return both row and column index
int getRowIndex(int index){
	float val = index / ROW_LENGTH;
	
	// A = 1, B = 2, C = 3, D = 4, E = 5
	if (val < 0 || val > 5) {
		return -1;
	}
	else {
		return val + 1;
	}
}

// Takes in a board index and returns its column index # (i.e. 27 returns 1 because it's in the 1st column)
int getColumnIndex(int index){
	return (index % ROW_LENGTH) + 1;
}

