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
static const int COL_LENGTH = 5;
static const enum Color { R = 'R', G = 'G', E = ' ' };

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

int main(){
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

void ProcessMoveRequest(){
	cout << endl << "Please enter current piece to move: " << endl;
	string choice;
	getline(cin, choice);
	if (choice.at(0) == 'A'){
		int colChoice = (int) choice.at(1) - 48;
		cout << char(board[colChoice - 1]);
	}
	else if (choice.at(0) == 'B'){
		int colChoice = (int)choice.at(1) - 48;
		cout << char(board[9 + colChoice - 1]);
	}
	else if (choice.at(0) == 'C'){
		int colChoice = (int)choice.at(1) - 48;
		cout << char(board[18 + colChoice - 1]);
	}
	else if (choice.at(0) == 'D'){
		int colChoice = (int)choice.at(1) - 48;
		cout << char(board[27 + colChoice - 1]);
	}
	else{
		int colChoice = (int)choice.at(1) - 48;
		cout << char(board[36 + colChoice - 1]);
	}
}