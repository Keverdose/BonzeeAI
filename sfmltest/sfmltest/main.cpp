#include <SFML/Graphics.hpp>
#include <algorithm>
#include "gameBoard.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "COMP472-SHREK");
	gameBoard* board = new gameBoard(window.getSize().x, window.getSize().y, &window);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
			{
				sf::View view = window.getView();
				view.setCenter(event.size.width / 2, event.size.height / 2);
				view.setSize(event.size.width, event.size.height);
				window.setView(view);
			}
		}

		window.clear(sf::Color::Blue);

		gameBoard* board = new gameBoard(window.getSize().x, window.getSize().y, &window);
		board->draw();

		window.display();
	}
}

////Relevant Code:
//sf::Vector2u wsize = window.getSize();
//
//int length = std::min(wsize.x, wsize.y) / 9;
//int offsetx = (wsize.x - (length * 9)) / 2;
//int offsety = (wsize.y - (length * 5)) / 2;
//
//bool white = false;
//// Create black and white board
//for (int row = 0; row < 5; row++) {
//	for (int col = 0; col < 9; col++) {
//		sf::RectangleShape box(sf::Vector2f(length, length));
//		box.setFillColor(white ? sf::Color::White : sf::Color::Black);
//		box.setPosition(offsetx + col * length, offsety + row * length);
//		window.draw(box);
//		white = !white;
//	}
//}