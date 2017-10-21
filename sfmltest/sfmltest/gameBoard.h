#pragma once
#include <algorithm>
#include <SFML\Graphics.hpp>
class gameBoard
{
public:
	// Default constructor
	gameBoard();

	// Destructor
	~gameBoard();

	// gameBoard constructor
	gameBoard(int wsizex, int wsizey, sf::RenderWindow* window);

	// Draw the gameboard onto the window
	void draw();
	
	// Mutators
	void reSize(int wsizex, int wsizey);

private:
	int length;
	int offsetx;
	int offsety;
	bool white = false;
	sf::RenderWindow* window;
};

