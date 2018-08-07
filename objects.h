//
// Created by Brandon Martin on 7/22/18.
//

#ifndef FLASHCARDS_OBJECTS_H
#define FLASHCARDS_OBJECTS_H

#include "constants.h"

enum class ColorSchemes {
    WHITE, BLUE, RED
};

class Card {

public:

    Card();
    ~Card();

    void setFrontTitle(const char* frontTitle);
    void setBackTitle(const char* backTitle);
    void setFrontSubtitle(const char* frontSubtitle);
    void setBackSubtitle(const char* backSubtitle);
    char* getFrontTitle();
    char* getBackTitle();
    char* getFrontSubtitle();
    char* getBackSubtitle();
    int getFrontTitleLength();
    int getBackTitleLength();
    int getFrontSubtitleLength();
    int getBackSubtitleLength();

private:

    char frontTitle[MAX_CARD_INPUT_LEN + 1] = {0};
    char backTitle[MAX_CARD_INPUT_LEN + 1] = {0};
    char frontSubtitle[MAX_CARD_INPUT_LEN + 1] = {0};
    char backSubtitle[MAX_CARD_INPUT_LEN + 1] = {0};
    int frontTitleLen;
    int backTitleLen;
    int frontSubtitleLen;
    int backSubtitleLen;

};

class Deck {

public:

    Deck();
    ~Deck();

    void setName(const char* name);
    char* getName();
    void setFrontTitle(const char* front);
    void setBackTitle(const char* back);
    char* getFrontTitle();
    char* getBackTitle();
    void setColorScheme(ColorSchemes scheme);
    ColorSchemes getColorScheme();
    int getSize();
    void addCard(Card* card);
    void removeCard(int index);
    Card* getCard(int index);

private:

    Card* cards[MAX_CARDS] = {0};
    char name[MAX_DECK_INPUT_LEN + 1] = {0};
    char frontTitle[MAX_DECK_INPUT_LEN + 1] = {0};
    char backTitle[MAX_DECK_INPUT_LEN + 1] = {0};
    ColorSchemes colorScheme;
    int size;

};

class DeckNode {
public:
    DeckNode(Deck* deck, DeckNode* next, DeckNode* prev);
    ~DeckNode();

    Deck* deck;
    DeckNode* next;
    DeckNode* prev;
};

class DeckLinkedList {

public:

    DeckLinkedList();
    ~DeckLinkedList();

    void addDeck(Deck* deck);
    void removeDeck(int index);
    Deck* getDeck(int index);
    int getSize();
    DeckNode* getHead();


private:

    DeckNode* head;
    int size;

};

#endif //FLASHCARDS_OBJECTS_H
