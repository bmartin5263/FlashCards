//
// Created by Brandon Martin on 7/14/18.
//

#ifndef FLASHCARDS_UI_SUPPORT_H
#define FLASHCARDS_UI_SUPPORT_H

#include <ncurses.h>
#include <panel.h>
#include "objects.h"

enum class Colors {
    BLUE, RED, GREEN, YELLOW, WHITE, GRAY, INVERT, DEFAULT,
    WHITE_HL, YELLOW_HL, C_BLUE_FRONT, C_BLUE_BACK, C_RED_FRONT, C_RED_BACK
};

enum class Attributes {
    NORMAL, BLINK
};

enum class Borders {
    TOP, MIDDLE, BOTTOM, FLOAT, NONE
};

class Element {

public:
    explicit Element(int x, int y, int length, int height);
    virtual ~Element() = default;

    virtual void render(WINDOW*) = 0;

    void getXY(int &x, int &y);
    //void move(int x, int y);
    void getDimensions(int &length, int &height);
    int getLength();

    virtual void hide(WINDOW*) = 0;
    virtual void show() = 0;
    virtual void freeze();
    virtual void thaw();
    void setHidden(bool hidden);
    bool isHidden();

private:
    int x;
    int y;
    int length;
    int height;
    bool hidden;

};

class Label : public Element {

public:

    Label(int x, int y, int length, Colors color, Attributes attribute);
    ~Label();

    void hide(WINDOW*);
    void show();
    void render(WINDOW*);
    void setText(const char* text);
    void setText(const char* text, Colors color, Attributes attribute);
    void setColor(Colors color);
    void setAttribute(Attributes attr);
    void clear(WINDOW* window);

private:

    char* body;
    Colors color;
    Attributes attribute;

};

class DeckMeter : public Element {
public:

    DeckMeter(int x, int y, int length, Colors color);
    ~DeckMeter();

    void setPosition(const int pos);
    void setDeck(Deck* deck);
    void setColorForTick(const int index, Colors color);
    void resetColors();
    void setColor(Colors color);
    void render(WINDOW*);
    void hide(WINDOW*);
    void show();
    void clear(WINDOW* window);

private:

    Colors* colorMap;
    Deck* deck;
    Colors color;
    int size;
    int position;

};

class Button : public Element {

public:

    Button(int x, int y, int length, const char* text, int textLen, Colors color, int textHighlight);
    ~Button();

    void render(WINDOW* window);

    void setState(ButtonState state);
    void reset();
    void hide(WINDOW*);
    void show();
    void clear(WINDOW* window);

private:

    char* label;
    int labelLen;
    int textHighlightOffset;
    Colors color;
    ButtonState state;

};

class CardBox : public Element {

public:

    CardBox(int x, int y, int length);
    ~CardBox();

    void render(WINDOW* window);
    void setCard(Card* card);
    void setColor(Colors color);
    void setSide(bool isFront);
    void hide(WINDOW*);
    void show();
    void clear(WINDOW* window);
    void makeRed();
    void makeGreen();
    void resetColor();

private:

    Card* card;
    Colors color;
    bool front;
    bool isRed;
    bool isGreen;

};

class DeckTable : public Element {

public:

    DeckTable(int x, int y, int length, int height);
    ~DeckTable();

    void render(WINDOW* window);
    void setDeckList(DeckLinkedList* deckList);
    void setIndex(int index);
    void hide(WINDOW*);
    void show();
    void clear(WINDOW* window);

private:

    DeckLinkedList* decks;
    int deckIndex;
};

class CardTable : public Element {

public:
    CardTable(int x, int y, int length, int height);
    ~CardTable();

    void render(WINDOW* window);
    void setDeck(Deck* deck);
    void setIndex(int index);
    void hide(WINDOW*);
    void show();
    void clear(WINDOW* window);

private:
    Deck* deck;
    int cardIndex;

};

class Form : public Element {

public:
    Form(int x, int y, int length, int height);
    ~Form();

    void render(WINDOW* window);
    void initializeForDeck();
    void initializeForCard();
    void setFormFields(char* field1, char* field2, char* field3, char* field4);
    void setIndex(int index);
    void hide(WINDOW*);
    void show();
    void clear(WINDOW* window);

private:
    char* field1;
    char* field2;
    char* field3;
    char* field4;
    int fieldIndex;
    bool isDeckEdit;
    bool initialized;
};

class QuizModeTable : public Element {

public:

    QuizModeTable(int x, int y, int length, int height);
    ~QuizModeTable();

    void render(WINDOW* window);
    void setIndex(int index);
    void setSideNames(char* front, char* back);
    void hide(WINDOW*);
    void show();
    void clear(WINDOW* window);

private:

    char* frontSide;
    char* backSide;
    int index;
};

/*
 * Class: Canvas
 *
 * Canvases are Window/Panel pairs that carry with them metadata about their size, initial color,
 * and location on the screen. They serve as a drawing board where elements can render their contents
 * onto and can be easily hidden when not in use.
 */
class Canvas {

public:
    Canvas(int locX, int locY, int dimX, int dimY, int maxElements, Colors color, Borders borders);
    ~Canvas();

    // No Copying Right Now...
    Canvas(const Canvas &other) = delete;
    Canvas& operator = (const Canvas & other) = delete;

    void burnRow(int y);
    void burnText(int x, int y, const char* text);
    void burnText(int x, int y, const char* text, Colors color);
    void addElement(Element* e);

    void hide();
    void hideElement(int elementIndex);
    void freezeElement(int elementIndex);
    void showElement(int elementIndex);
    void thawElement(int elementIndex);
    void show();

    Element* getElement(int index);

    void render();
    int isHidden() const;

    WINDOW* window;
    PANEL* panel;

private:
    Element** elements;
    int hidden;
    int x;
    int y;
    int cols;
    int rows;
    int numElements;
    int maxElements;
    Colors defaultColor;
};

#endif //FLASHCARDS_UI_SUPPORT_H
