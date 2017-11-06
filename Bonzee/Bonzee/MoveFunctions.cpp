#include "MoveFunctions.h"


bool MoveFunctions::IsValidChoice(string choice)
{
	if (choice == "") 
		return false; 

	char inputChar = choice.at(0);

	if (inputChar == 'A' || inputChar == 'B' || inputChar == 'C' || inputChar == 'D' || inputChar == 'E') {
		if ((int)choice.at(1) - ASCII_LETTER_OFFSET > 0 && (int)choice.at(1) - ASCII_LETTER_OFFSET <= ROW_LENGTH)
			return true; 
	}

	return false;
}

int MoveFunctions::BoardToIndex(string choice){
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

bool MoveFunctions::adjacent(int position, int destination, std::map<int, std::vector<int>>& adjacentCells)
{
	for (int i = 0; i < adjacentCells[position].size(); i++) {
		if (adjacentCells[position].at(i) == destination)
			return true;
	}

	return false;
}

bool MoveFunctions::destinationCheck(int position, int destination, char* board, std::map<int, std::vector<int>> adjacentCells)
{

	// If cell occupied, return false;
	if (board[destination] != ' ')
		return false;

	//Else check if the cell is adjacent to position.
	else
		return MoveFunctions::adjacent(position, destination, adjacentCells);
}

void MoveFunctions::attacking(Move inputMove, char* currentBoard, std::map<int, std::vector<int>> adjacentCells)
{

	// Get the move indices
	int dest = inputMove.destination;
	int pos = inputMove.start;

	// Move attacking token forward/backward 
	char playerToken = currentBoard[pos];
	currentBoard[dest] = currentBoard[pos];
	currentBoard[pos] = ' ';

	// Forward Attack 
	int direction = dest - pos;
	int tempPosition = dest;
	int target = dest + direction;
	char targetColor = currentBoard[target];

	// Backward attack: Check If destination is on board edge AND If target cell is empty or same color as initial position token
	if (target < 0 || target > MAX_BOARD_SIZE || targetColor == ' ' || targetColor == playerToken || (dest % ROW_LENGTH == 0 && (direction == -1 || direction == -(ROW_LENGTH + 1) || direction == (ROW_LENGTH - 1))) || (dest % ROW_LENGTH == (ROW_LENGTH - 1) && (direction == 1 || direction == (ROW_LENGTH + 1) || direction == -(ROW_LENGTH - 1)))) {
		direction *= -1;
		tempPosition = pos;
		target = pos + direction;
		targetColor = currentBoard[target];
	}

	// Begin attack loop
	if (currentBoard[pos] != targetColor) {
		while (target < MAX_BOARD_SIZE && target > -1 && (targetColor == currentBoard[target]) && MoveFunctions::adjacent(tempPosition, target, adjacentCells) && currentBoard[target] != ' ' && currentBoard[target] != playerToken) {
			currentBoard[target] = ' ';
			tempPosition = target;
			target += direction;
			//MoveFunctions::tokenCountUpdate(greenCounter, redCounter, isPlayerOne);
		}
	}
}

vector<int> MoveFunctions::checkPossibleMoves(int index, char* updatedBoard, std::map<int, std::vector<int>> adjacentCells)
{
	std::vector<int> moves;

	for (int i = 0; i < adjacentCells[index].size(); i++) {
		if (updatedBoard[adjacentCells[index].at(i)] == ' ')
			moves.push_back(adjacentCells[index].at(i));
	}

	return moves;
}

vector<Move> MoveFunctions::getAllMoves(char* tempBoard, bool player, std::map<int, std::vector<int>> adjacentCells)
{
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
				vector<int> possibleMovesDestination = checkPossibleMoves(i, tempBoard, adjacentCells);

				// Loops through all available adjacents and stores all valid moves
				for (int j = 0; j < possibleMovesDestination.size(); j++) {
					temp.destination = possibleMovesDestination[j];
					allMoves.push_back(temp);
				}
			}
		}
	}
	return allMoves;
}

void MoveFunctions::ProcessMoveRequest(bool isPlayerOne, bool aiTurn, bool singlePlayer, char* board, std::map<int, std::vector<int>> adjacentCells) {

	bool completedTurn = false;
	string answer;

	do {
		// Check if current turn is ai's turn, and it's in singleplayer mode
		if (isPlayerOne == aiTurn && singlePlayer) {

			// Start Timer before AI checks for best move
			clock_t start = clock();

			// Get the best move from the ai (Via recursion)
			Move aiMove = HeuristicFunctions::getAiMove(DEPTH, board, aiTurn, adjacentCells);

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
						// cout << "  Red: " << redCounter << ", Green: " << greenCounter << endl;
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