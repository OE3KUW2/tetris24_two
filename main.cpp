/*****************************************************************************
 * T e t r i s
 * sept 24 - OE3KUW / tetris24
 * oct 24  - drawSquare colors added           (Samuel Kirakosyan)
 * nov 16  - drawLine colors added             (Bernhard Lintner) :)
 * nov 16  - drawStageFrame added              (Reinhard Daxböck)
 * nov 16  - drawText now with colors          (Paul Kronawitter)
 * nov 16  - generate figures started
 *****************************************************************************/
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <thread> // Für std::this_thread::sleep_for
#include <chrono> // Für std::chrono::milliseconds

using namespace sf;

#define TRUE                 1
#define FALSE                0

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

#define TEXT_X            TLPX
#define TEXT_LEVEL        (BRY - 24)

#define MAX_FIGURE          15


int stage [ROWS][COLS] = {{0}};
int frozen[ROWS][COLS] = {{0}};
int shadow[ROWS][COLS] = {{0}};


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
void drawStageFrame(void); // Daxböck
void drawPatternFrame(void);
void drawPattern(int p[][N]);
void drawStage(void);
void drawFrozen(void);


void setNextFigure(int p[][N]);
void setNewCenterPoint(int *y, int *x);

void exchange2Boxes(int p[][N],int y1, int x1, int y2, int x2);
void exchangePatternBoxes(int p[][N],
                          int y1, int x1, int y2, int x2,
                          int y3, int x3, int y4, int x4,
                          int y5, int x5, int y6, int x6);
void rotatePattern(int p[][N]);



// ******************************   m a i n:  *********************************
int main()
{
    int x, y, color, tick = 0;
    char text[TEXT_LENGTH];
    int count = 0;
    int pattern[N][N] = {{0}};
    int refresh = TRUE;
    int work = TRUE;
    int doItOnlyOnce = TRUE;
    Event event;
    int cy, cx; // center point of falling objects

    // initialization phase:
    RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "TETRIS 2CHELS 2024");
    w = &window;


    // inits:
    setNextFigure(pattern);
    setNewCenterPoint(&cy,&cx);

    while (window.isOpen() && work)
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                if (doItOnlyOnce) {
                    // bisherige pattern speichern
                    rotatePattern(pattern);
                    drawPattern(pattern);
                    refresh = TRUE;
                    doItOnlyOnce = FALSE;
                    // falls sich die neue Figur nicht übertragen lässt -> zurück zur gespeicherten

                } else doItOnlyOnce = TRUE;

            } else if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                    count = 10000;
            } else if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                    count = 0;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                count = 0;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                count = -1;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                work = FALSE;
            }

        }


        //window.clear(); ... zu langsam!
        drawStageFrame();
        drawPatternFrame();
        drawPattern(pattern);



        tick++;

        if (tick > 2)
        {
            tick = 0;
            refresh = TRUE;

            if ((stage[cy + 1][cx] == 0)  &&     // anstatt nur das center zu checken - checke 5x5!
                (frozen[cy + 1][cx] == 0) &&
                (cy < (ROWS - 1)))
            {
                // move possible:
                stage[cy+1][cx] = stage[cy][cx]; // alle 5x5 übertragen
                stage[cy][cx] = 0;               // alle 5x5 löschen
                cy++;
            }
            else
            {
                // move nicht mehr möglich!

                frozen[cy][cx] = stage[cy][cx]; // alle 5x5 übertragen
                stage[cy][cx] = 0;              // alle 5x5 löschen
                setNewCenterPoint(&cy,&cx);

                // überprüfe alle Zeilen in frozen auf Völlständigkeit
                // falls Vollständig -> Zeile löschen

            }
            count++;


        }

        drawStage();
        drawFrozen();
        sprintf(text, "count: %d", count);
        drawText(text, TEXT_X, TEXT_LEVEL, GREEN);


        if (refresh) { window.display(); refresh = FALSE;}
        std::  this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}


// ****************************** functions: **********************************

