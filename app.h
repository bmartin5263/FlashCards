//
// Created by Brandon Martin on 8/5/18.
//

#ifndef FLASHCARDS_APP_H
#define FLASHCARDS_APP_H

#include "objects.h"
#include "ui.h"

class App {

public:

    static bool isValidInput(int input);

    App();
    App(const App&) = delete;
    App operator = (const App&) = delete;
    ~App();

    void run();

private:
    void getTestDeck();
    DeckLinkedList* parseDataFile();

    UI* const ui;
    DeckLinkedList* deckList;

};

#endif //FLASHCARDS_APP_H
