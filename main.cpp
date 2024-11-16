/*****************************************************************************
 * T e t r i s
 * sept 24 - OE3KUW / tetris24
 * oct 24  - drawSquare colors added           (Samuel Kirakosyan)
 * nov 16  - drawLine colors added             (Bernhard Lintner) :)
 * nov 16  - drawStageFrame added              (Reinhard Daxböck)
 * nov 16  - drawText now with colors          (Paul Kronawitter)
 *****************************************************************************/
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <thread> // Für std::this_thread::sleep_for
#include <chrono> // Für std::chrono::milliseconds

using namespace sf;

#define ROWS                30 // 5 hidden + 25 visible
#define COLS                20
#define HIDDEN_LINES         5 // line 0,1,2,3,4 are hidden!
#define WIDTH              600
#define HEIGHT             800
#define TLX                100
#define TLY                 50
#define BRX                500
#define BRY                550

#define TLPX               560  // Top Left Preparation window X
#define TLPY               TLY
#define BRPX               700
#define BRPY               190

#define BOX_X              ((BRX - TLX)/COLS)
#define BOX_Y              ((BRY - TLY)/(ROWS - HIDDEN_LINES))
#define BOX_X_SIZE         ((float)(BRX - TLX)/COLS)
#define BOX_Y_SIZE         ((float)(BRY - TLY)/(ROWS - HIDDEN_LINES))

#define TEXT_LENGTH         30
#define N                    7
#define FRAME_WIDTH          3 // the frame will be drawn with more than one line ...


int stage [ROWS][COLS] = {{0}};
int frozen[ROWS][COLS] = {{0}};
int shadow[ROWS][COLS] = {{0}};

int pattern[N][N] = {{0}};

enum COLORS {
    BLACK = 0,
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN,
    TRANSPARENT
};
#define MAX_COLOR TRANSPARENT
// ****************************** globals: *********************************
/// pointer on the window
RenderWindow * w;

// ****************************** prototypes: *********************************
void drawSquare(int y, int x, int color);
void drawLine(int x1, int y1, int x2, int y2, int color); // Lintner
void drawText(char * string, int x, int y, int color); // Kranewitter
void drawStageFrame(int x1, int y1, int x2, int y2); // Daxböck
void drawPreparationWindow(int x1, int y1, int x2, int y2); //  Daxböck

// ******************************   m a i n:  *********************************
int main()
{
    int x, y, color;
    char text[TEXT_LENGTH];
    int count = 0;

    // initialization phase:
    RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "TETRIS 2CHELS 2024");
    w = &window;

    while (window.isOpen())
    {

        // reading key factors:

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                // Aktion für W-Taste
                // std::cout << "Taste W wurde gedrueckt" << std::endl;
                // d += 10;
            }
        }

        window.clear();
        //window.draw(whiteRect);
        //window.draw(cyanRect);

        //drawSquare(5, 0, GREEN);  // y, x, c
        //drawSquare(29, 19, GREEN);  // y, x, c


        for (y = 5; y < ROWS; ++y)
            for (x = 0; x < COLS; ++x)
            {
                color = rand() % MAX_COLOR;
                drawSquare(y, x, color);  // y, x, c
            }


        drawStageFrame(TLX, TLY, BRX, BRY);


        for (y = 5; y < ROWS; ++y)
            for (x = 0; x < COLS; ++x)
            {
                color = rand() % MAX_COLOR;
                drawLine(TLX, TLY, TLX, BRY, color); // x1, y1, x2, y2, c
                drawLine(TLX, BRY, BRX, BRY, color);
                drawLine(BRX, TLY, BRX, BRY, color);
            }
        //drawLine(TLX, TLY, TLX, BRY, WHITE);
        //drawLine(TLX, BRY, BRX, BRY, WHITE);
        //drawLine(BRX, TLY, BRX, BRY, WHITE);

        count++;

        sprintf(text, "count: %d", count);
        drawText(text, TLPX, TLPY, GREEN);

        window.display();
        sleep(milliseconds(1));  // one millisecond
    }

    return 0;
}


// ****************************** functions: **********************************