void drawSquare(int y, int x, int color)
///  paint square; stage position, color
{
    // if (shadow[y][x] != color) {


        RectangleShape Rect(sf::Vector2f(BOX_X_SIZE, BOX_Y_SIZE));
        switch (color) {
            case BLACK:
                Rect.setFillColor(sf::Color::Black);
                break;
            case WHITE:
                Rect.setFillColor(sf::Color::White);
                break;
            case RED:
                Rect.setFillColor(sf::Color::Red);
                break;
            case GREEN:
                Rect.setFillColor(sf::Color::Green);
                break;
            case BLUE:
                Rect.setFillColor(sf::Color::Blue);
                break;
            case YELLOW:
                Rect.setFillColor(sf::Color::Yellow);
                break;
            case MAGENTA:
                Rect.setFillColor(sf::Color::Magenta);
                break;
            case CYAN:
                Rect.setFillColor(sf::Color::Cyan);
                break;
            case TRANSPARENT:
                Rect.setFillColor(sf::Color::Transparent);
                break;
            default:
                std::cerr << "unknown color used in drawSquare" << std::endl;
        }
        Rect.setPosition((int) (x * BOX_X) + TLX, (int) ((y - HIDDEN_LINES) * BOX_Y) + TLY);
        w->draw(Rect);

       //  shadow[y][x] != color; }
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

    RectangleShape Rect(sf::Vector2f(150, 24 ));
    Rect.setFillColor(sf::Color::Black);
    Rect.setPosition(x, y);
    w->draw(Rect);


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

void drawStageFrame(void)
{

    for (int i = 0; i < FRAME_WIDTH; i++)
    {
        drawLine(TLX - i, TLY - i, TLX - i, BRY + i, WHITE);
        drawLine(TLX - i, BRY + i, BRX + i, BRY + i, WHITE);
        drawLine(BRX + i, TLY - i, BRX + i, BRY + i, WHITE);
        //drawLine(TLX - i, TLY - i, BRX + i, TLY - i, WHITE);
    }

}

void drawPatternFrame(void)
{
    drawLine(TLPX - 1, TLPY - 1, TLPX - 1, BRPY + 1, WHITE);
    drawLine(TLPX - 1, BRPY + 1, BRPX + 1, BRPY + 1, WHITE);
    drawLine(BRPX + 1, TLPY - 1, BRPX + 1, BRPY + 1, WHITE);
    drawLine(TLPX - 1, TLPY - 1, BRPX + 1, TLPY - 1, WHITE);
}


void drawPattern(int p[][N])
/// pattern - array
{
    int x, y;
    RectangleShape Rect(sf::Vector2f(BOX_X_SIZE, BOX_Y_SIZE ));

    for (y = 0; y < N; y++) for (x = 0; x < N; x++)
    {
        switch (p[y][x])
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
        Rect.setPosition((x * BOX_X) + TLPX,  (y * BOX_Y) + TLPY);
        w->draw(Rect);
    }

}

void setNextFigure(int p[][N])
{
    int x, y;
    int figure;

    for (y = 0; (y) < N; ++y) for (x = 0; x < N; ++x) p[y][x] = 0;

    figure = rand()% MAX_FIGURE;

    figure = 11;

    switch (figure) {

        case  0: p[3][3] = CYAN; break;
        case  1: p[3][3] = RED; p[3][4] = RED; p[4][3] = RED; p[4][4] = RED; break;
        case  2:
        case  3:
        case  4:
        case  5:
        case  6:
        case  7:
        case  8:
        case  9:
        case 10:
        case 11: p[3][3] = YELLOW; p[2][3] = YELLOW; p[3][2] = YELLOW; p[2][4] = YELLOW; break;
        case 12:
        case 13:
        case 14:
        case 15: p[2][2] = BLUE; p[2][3] = BLUE; p[2][4] = BLUE; p[4][2] = BLUE; p[4][3] = BLUE; p[4][4] = BLUE; p[3][4] = BLUE; break;
    }

}
void rotatePattern(int p[][N])
{
    exchangePatternBoxes(p, 1,1,1,5,5,1,1,1,5,5,5,1);
    exchangePatternBoxes(p, 1,2,2,5,4,1,1,2,5,4,4,1);
    exchangePatternBoxes(p, 1,3,3,5,3,1,1,3,5,3,3,1);
    exchangePatternBoxes(p, 1,4,4,5,2,1,1,4,5,2,2,1);
    exchangePatternBoxes(p, 2,2,2,4,4,2,2,2,4,4,4,2);
    exchangePatternBoxes(p, 2,3,3,4,3,2,2,3,4,3,3,2);
}

void exchangePatternBoxes(int p[][N],
                          int y1, int x1, int y2, int x2,
                          int y3, int x3, int y4, int x4,
                          int y5, int x5, int y6, int x6)
{
    exchange2Boxes(p, y1, x1, y2, x2);
    exchange2Boxes(p, y3, x3, y4, x4);
    exchange2Boxes(p, y5, x5, y6, x6);
}

void exchange2Boxes(int p[][N],int y1, int x1, int y2, int x2)
{
    p[y1][x1] ^= p[y2][x2];
    p[y2][x2] ^= p[y1][x1];
    p[y1][x1] ^= p[y2][x2];
}

void setNewCenterPoint(int *y, int *x)
{
    *y = 2;
    *x = rand()% (COLS - 4) + 2;
    stage[*y][*x] = GREEN;
}

void drawStage(void)
{
    int y, x;

    RectangleShape Rect(sf::Vector2f((BRX - TLX), (BRY - TLY - HIDDEN_LINES) ));

    Rect.setFillColor(sf::Color::Black);
    Rect.setPosition(TLX, TLY);
    w->draw(Rect);

    for (y = 5; y < ROWS; ++y)
        for (x = 0; x < COLS; ++x)
        {
            if (stage[y][x]) drawSquare(y, x, stage[y][x]);
        }
}
void drawFrozen(void)
{
    int y, x;

    for (y = 5; y < ROWS; ++y)
        for (x = 0; x < COLS; ++x)
        {
            if (frozen[y][x]) drawSquare(y, x, RED/*frozen[y][x]*/);  // only frozen stones!
        }
}
