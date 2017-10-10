#pragma once
class gameBoard
{
public:
	gameBoard();
	~gameBoard();

	//Defines the board size and each cell size
	enum{BOARD_WIDTH = 9, BOARD_HEIGHT = 5, CELL_SIZE = 50 };
	// A status to see if the baord is occupied
	enum BoardStatus{EMPTY, OCCUPIED};

};

