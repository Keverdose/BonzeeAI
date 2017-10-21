#include <iostream>
#include <string>

// -- Namespaces
using std::cout;
using std::endl;
using std::cin;
using std::string;

// -- Variable definitions
static const int MAX_BOARD_SIZE = 45;
static const int ROW_LENGTH = 9;
static const int COLUMN_LENGTH = 5;
static const int MAX_PIECES_NUM = 22;

static int greenCounter = MAX_PIECES_NUM;
static int redCounter = MAX_PIECES_NUM;
static bool isPlayerOne = true;

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


// Board setting to win in one move
/*char board[MAX_BOARD_SIZE] = {
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'G', 'G', 'G', 'G', ' ', 'R', 'R', 'R', ' ',
	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G' };
	
redCounter = 3;
	*/

// -- Function definitions
void PrintBoard();
void ProcessMoveRequest();
bool isGameOver();
bool availableSpace(int);
bool isValid(char);
bool IsValidChoice(string);
bool destinationCheck(int, int);
int BoardToIndex(string);
void attacking(int, int);
bool adjacent(int, int);
void tokenCountUpdate();


int main() {

	while (!isGameOver()) {
		if (isPlayerOne)
			cout << "Player One's Turn\n\n";
		
		else
			cout << "Player Two's Turn\n\n";
		
		PrintBoard();
		ProcessMoveRequest();
		cin.get();
		isPlayerOne= !isPlayerOne;
	}

	// Post Game Display
	if (greenCounter <= 0)
		cout << "Game Over Red Wins!";

	else if (redCounter <= 0)
		cout << "Game Over Green Wins!";

	cin.get();
}


// Function to check if game is over or not.
bool isGameOver() {
	return (greenCounter <= 0 || redCounter <= 0);
}


// Function to Print Current Board Configuration
void PrintBoard() {

	cout << " ============================================================ " << endl;
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

	cout << "\n\n ============================================================\n" << endl;

}


void ProcessMoveRequest() {

	bool completedTurn = false;
	string answer;

	do {
		cout << endl << "Please enter move: ";
		getline(cin, answer);

		// Input Validation
		if (answer.length() != 5)
			cout << "Invalid String, please try again." << endl;

		else if (answer.at(2) != ' ')
			cout << "Invalid String, please try again." << endl;


		// Process Input
		else {
			// Transforms the answer into two coordinates
			int choIndex, destIndex;
			string choice = { answer.at(0), answer.at(1) };
			string destination = { answer.at(3), answer.at(4) };

			cout << "Position: " << choice << ", Destination: " << destination << endl;

			// Checks if the two coordinates are within the array, if so then continue. else, prompt again
			if (IsValidChoice(choice) && IsValidChoice(destination)) {
				choIndex = BoardToIndex(choice);
				destIndex = BoardToIndex(destination);

				// Checks if selected token is valid, if there's available move for choindex, and if the destination is valid.

				/*if (isValid(board[choIndex]) && availableSpace(choIndex) && destinationCheck(choIndex, destIndex)) {*/

				if (isValid(board[choIndex]) && availableSpace(choIndex) && destinationCheck(choIndex, destIndex)) {

					// Execute Attack Algorithm
					attacking(choIndex, destIndex);

					// Move attacking token forward/backward 
					board[destIndex] = board[choIndex];
					board[choIndex] = ' ';
					completedTurn = true;
				}

				else
					cout << "This is an invalid move for the player. Please try again." << endl;
			}
			else
				cout << "Invalid positions, please try again." << endl;
		}

		PrintBoard();

	} while (!completedTurn);
}


