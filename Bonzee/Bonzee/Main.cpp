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

static const enum Color { R = 'R', G = 'G', E = ' ' };
static bool player = true;

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

Color board[MAX_BOARD_SIZE] = {
	R, R, R, R, R, R, R, R, R,
	R, R, R, R, R, R, R, R, R,
	G, G, G, G, E, R, R, R, R,
	G, G, G, G, G, G, G, G, G,
	G, G, G, G, G, G, G, G, G };

/* // -- char version of board
char board[MAX_BOARD_SIZE] = {
	'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
	'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
	'G', 'G', 'G', 'G', ' ', 'R', 'R', 'R', 'R',
	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
	'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G' };
*/

// -- Function definitions
void PrintBoard();
void ProcessMoveRequest();

int main() {
	PrintBoard();
	ProcessMoveRequest();
	cin.get();
}

// Function to Print Current Board Configuration
void PrintBoard(){

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
			cout << " [ " << (char)board[i] << " ]";
		else
			cout << " ( " << (char)board[i] << " )";
		
	}

	cout << "\n\n ============================================================\n" << endl;

}

// Function that returns boolean to check whether the selected token has an available move
bool availableSpace(int currentPosition) {
	// If the current position is out of bound
	if (currentPosition < 0 || currentPosition > 44) {
		return false;
	}
	// Checks downwards
	if (currentPosition + ROW_LENGTH < MAX_BOARD_SIZE) {
		if (board[currentPosition + ROW_LENGTH] == Color::E) { return true; }
	}
	// Checks upwards
	if (currentPosition - ROW_LENGTH > -1) {
		if (board[currentPosition - 9] == Color::E) { return true; }
	}
	// Checks right
	if ((currentPosition % ROW_LENGTH + 1) != ROW_LENGTH) { // Makes sure the rightmost column can't go right
		if (board[currentPosition + 1] == Color::E) { return true; }
	}
	// Checks left
	if ((currentPosition % ROW_LENGTH) != 0) { // Makes sure the leftmost column can't go left
		if (board[currentPosition - 1] == Color::E) { return true; }
	}
	//IF CELL IS BLACK
	if (currentPosition % 2) {
		// Checks topleft
		if (currentPosition - (ROW_LENGTH + 1) > 0 && (currentPosition % ROW_LENGTH) != 0) {
			if (board[currentPosition - (ROW_LENGTH + 1)] == Color::E) { return true; }
		}
		// Checks topright
		if (currentPosition - (ROW_LENGTH - 1) > 0 && (currentPosition % ROW_LENGTH + 1) != ROW_LENGTH) {
			if (board[currentPosition - (ROW_LENGTH - 1)] == Color::E) { return true; }
		}
		// Checks bottomleft
		if ((currentPosition % ROW_LENGTH) != 0 && (currentPosition + (ROW_LENGTH - 1) < MAX_BOARD_SIZE)) {
			if (board[currentPosition + (ROW_LENGTH - 1)] == Color::E) { return true; }
		}
		// Checks bottomright
		if ((currentPosition + (ROW_LENGTH + 1) < MAX_BOARD_SIZE) && (currentPosition % ROW_LENGTH + 1) != ROW_LENGTH) {
			if (board[currentPosition + (ROW_LENGTH + 1)] == Color::E) { return true; }
		}
	}
	/*if (currentPosition < 36) {
		if (board[currentPosition + ROW_LENGTH] == Color::E) { return true; }
	}
	if (currentPosition > 8) {
		if (board[currentPosition - 9] == Color::E) { return true; }
	}
	if (currentPosition != 8 || currentPosition != 17 || currentPosition != 26 || currentPosition != 35 || currentPosition != 44) {
		if (board[currentPosition + 1] == Color::E) { return true; }
	}
	if (currentPosition != 0 || currentPosition != 9 || currentPosition != 18 || currentPosition != 27 || currentPosition != 36) {
		if (board[currentPosition + 1] == Color::E) { return true; }
	}*/

	return false;
}

void ProcessMoveRequest() {
	cout << endl << "Please enter current piece to move: " << endl;
	string choice;
	getline(cin, choice);
	cout << "Current Piece Selected: ";
	cout << (char(choice.at(0)));
	cout << (char(choice.at(1))) << endl;

	cout << "Please enter which empty space to move to: ";
	string destination;
	getline(cin, destination);
	cout << "Destination Location: ";
	cout << (char(destination.at(0)));
	cout << (char(destination.at(1))) << endl;

	char letter = choice.at(0);
	int number = (int)choice.at(1) - 48;

	//cout << number;
	char destinationLetter = destination.at(0);
	int destinationNumber = (int)destination.at(1) - 48;
	int offset;
	int destinationOffset;

	switch (letter)
	{
	case 'a': offset = 0;
		break;
	case 'b': offset = 9;
		break;
	case 'c': offset = 18;
		break;
	case 'd': offset = 27;
		break;
	case 'e': offset = 36;
		break;
	default: cout << "NO";
	}

	switch (destinationLetter)
	{
	case 'a': destinationOffset = 0;
		break;
	case 'b': destinationOffset = 9;
		break;
	case 'c': destinationOffset = 18;
		break;
	case 'd': destinationOffset = 27;
		break;
	case 'e': destinationOffset = 36;
		break;
	default: cout << "NO";
	}

	Color currentCell = board[offset + (number - 1)];
	Color destinationCell = board[destinationOffset + (destinationNumber - 1)];

	board[offset + (number - 1)] = destinationCell;
	board[destinationOffset + (destinationNumber - 1)] = currentCell;

	PrintBoard();
}