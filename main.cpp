#include <iostream>
#include <assert.h>
#include "activities.h"
#include "app.h"


App::App() :
    ui(UI::getInstance()), deckList(nullptr) {
}

bool App::isValidInput(int input) {
    return  (input >= 32 && input <= 57) || // numbers and symbols
            (input >= 63 && input <= 90) || // uppercase
            (input >= 97 && input <= 122) ||
            input == DELETE_KEY;
}

void App::run() {
    deckList = parseDataFile();
    getTestDeck();

    ui->startupInitialization();
    ui->assignDeckList(deckList);
    DeckLister dl(deckList);
    dl.launch();
    DeckLister::ReturnCode returnCode = dl.getReturnCode();
    assert(returnCode == DeckLister::ReturnCode::NORMAL);
}

DeckLinkedList* App::parseDataFile() {
    return new DeckLinkedList();
}

void App::getTestDeck() {
    Deck* myDeck = new Deck();
    myDeck->setName("My Awesome Deck");
    myDeck->setFrontTitle("FrontSide");
    myDeck->setBackTitle("BackSide");
    myDeck->setColorScheme(ColorSchemes::RED);
    Card* myCard = new Card();
    myCard->setFrontTitle("Hello ");
    myCard->setBackTitle("My Name Is Brandon");
    myDeck->addCard(myCard);
    Card* myCard2 = new Card();
    myCard2->setFrontTitle("This is my Second Card");
    myCard2->setFrontSubtitle("With a Subtitle");
    myCard2->setBackTitle("How Does It Look?");
    myDeck->addCard(myCard2);
    Card* myCard3 = new Card();
    myCard3->setFrontTitle("This App Was Made Entirely");
    myCard3->setBackTitle("In C++");
    myDeck->addCard(myCard3);
    Deck* myEmptyDeck = new Deck();
    myEmptyDeck->setName("This Deck is Empty");
    myEmptyDeck->setFrontTitle("MyFront");
    myEmptyDeck->setBackTitle("MyBack");
    myEmptyDeck->setColorScheme(ColorSchemes::BLUE);
    deckList->addDeck(myEmptyDeck);
    deckList->addDeck(myDeck);
}

App::~App() {
    UI::releaseInstance();
    assert(UI::referenceCountIsZero());
    delete deckList;
}

int main() {
    App program;
    program.run();
}