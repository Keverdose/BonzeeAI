#include "gameBoard.h"



gameBoard::gameBoard()
{
}


gameBoard::~gameBoard()
{
}

gameBoard::gameBoard(int wsizex, int wsizey, sf::RenderWindow* windo){
	length = std::min(wsizex, wsizey) / 9;
	offsetx = (wsizex - (length * 9)) / 2;
	offsety = (wsizey - (length * 5)) / 2;
	this->window = windo;
}

void gameBoard::reSize(int wsizex, int wsizey) {

}

void gameBoard::draw() {
	for (int row = 0; row < 5; row++) {
		for (int column = 0; column < 9; column++) {
			sf::RectangleShape square(sf::Vector2f(length, length));
			square.setFillColor(white ? sf::Color::White : sf::Color::Black);
			square.setPosition(this->offsetx + column*length, this->offsety + row*length);
			this->window->draw(square);
			white = !white;
		}
	}
}
