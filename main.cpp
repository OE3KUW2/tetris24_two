/*****************************************************************************
 * firstSFML
 * sept 24 - OE3KUW / tetris24
 *****************************************************************************/
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int main()
{
    int x, y;
    int d;

    RenderWindow window(sf::VideoMode(600, 1000), "TETRIS 2CHELS");

    x = y = 300;
    d = 1;

    RectangleShape whiteRect(sf::Vector2f(200.f, 100.f));
    whiteRect.setFillColor(sf::Color::White);
    whiteRect.setPosition(100.f, 100.f);

    RectangleShape cyanRect(sf::Vector2f(100.f, 50.f));
    cyanRect.setFillColor(sf::Color::Cyan);
    cyanRect.setPosition(350.f, 300.f);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                // Aktion für W-Taste
                std::cout << "Taste W wurde gedrueckt" << std::endl;
                d += 10;
            }

        }



        window.clear();
        window.draw(whiteRect);
        window.draw(cyanRect);
        window.display();
        sleep(milliseconds(50));

        cyanRect.setPosition(x, y);
        if (y < 900) y +=  d;

    }

    return 0;
}
