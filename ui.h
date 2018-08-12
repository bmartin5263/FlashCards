//
// Created by Brandon Martin on 7/14/18.
//

#ifndef FLASHCARDS_UI_H
#define FLASHCARDS_UI_H

#include "ui_support.h"

#define CONSOLE_LABEL_LEN 48
#define DECK_LABEL_LEN 32
#define DECK_SIZE_LABEL_LEN 2
#define SIDE_LABEL_LEN 34
#define DECK_METER_LEN 48
#define INPUT_LABEL_LEN 48
#define ARROW_BUTTON_LEN 7
#define FLIP_BUTTON_LEN 14
#define ODD_BUTTON_LEN 17
#define EVEN_BUTTON_LEN 16
#define CARD_BOX_HEIGHT 9
#define CARD_BOX_LENGTH 44
#define DECK_TABLE_LENGTH 48
#define DECK_TABLE_HEIGHT 13
#define FORM_LENGTH 48
#define FORM_HEIGHT 5

#define WHITE_VAL 15
#define BLUE_VAL 39
#define RED_VAL 199
#define GREEN_VAL 82
#define YELLOW_VAL 226
#define GRAY_VAL 8

#define BLUE_BG 12
#define CARD_BLUE_FRONT 111
#define CARD_BLUE_BACK 123
#define CARD_RED_FRONT 209
#define CARD_RED_BACK 134

// Deck Color Schemes



enum class ButtonGroups {
    NONE, CARD_VIEW, DECK_LIST, CARD_LIST, QUIZ
};

/*
 * Class: UI
 *
 * The UI class provides an API for updating and drawing various elements and canvases and manipulate
 * them through predefined public methods specific to the program and helper private methods for performing
 * various curses tasks.
 */
class UI {

public:

    static UI* INSTANCE;
    static int REF_COUNT;

    static UI* getInstance();
    static void releaseInstance();
    static bool referenceCountIsZero();

    static unsigned int getPairNumber(Colors);
    static unsigned int getColorPair(Colors);
    static unsigned int getAttribute(Attributes);
    static int getMovement(int input);
    static void resolveColorScheme(ColorSchemes scheme, Colors &front, Colors &back);

    void draw();

    void assignDeckList(DeckLinkedList* deckList);
    void setDeckListIndex(int index);
    void setCardListIndex(int index);
    void setEditorIndex(int index);
    void setCardSide(bool isFront, const char* sideName, Colors color);
    void setDeckPosition(int index);

    void activateCursorForEditor();
    void activateCursorForQuiz();
    void moveCursor(int x, int y);
    void deactivateCursor();

    void injectCurrentDeck(Deck* deck);
    void initializeDeckEditor();
    void initializeCardEditor();
    void injectCurrentCard(Card* card, int index);
    void injectFields(char* field1, char* field2, char* field3, char* field4);

    void console(const char* message);
    void error(const char* message);

    void openDeckLister();
    void closeDeckLister();
    void openCardViewer();
    void closeCardViewer();
    void openCardLister();
    void closeCardLister();
    void openDeckEditor();
    void closeDeckEditor();
    void openCardEditorFromList();
    void closeCardEditorFromList();
    void openQuiz();
    void closeQuiz();

    void resetDeckMeterColors();
    void setDeckMeterTickColor(int index, Colors color);
    void setInputFieldText(char* text);
    void setInputFieldCorrect();
    void setInputFieldIncorrect();
    void resetInputField();

    void updateButtonState(ButtonGroups group, ButtonState states[NUM_BUTTONS]);
    void startupInitialization();

private:

    // Big Four
    UI();
    UI(const UI &other);
    UI& operator = (const UI &t);
    ~UI();

    void initializeCanvases();
    void initializeCurses();
    void initializeElements();

    void closeButtonGroup(ButtonGroups group);
    void openButtonGroup(ButtonGroups group);
    void switchButtonGroup(ButtonGroups group);
    void activateCursor(Canvas* hostCanvas);

    /* Canvases */
    Canvas* C_CONSOLE;
    Canvas* C_BUTTONS;
    Canvas* C_CARD_VIEW;
    Canvas* C_LIST_VIEW;
    Canvas* C_EDIT_VIEW;

    /* Element */
    Label* E_CONSOLE;
    Label* E_DECK_NAME;
    Label* E_DECK_SIZE;
    Label* E_SIDE_NAME;
    Label* E_CARD_NUM;
    Label* E_INPUT;
    DeckMeter* E_DECK_METER;
    Button* E_LEFT_ARROW;
    Button* E_RIGHT_ARROW;
    Button* E_FLIP_BUTTON;
    Button* E_EDIT_BUTTON;
    Button* E_EXIT_BUTTON;
    Button* E_QUIZ_BUTTON;
    Button* E_END_QUIZ_BUTTON;
    Button* E_CHANGE_DECK_BUTTON;
    Button* E_MODIFY_DECK_BUTTON;
    Button* E_ADD_CARD_BUTTON;
    Button* E_DELETE_BUTTON;
    Button* E_CARD_VIEW_BUTTON;
    Button* E_NEW_DECK_BUTTON;
    Button* E_DELETE_DECK_BUTTON;
    Button* E_EDIT_DECK_BUTTON;
    Button* E_OPEN_BUTTON;
    Button* E_ANSWER_BUTTON;
    CardBox* E_CARD_BOX;
    DeckTable* E_DECK_TABLE;
    CardTable* E_CARD_TABLE;
    Form* E_EDIT_FORM;

    ButtonGroups currentButtonGroup;

    Canvas* cursorCanvas;
    int cardViewButtons[NUM_BUTTONS];
    int deckListButtons[NUM_BUTTONS];
    int cardListButtons[NUM_BUTTONS];
    int quizButtons[NUM_BUTTONS];
    int cursorX;
    int cursorY;
    bool cursorActive;

};


#endif //FLASHCARDS_UI_H
