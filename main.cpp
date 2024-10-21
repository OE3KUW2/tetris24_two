/*****************************************************************************
 * firstSFML
 * sept 24 - OE3KUW / tetris24
 *****************************************************************************/
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

#define ROWS           30
#define COLS           20
#define WIDTH         600
#define HEIGHT        800

int shadow[ROWS][COLS] = {{0}};

#define RED           1

// prototypes:
void drawSquare(int y, int x, int color, RenderWindow * w);



int main()
{
    int x, y;
    int d;

    RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "TETRIS 2CHELS");

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
        drawSquare(3,3,RED, &window);  // y, x, c
        drawSquare(3,5,RED, &window);
        drawSquare(3,7,RED, &window);
        drawSquare(3,15,RED, &window);
        drawSquare(3,19,RED, &window);


        window.display();
        sleep(milliseconds(50));

        cyanRect.setPosition(x, y);
        if (y < 900) y +=  d;

    }

    return 0;
}



void drawSquare(int y, int x, int color, RenderWindow * w)
{

    RectangleShape Rect(sf::Vector2f((float)WIDTH/COLS, (float)HEIGHT/ROWS));
    switch (color)
    {
        case RED: Rect.setFillColor(sf::Color::Red);
    }
    Rect.setPosition((int)(x * (float)WIDTH/COLS), (int)( y * (float)HEIGHT/ROWS));
    w->draw(Rect);
}
