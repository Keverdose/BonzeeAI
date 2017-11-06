#include "BoardFunctions.h"

void BoardFunctions::PrintBoard(char* board)
{
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
	cout << "\n ============================================================\n" << endl;
}

// Check if either player wins.
bool BoardFunctions::winningBoard(char* board) {
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

bool BoardFunctions::isValid(bool isPlayerOne, char color)
{
	if (isPlayerOne)
		return (color == 'G');
	else
		return (color == 'R');
}