// Function that returns boolean to check whether the selected token has an available move
bool availableSpace(int currentPosition) {

	// If the current position is out of bound
	if (currentPosition < 0 || currentPosition > 44) 
		return false;

		
	// Checks downwards
	if (currentPosition + ROW_LENGTH < MAX_BOARD_SIZE) 
		if (board[currentPosition + ROW_LENGTH] == ' ') 
			return true; 
		
	// Checks upwards
	if (currentPosition - ROW_LENGTH > -1) 
		if (board[currentPosition - 9] == ' ') 
			return true; 
		
	// Checks right (Makes sure the rightmost column can't go right)
	if ((currentPosition % ROW_LENGTH + 1) != ROW_LENGTH)  
		if (board[currentPosition + 1] == ' ') 
			return true; 
		
	// Checks left (Makes sure the leftmost column can't go left)
	if ((currentPosition % ROW_LENGTH) != 0)  
		if (board[currentPosition - 1] == ' ') 
			return true; 
		

	// IF CELL IS BLACK (DIAGONAL CHECKS)
	if (currentPosition % 2 == 0) {

		// Checks topleft
		if (currentPosition - (ROW_LENGTH + 1) > 0 && (currentPosition % ROW_LENGTH) != 0) 
			if (board[currentPosition - (ROW_LENGTH + 1)] == ' ') 
				return true;
			
		// Checks topright
		if (currentPosition - (ROW_LENGTH - 1) > 0 && (currentPosition % ROW_LENGTH + 1) != ROW_LENGTH) 
			if (board[currentPosition - (ROW_LENGTH - 1)] == ' ') 
				return true;
			
		// Checks bottomleft
		if ((currentPosition % ROW_LENGTH) != 0 && (currentPosition + (ROW_LENGTH - 1) < MAX_BOARD_SIZE)) 
			if (board[currentPosition + (ROW_LENGTH - 1)] == ' ') 
				return true;
			
		// Checks bottomright
		if ((currentPosition + (ROW_LENGTH + 1) < MAX_BOARD_SIZE) && (currentPosition % ROW_LENGTH + 1) != ROW_LENGTH) 
			if (board[currentPosition + (ROW_LENGTH + 1)] == ' ') 
				return true;

	}

	return false;
}


// Checks whether it is a valid destination 
bool destinationCheck(int position, int destination) {

	// If cell occupied, return false;
	if (board[destination] != ' ') 
		return false; 

	//Else check if the cell is adjacent to position.
	else 
		return adjacent(position, destination);
}



bool adjacent(int position, int destination) {

	// Have to make sure that the 1st column and last column cant go left or right
	if (position % ROW_LENGTH == 0) 
		return(position + 1 == destination || position + 10 == destination || position - 9 == destination || position + 9 == destination || position - 8 == destination);
	
	else if (position % ROW_LENGTH == ROW_LENGTH - 1) 
		return(position - 1 == destination || position - 10 == destination || position - 9 == destination || position + 9 == destination || position + 8 == destination);
	
	return(position + 1 == destination
		|| position - 1 == destination
		|| position - 10 == destination
		|| position + 10 == destination
		|| position - 9 == destination
		|| position + 9 == destination
		|| position - 8 == destination
		|| position + 8 == destination);
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

	if (inputChar == 'a' || inputChar == 'b' || inputChar == 'c' || inputChar == 'd' || inputChar == 'e') {
		if ((int)choice.at(1) - 48 > 0 && (int)choice.at(1) - 48 <= 9)  
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

	return (offset * ROW_LENGTH + (int)choice.at(1) - 48 - 1);
}

// Method to process the attacking
void attacking(int pos, int dest) {
	// to-do Check the pos's color, check if dest + direction is opposite color, if so delete -> Loop until out of bounds of array (Wary of left and right column)

	int direction    = dest - pos;
	int tempPosition = dest;
	int target       = dest + direction;
	char targetColor = board[target];

	// If there's no forward attack
	if (targetColor == ' ' || targetColor == board[pos]) {

		direction *= -1;
		tempPosition = pos;
		target       = pos + direction;
		targetColor  = board[target];
	}
	
	// Begin attack loop
	if (board[pos] != targetColor) {
		while (target < 45 && target > -1 && (targetColor == board[target]) && adjacent(tempPosition, target)) {
			board[target] = ' ';
			tempPosition = target;
			target += direction;

			tokenCountUpdate();
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