void drawSquare(int y, int x, int color)
///  paint square; stage position, color
{
    RectangleShape Rect(sf::Vector2f(BOX_X_SIZE, BOX_Y_SIZE ));
    switch (color)
    {
        case BLACK:         Rect.setFillColor(sf::Color::Black);       break;
        case WHITE:         Rect.setFillColor(sf::Color::White);       break;
        case RED:           Rect.setFillColor(sf::Color::Red);         break;
        case GREEN:         Rect.setFillColor(sf::Color::Green);       break;
        case BLUE:          Rect.setFillColor(sf::Color::Blue);        break;
        case YELLOW:        Rect.setFillColor(sf::Color::Yellow);      break;
        case MAGENTA:       Rect.setFillColor(sf::Color::Magenta);     break;
        case CYAN:          Rect.setFillColor(sf::Color::Cyan);        break;
        case TRANSPARENT:   Rect.setFillColor(sf::Color::Transparent); break;
        default: std::cerr << "unknown color used in drawSquare" << std::endl;
    }
    Rect.setPosition((int)(x * BOX_X) + TLX, (int)( (y - HIDDEN_LINES) * BOX_Y) + TLY);
    w->draw(Rect);
}

///  from x1,y1 to x2,y2, color
void drawLine(int x1, int y1, int x2, int y2, int color)
{
    sf::VertexArray line(sf::Lines, 2);

    // Startpunkt der Linie
    line[0].position = sf::Vector2f(x1, y1);
    //line[0].color = sf::Color::White;

    line[1].position = sf::Vector2f(x2, y2);
    //line[1].color = sf::Color::White;

    switch (color)
    {
        case BLACK:         line[0].color = sf::Color::Black;
            line[1].color = sf::Color::Black;       break;
        case WHITE:         line[0].color = sf::Color::White;
            line[1].color = sf::Color::White;       break;
        case RED:           line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;         break;
        case GREEN:         line[0].color = sf::Color::Green;
            line[1].color = sf::Color::Green;       break;
        case BLUE:          line[0].color = sf::Color::Blue;
            line[1].color = sf::Color::Blue;        break;
        case YELLOW:        line[0].color = sf::Color::Yellow;
            line[1].color = sf::Color::Yellow;      break;
        case MAGENTA:       line[0].color = sf::Color::Magenta;
            line[1].color = sf::Color::Magenta;     break;
        case CYAN:          line[0].color = sf::Color::Cyan;
            line[1].color = sf::Color::Cyan;        break;
        case TRANSPARENT:   line[0].color = sf::Color::Transparent;
            line[1].color = sf::Color::Transparent; break;
        default: std::cerr << "this color does not exist in drawLine" << std::endl;
    }

    w->draw(line);

}
///  string x,y left top corner, color
void drawText(char * string, int x, int y, int color)
{
    sf::Font font;
    if (!font.loadFromFile("c:/Windows/Fonts/arial.ttf")) {
        // Fehlerbehandlung, falls die Schriftart nicht geladen werden kann
        // ?
    }

    // Text erstellen
    sf::Text text;
    text.setFont(font); // Schriftart setzen
    text.setString(string); // Textinhalt
    text.setCharacterSize(24); // Schriftgröße in Pixeln

    // integrate here same colors as in drawSquare instead of white....

    switch (color)
    {
        case BLACK:     text.setFillColor(sf::Color::Black);    break;
        case WHITE:     text.setFillColor(sf::Color::White);    break;
        case RED:       text.setFillColor(sf::Color::Red);      break;
        case GREEN:     text.setFillColor(sf::Color::Green);    break;
        case BLUE:      text.setFillColor(sf::Color::Blue);     break;
        case YELLOW:    text.setFillColor(sf::Color::Yellow);   break;
        case MAGENTA:   text.setFillColor(sf::Color::Magenta);  break;
        case CYAN:      text.setFillColor(sf::Color::Cyan);     break;
        case TRANSPARENT:
            text.setFillColor(sf::Color::Transparent);    break;
        default: std::cerr << "unknown color used in drawText" << std::endl;
    }

    text.setStyle(sf::Text::Bold); // Stil (optional)
    text.setPosition(x, y); // x = 200, y = 150
    w->draw(text);

}

void drawStageFrame(int x1, int y1, int x2, int y2)
/// corner top left and bottom right for the stage frame
{
    //drawLine(x1, y1, x2, y2, WHITE);

    for (int i = 0; i < FRAME_WIDTH; i++)
    {
        drawLine(TLX - i, TLY - i, TLX - i, BRY + i, WHITE);
        drawLine(TLX - i, BRY + i, BRX + i, BRY + i, WHITE);
        drawLine(BRX + i, TLY - i, BRX + i, BRY + i, WHITE);
        //drawLine(TLX - i, TLY - i, BRX + i, TLY - i, WHITE);
    }

}
