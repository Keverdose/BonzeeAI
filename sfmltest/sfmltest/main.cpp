#include <SFML/Graphics.hpp>
#include <algorithm>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Chess");

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

		//Relevant Code:
		sf::Vector2u wsize = window.getSize();

		int length = std::min(wsize.x, wsize.y) / 8;
		int offsetx = (wsize.x - (length * 8)) / 2;
		int offsety = (wsize.y - (length * 8)) / 2;

		bool white = true;

		for (int row = 0; row < 8; row++)
		{
			for (int col = 0; col < 8; col++)
			{
				sf::RectangleShape box(sf::Vector2f(length, length));
				box.setFillColor(white ? sf::Color::White : sf::Color::Black);
				box.setPosition(offsetx + col * length, offsety + row * length);
				window.draw(box);
				white = !white;
			}

			white = !white;
		}
		//Relevant Code End

		window.display();
	}
}