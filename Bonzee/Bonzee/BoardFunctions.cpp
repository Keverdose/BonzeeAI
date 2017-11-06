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

void BoardFunctions::generateMap(int index, int boardSize, int row_length, std::map<int, std::vector<int>>& adjacentCells)
{
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
	if (index >(row_length - 1))
		adjacentCells[index].push_back(index - row_length);
	// Add Bottom Index
	if (index < (boardSize - row_length))
		adjacentCells[index].push_back(index + row_length);
}

void BoardFunctions::singleOrMultiplayer(bool& aiTurn, bool& singlePlayer)
{
	bool modeChosen = false;

	do {
		// Ask and process input
		cout << "Would you like to play singleplayer (vs AI) or multiplayer (vs Human) (S/M): ";
		string answer = "";
		getline(cin, answer);
		transform(answer.begin(), answer.end(), answer.begin(), ::toupper);

		// Input Check
		if (answer.length() != 1) {
			cout << "Invalid choice, please try again: ";
		}

		else {
			// If user chooses singleplayer mode, make him choose the AI's Token.
			if (answer.at(0) == 'S') {

				bool aiTokenChosen = false;

				do {
					// Ask and process input
					cout << "> Singleplayer mode chosen. \n> You will play against a minimax A.I. \n\nWhich token will the A.I. play? (R/G): ";
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
