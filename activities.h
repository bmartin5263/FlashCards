//
// Created by Brandon Martin on 7/13/18.
//

#ifndef FLASHCARDS_ACTIVITES_H
#define FLASHCARDS_ACTIVITES_H

#include "ui.h"

class DeckLister {

public:

    enum class ReturnCode {
        NORMAL, ABORTED, INVALID_DECK_INDEX
    };

    DeckLister(DeckLinkedList* deckList);
    DeckLister(const DeckLister&) = delete;
    DeckLister operator = (const DeckLister&) = delete;
    ~DeckLister();

    void launch();
    void handleInput(int input);
    void moveButtonPointer(int direction);
    void updateButtonStates();
    void removeDeck();
    void openDeck();
    void nextDeck();
    void prevDeck();
    void editDeck();
    void newDeck();
    ReturnCode getReturnCode();

private:

    DeckLinkedList* deckList;
    UI* ui;
    ButtonState buttonMap[NUM_BUTTONS];
    ReturnCode returnCode;
    int buttonPointer;
    int deckIndex;
    bool running;
    bool editingDeck;
    bool creatingNewDeck;

};

class CardViewer {

public:

    enum class ReturnCode {
        NORMAL, ABORTED
    };

    CardViewer(Deck* currentDeck);
    CardViewer(const CardViewer&) = delete;
    CardViewer operator = (const CardViewer&) = delete;
    ~CardViewer();

    void launch();
    void updateButtonStates();
    void handleInput(int input);
    void moveButtonPointer(int movement);
    void flipCard();
    void nextCard();
    void prevCard();
    void modifyDeck();
    void newCard();
    void quiz();
    ReturnCode getReturnCode();

private:

    ReturnCode returnCode;
    Deck* deck;
    UI* ui;
    ButtonState buttonMap[NUM_BUTTONS];
    Colors frontColor;
    Colors backColor;
    int buttonPointer;
    int cardIndex;
    bool isFront;
    bool running;
    bool creatingCard;
};

class CardLister {

public:

    enum class ReturnCode {
        NORMAL, ABORTED, CHANGE_DECK
    };

    CardLister(Deck* currentDeck, int startIndex);
    CardLister(const CardLister&) = delete;
    CardLister operator = (const CardLister&) = delete;
    ~CardLister();

    void launch();
    void updateButtonStates();
    void handleInput(int input);
    void moveButtonPointer(int movement);
    void newCard();
    void deleteCard();
    void editCard();
    void nextCard();
    void prevCard();
    int getIndex();
    ReturnCode getReturnCode();

private:

    ReturnCode returnCode;
    Deck* deck;
    UI* ui;
    ButtonState buttonMap[NUM_BUTTONS];
    int buttonPointer;
    int cardIndex;
    bool running;
    bool editingCard;
    bool creatingCard;

};

class DeckEditor {

public:

    enum class ReturnCode {
        NO_CHANGE, ABORTED, EDITED_DECK
    };

    DeckEditor(Deck* deck);
    DeckEditor(const DeckEditor&) = delete;
    DeckEditor operator = (const DeckEditor&) = delete;
    ~DeckEditor();

    void launch();
    bool checkFields();
    void changeScheme();
    void changeSchemeFieldText();
    void handleInput(int input);
    void nextField();
    void previousField();
    void modifyField(int input);
    void setCursor();
    void save();
    ReturnCode getReturnCode();
    char* getName();
    char* getFrontTitle();
    char* getBackTitle();
    ColorSchemes getColorScheme();

private:

    UI* ui;
    char name[MAX_DECK_INPUT_LEN + 1] = {0};
    char frontTitle[MAX_DECK_INPUT_LEN + 1] = {0};
    char backTitle[MAX_DECK_INPUT_LEN + 1] = {0};
    char schemeField[MAX_DECK_INPUT_LEN + 1] = {0};
    int nameSize;
    int frontSize;
    int backSize;
    ColorSchemes colorScheme;
    ReturnCode returnCode;
    int fieldIndex;
    bool running;

};

class CardEditor {

public:

    enum class ReturnCode {
        NO_CHANGE, ABORTED, EDITED_CARD
    };

    CardEditor(Card* card);
    CardEditor(const CardEditor&) = delete;
    CardEditor operator = (const CardEditor&) = delete;
    ~CardEditor();

    void launch();
    ReturnCode getReturnCode();
    char* getFrontSubtitle();
    char* getFrontTitle();
    char* getBackTitle();
    char* getBackSubtitle();

private:

    bool checkFields();
    void handleInput(int input);
    void nextField();
    void previousField();
    void modifyField(int input);
    void setCursor();
    void save();

    UI* ui;
    char frontTitle[MAX_CARD_INPUT_LEN + 1] = {0};
    char frontSubtitle[MAX_CARD_INPUT_LEN + 1] = {0};
    char backTitle[MAX_CARD_INPUT_LEN + 1] = {0};
    char backSubtitle[MAX_CARD_INPUT_LEN + 1] = {0};
    int frontTitleSize;
    int frontSubtitleSize;
    int backTitleSize;
    int backSubtitleSize;
    ReturnCode returnCode;
    int fieldIndex;
    bool running;

};

class DeckQuiz {

public:

    enum class ReturnCode {
        NORMAL, ABORTED
    };

    DeckQuiz(Deck* deck);
    DeckQuiz(const DeckQuiz&) = delete;
    DeckQuiz operator = (const DeckQuiz&) = delete;
    ~DeckQuiz();

    void launch();

private:

    void handleInput(int input);
    void updateButtonStates();
    void moveButtonPointer(int movement);

    Card** quizDeck;
    UI* ui;
    Deck* deckRef;
    ButtonState buttonMap[NUM_BUTTONS];
    Colors frontColor;
    Colors backColor;
    int buttonPointer;
    int cardIndex;
    char inputField[INPUT_FIELD_LENGTH + 1] = {0};
    bool givingInput;
    bool isFront;
    bool running;
    ReturnCode returnCode;

};

#endif //FLASHCARDS_ACTIVITES_H
