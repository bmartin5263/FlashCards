//
// Created by Brandon Martin on 7/13/18.
//

#include <assert.h>
#include <string.h>
#include <time.h>
#include <cctype>
#include "activities.h"
#include "app.h"

DeckLister::DeckLister(DeckLinkedList *deckList) :
    returnCode(DeckLister::ReturnCode::ABORTED), deckList(deckList), ui(UI::getInstance()), buttonPointer(-1),
    deckIndex(0), running(false), editingDeck(false), creatingNewDeck(false)
{
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttonMap[i] = ButtonState::DISABLED;
    }
}

DeckLister::ReturnCode DeckLister::getReturnCode() {
    return returnCode;
}

void DeckLister::launch() {

    ui->openDeckLister();

    // Initial Data
    running = true;
    buttonPointer = 2;

    // Prepare User Interface
    ui->setDeckListIndex(deckIndex);
    updateButtonStates();

    while (running) {
        ui->draw();
        int input = getch();
        handleInput(input);
    }

    ui->closeDeckLister();
}

void DeckLister::handleInput(int input) {
    int movement = UI::getMovement(input);
    int beforeDeckSize = deckList->getSize();
    if (movement < 0) {
        if (input == ' ' || input == ENTER_KEY) {
            switch(buttonPointer) {
                case BI_EXIT:
                    returnCode = ReturnCode::NORMAL;
                    running = false;
                    return;;
                case BI_DELETE_DECK:
                    removeDeck();
                    break;
                case BI_LEFT_ARROW:
                    prevDeck();
                    break;
                case BI_RIGHT_ARROW:
                    nextDeck();
                    break;
                case BI_OPEN:
                    openDeck();
                    break;
                case BI_EDIT_DECK:
                    editDeck();
                    break;
                case BI_NEW_DECK:
                    newDeck();
                    if (deckList->getSize() > beforeDeckSize) {
                        openDeck();
                    }
                    break;
            }
        }
        else {
            switch(input) {
                case '.':
                case '>':
                    nextDeck();
                    break;
                case ',':
                case '<':
                    prevDeck();
                    break;
                case 'n':
                case 'N':
                    newDeck();
                    if (deckList->getSize() > beforeDeckSize) {
                        openDeck();
                    }
                    break;
                case 'o':
                case 'O':
                    openDeck();
                    break;
                case 'E':
                case 'e':
                    editDeck();
                    break;
                case 'D':
                case 'd':
                    removeDeck();
                    break;
                case 'x':
                case 'X':
                    returnCode = ReturnCode::NORMAL;
                    running = false;
                    return;
                default:
                    break;

            }
        }
    } else {
        moveButtonPointer(movement);
    }
}

void DeckLister::editDeck() {
    if (deckList->getSize() > 0) {
        editingDeck = true;
        updateButtonStates();
        Deck* deck = deckList->getDeck(deckIndex);
        DeckEditor de(deck);
        de.launch();
        if (de.getReturnCode() == DeckEditor::ReturnCode::EDITED_DECK) {
            deck->setName(de.getName());
            deck->setFrontTitle(de.getFrontTitle());
            deck->setBackTitle(de.getBackTitle());
            deck->setColorScheme(de.getColorScheme());
        }
        editingDeck = false;
        updateButtonStates();
    }
}

void DeckLister::newDeck() {
    creatingNewDeck = true;
    updateButtonStates();
    DeckEditor de(nullptr);
    de.launch();
    if (de.getReturnCode() == DeckEditor::ReturnCode::EDITED_DECK) {
        Deck* newDeck = new Deck();
        newDeck->setName(de.getName());
        newDeck->setFrontTitle(de.getFrontTitle());
        newDeck->setBackTitle(de.getBackTitle());
        newDeck->setColorScheme(de.getColorScheme());
        deckList->addDeck(newDeck);
        deckIndex = 0;
    }
    creatingNewDeck = false;
    updateButtonStates();
}

void DeckLister::prevDeck() {
    if (deckIndex > 0) {
        deckIndex--;
        ui->setDeckListIndex(deckIndex);
    }
}

void DeckLister::nextDeck() {
    if (deckIndex+1 < deckList->getSize()) {
        deckIndex++;
        ui->setDeckListIndex(deckIndex);
    }
}

void DeckLister::removeDeck() {
    if (deckList->getSize() > 0) {
        deckList->removeDeck(deckIndex);
        int size = deckList->getSize();
        if (size == 0) {
            buttonPointer = 2;
            deckIndex = -1;
        } else if (size == deckIndex)
            deckIndex--;
        ui->setDeckListIndex(deckIndex);
        updateButtonStates();
    }
}

void DeckLister::openDeck() {
    if (deckList->getSize() > 0) {
        Deck* deck = deckList->getDeck(deckIndex);
        assert(deck != nullptr);
        ui->closeDeckLister();
        CardViewer cv(deck);
        cv.launch();
        ui->openDeckLister();
        buttonPointer = 3;
        updateButtonStates();
    }
}

void DeckLister::moveButtonPointer(int direction) {
    buttonPointer = movementMap[buttonPointer][direction];
    while (buttonMap[buttonPointer] == ButtonState::DISABLED) {
        buttonPointer = movementMap[buttonPointer][direction];
    }
    updateButtonStates();
}

void DeckLister::updateButtonStates() {
    for (int i = 0; i < NUM_BUTTONS; i++) buttonMap[i] = ButtonState::DISABLED;
    if (deckList->getSize() > 0) {
        buttonMap[BI_OPEN] = ButtonState::ACTIVE;
        buttonMap[BI_DELETE_DECK] = ButtonState::ACTIVE;
        buttonMap[BI_MODIFY_DECK] = ButtonState::ACTIVE;
        buttonMap[BI_LEFT_ARROW] = ButtonState::ACTIVE;
        buttonMap[BI_RIGHT_ARROW] = ButtonState::ACTIVE;
    }
    buttonMap[BI_NEW_DECK] = ButtonState::ACTIVE;
    buttonMap[BI_EXIT] = ButtonState::ACTIVE;
    if (buttonMap[buttonPointer] == ButtonState::DISABLED) {
        buttonMap[buttonPointer] = ButtonState::DIS_HIGHLIGHT;
    } else {
        buttonMap[buttonPointer] = ButtonState::HIGHTLIGHTED;
    }
    if (editingDeck) {
        buttonMap[BI_MODIFY_DECK] = ButtonState::PRESSED;
    }
    if (creatingNewDeck) {
        buttonMap[BI_NEW_DECK] = ButtonState::PRESSED;
    }
    ui->updateButtonState(ButtonGroups::DECK_LIST, buttonMap);
}


DeckLister::~DeckLister() {
    UI::releaseInstance();
}

CardViewer::CardViewer(Deck* currentDeck) :
    returnCode(ReturnCode::ABORTED), deck(currentDeck), ui(UI::getInstance()), buttonPointer(-1),
    cardIndex(0), isFront(true), running(false), creatingCard(false)
{
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttonMap[i] = ButtonState::DISABLED;
    }
    UI::resolveColorScheme(deck->getColorScheme(), frontColor, backColor);
}

void CardViewer::launch() {
    // Initial Data
    running = true;
    if (deck->getSize() == 0) {
        buttonPointer = 2;
    } else {
        buttonPointer = 4;
        ui->setDeckPosition(cardIndex);
    }

    // Prepare User Interface
    ui->openCardViewer();
    ui->injectCurrentDeck(deck);
    ui->injectCurrentCard(deck->getCard(0), cardIndex);
    ui->setCardSide(true, deck->getFrontTitle(), frontColor);
    updateButtonStates();

    while (running) {
        ui->draw();
        int input = getch();
        handleInput(input);
    }

    ui->closeCardViewer();
}

void CardViewer::handleInput(int input) {
    int movement = UI::getMovement(input);
    if (movement < 0) {
        if (input == ' ' || input == ENTER_KEY) {
            switch(buttonPointer) {
                case BI_CHANGE_DECK:
                    returnCode = ReturnCode::NORMAL;
                    running = false;
                    break;
                case BI_FLIP:
                    flipCard();
                    break;
                case BI_LEFT_ARROW:
                    prevCard();
                    break;
                case BI_RIGHT_ARROW:
                    nextCard();
                    break;
                case BI_MODIFY_DECK:
                    modifyDeck();
                    break;
                case BI_ADD_CARD:
                    newCard();
                    break;
                case BI_QUIZ:
                    quiz();
                    break;
            }
        }
        else {
            switch(input) {
                case 'F':
                case 'f':
                    flipCard();
                    break;
                case '.':
                case '>':
                    nextCard();
                    break;
                case ',':
                case '<':
                    prevCard();
                    break;
                case 'a':
                case 'A':
                    newCard();
                    break;
                case 'c':
                case 'C':
                    returnCode = ReturnCode::NORMAL;
                    running = false;
                    return;
                case 'm':
                case 'M':
                    modifyDeck();
                    break;
                case 'q':
                case 'Q':
                    quiz();
                    break;
                default:
                    break;

            }
        }
    } else {
        moveButtonPointer(movement);
    }
}

void CardViewer::moveButtonPointer(int direction) {
    buttonPointer = movementMap[buttonPointer][direction];
    while (buttonMap[buttonPointer] == ButtonState::DISABLED) {
        buttonPointer = movementMap[buttonPointer][direction];
    }
    updateButtonStates();
}

void CardViewer::nextCard() {
    if (cardIndex < deck->getSize() - 1) {
        cardIndex++;
        ui->setDeckPosition(cardIndex);
        ui->injectCurrentCard(deck->getCard(cardIndex), cardIndex);
    }
}

void CardViewer::prevCard() {
    if (cardIndex > 0) {
        cardIndex--;
        ui->setDeckPosition(cardIndex);
        ui->injectCurrentCard(deck->getCard(cardIndex), cardIndex);
    }
}

void CardViewer::newCard() {
    if (deck->getSize() < MAX_CARDS) {
        creatingCard = true;
        updateButtonStates();
        CardEditor ce(nullptr);
        ce.launch();
        if (ce.getReturnCode() == CardEditor::ReturnCode::EDITED_CARD) {
            Card* newCard = new Card();
            newCard->setFrontTitle(ce.getFrontTitle());
            newCard->setFrontSubtitle(ce.getFrontSubtitle());
            newCard->setBackTitle(ce.getBackTitle());
            newCard->setBackSubtitle(ce.getBackSubtitle());
            deck->addCard(newCard);

            ui->injectCurrentDeck(deck);
            int deckSize = deck->getSize();
            cardIndex = deckSize-1;
            ui->setDeckPosition(cardIndex);
            ui->injectCurrentCard(deck->getCard(cardIndex), cardIndex);
            if (deckSize == MAX_CARDS) buttonPointer = 5;
        }
        creatingCard = false;
        updateButtonStates();
    }
}

void CardViewer::quiz() {
    if (deck->getSize() > 0) {
        ui->closeCardViewer();
        DeckQuiz dq(deck);
        dq.launch();
        ui->openCardViewer();
        ui->injectCurrentCard(deck->getCard(cardIndex), cardIndex);
        ui->setDeckPosition(cardIndex);
        if (isFront) {
            ui->setCardSide(isFront, deck->getFrontTitle(), frontColor);
        } else {
            ui->setCardSide(isFront, deck->getBackTitle(), backColor);
        }
        updateButtonStates();
    }
}

void CardViewer::updateButtonStates() {

    for (int i = 0; i < NUM_BUTTONS; i++) buttonMap[i] = ButtonState::ACTIVE;
    if (deck->getSize() == MAX_CARDS) {
        buttonMap[BI_ADD_CARD] = ButtonState::DISABLED;
    } else if (deck->getSize() == 0) {
        buttonMap[BI_RIGHT_ARROW] = ButtonState::DISABLED;
        buttonMap[BI_LEFT_ARROW] = ButtonState::DISABLED;
        buttonMap[BI_FLIP] = ButtonState::DISABLED;
        buttonMap[BI_QUIZ] = ButtonState::DISABLED;
    }
    if (buttonMap[buttonPointer] == ButtonState::DISABLED) {
        buttonMap[buttonPointer] = ButtonState::DIS_HIGHLIGHT;
    } else {
        buttonMap[buttonPointer] = ButtonState::HIGHTLIGHTED;
    }
    if (creatingCard) {
        buttonMap[BI_ADD_CARD] = ButtonState::PRESSED;
    }
    ui->updateButtonState(ButtonGroups::CARD_VIEW, buttonMap);

}

void CardViewer::flipCard() {
    if (deck->getSize() > 0) {
        isFront = not isFront;
        if (isFront) {
            ui->setCardSide(isFront, deck->getFrontTitle(), frontColor);
        } else {
            ui->setCardSide(isFront, deck->getBackTitle(), backColor);
        }
    }
}

void CardViewer::modifyDeck() {
    ui->closeCardViewer();
    CardLister cl(deck, cardIndex);
    cl.launch();
    ui->openCardViewer();
    if (cl.getReturnCode() == CardLister::ReturnCode::CHANGE_DECK) {
        returnCode = ReturnCode::NORMAL;
        running = false;
        return;
    }
    if (deck->getSize() == 0) {
        ui->injectCurrentCard(nullptr, 0);
    } else {
        cardIndex = cl.getIndex();
        ui->setDeckPosition(cardIndex);
        ui->injectCurrentCard(deck->getCard(cardIndex), cardIndex);
    }
    updateButtonStates();
}

CardViewer::~CardViewer() {
    UI::releaseInstance();
}

CardLister::CardLister(Deck *currentDeck, int startIndex) :
    returnCode(ReturnCode::ABORTED), deck(currentDeck), ui(UI::getInstance()), buttonPointer(6),
    cardIndex(startIndex), running(false), editingCard(false), creatingCard(false)
{
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttonMap[i] = ButtonState::DISABLED;
    }
    ui->setCardListIndex(cardIndex);
}

void CardLister::launch() {
    ui->openCardLister();

    running = true;
    updateButtonStates();

    while (running) {
        ui->draw();
        int input = getch();
        handleInput(input);
    }

    ui->closeCardLister();
}

void CardLister::handleInput(int input) {
    int movement = UI::getMovement(input);
    if (movement < 0) {
        if (input == ' ' || input == ENTER_KEY) {
            switch(buttonPointer) {
                case BI_CARD_VIEW:
                    returnCode = ReturnCode::NORMAL;
                    running = false;
                    break;
                case BI_CHANGE_DECK:
                    returnCode = ReturnCode::CHANGE_DECK;
                    running = false;
                    break;
                case BI_LEFT_ARROW:
                    prevCard();
                    break;
                case BI_RIGHT_ARROW:
                    nextCard();
                    break;
                case BI_DELETE:
                    deleteCard();
                    break;
                case BI_EDIT:
                    editCard();
                    break;
                case BI_ADD_CARD:
                    newCard();
                    break;
            }
        }
        else {
            switch(input) {
                case '.':
                case '>':
                    nextCard();
                    break;
                case ',':
                case '<':
                    prevCard();
                    break;
                case 'a':
                case 'A':
                    newCard();
                    break;
                case 'e':
                case 'E':
                    editCard();
                    break;
                case 'c':
                case 'C':
                    returnCode = ReturnCode::CHANGE_DECK;
                    running = false;
                    return;
                case 'v':
                case 'V':
                    returnCode = ReturnCode::NORMAL;
                    running = false;
                    return;
                case 'd':
                case 'D':
                    deleteCard();
                    break;
                default:
                    break;

            }
        }
    } else {
        moveButtonPointer(movement);
    }
}

void CardLister::newCard() {
    if (deck->getSize() < MAX_CARDS) {
        creatingCard = true;
        updateButtonStates();
        CardEditor ce(nullptr);
        ce.launch();
        if (ce.getReturnCode() == CardEditor::ReturnCode::EDITED_CARD) {
            Card* newCard = new Card();
            newCard->setFrontTitle(ce.getFrontTitle());
            newCard->setBackTitle(ce.getBackTitle());
            newCard->setBackSubtitle(ce.getBackSubtitle());
            newCard->setFrontSubtitle(ce.getFrontSubtitle());
            deck->addCard(newCard);
            ui->injectCurrentDeck(deck);
        }
        creatingCard = false;
        updateButtonStates();
    }
}

void CardLister::editCard() {
    editingCard = true;
    updateButtonStates();
    Card* card = deck->getCard(cardIndex);
    CardEditor ce(card);
    ce.launch();
    if (ce.getReturnCode() == CardEditor::ReturnCode::EDITED_CARD) {
        card->setFrontTitle(ce.getFrontTitle());
        card->setBackTitle(ce.getBackTitle());
        card->setBackSubtitle(ce.getBackSubtitle());
        card->setFrontSubtitle(ce.getFrontSubtitle());
    }
    editingCard = false;
    updateButtonStates();
}

void CardLister::moveButtonPointer(int direction) {
    buttonPointer = movementMap[buttonPointer][direction];
    while (buttonMap[buttonPointer] == ButtonState::DISABLED) {
        buttonPointer = movementMap[buttonPointer][direction];
    }
    updateButtonStates();
}

void CardLister::nextCard() {
    if (cardIndex+1 < deck->getSize()) {
        cardIndex++;
        ui->setCardListIndex(cardIndex);
    }
}

void CardLister::prevCard() {
    if (cardIndex-1 >= 0) {
        cardIndex--;
        ui->setCardListIndex(cardIndex);
    }
}

void CardLister::deleteCard() {
    assert(cardIndex >= 0);
    assert(cardIndex < deck->getSize());
    deck->removeCard(cardIndex);
    int size = deck->getSize();
    if (size == 0) {
        buttonPointer = 2;
        cardIndex = 0;
    } else if (size == cardIndex) {
        cardIndex--;
    }
    ui->injectCurrentDeck(deck);
    ui->setCardListIndex(cardIndex);
    updateButtonStates();
}

void CardLister::updateButtonStates() {

    for (int i = 0; i < NUM_BUTTONS; i++) buttonMap[i] = ButtonState::ACTIVE;
    if (deck->getSize() == 0) {
        buttonMap[BI_LEFT_ARROW] = ButtonState::DISABLED;
        buttonMap[BI_RIGHT_ARROW] = ButtonState::DISABLED;
        buttonMap[BI_EDIT] = ButtonState::DISABLED;
        buttonMap[BI_DELETE] = ButtonState::DISABLED;
    }
    if (buttonMap[buttonPointer] == ButtonState::DISABLED) {
        buttonMap[buttonPointer] = ButtonState::DIS_HIGHLIGHT;
    } else {
        buttonMap[buttonPointer] = ButtonState::HIGHTLIGHTED;
    }
    if (editingCard) {
        buttonMap[BI_EDIT] = ButtonState::PRESSED;
    }
    if (creatingCard) {
        buttonMap[BI_ADD_CARD] = ButtonState::PRESSED;
    }
    ui->updateButtonState(ButtonGroups::CARD_LIST, buttonMap);

}

CardLister::ReturnCode CardLister::getReturnCode() {
    return returnCode;
}

int CardLister::getIndex() {
    return cardIndex;
}

CardLister::~CardLister() {
    UI::releaseInstance();
}

DeckEditor::DeckEditor(Deck *deck) :
    returnCode(ReturnCode::ABORTED), ui(UI::getInstance()), nameSize(0),
    frontSize(0), backSize(0), fieldIndex(0), running(false)
{
    if (deck != nullptr) {
        nameSize = (int)strlen(deck->getName());
        frontSize = (int)strlen(deck->getFrontTitle());
        backSize = (int)strlen(deck->getBackTitle());
        colorScheme = deck->getColorScheme();

        strncpy(name, deck->getName(), (size_t)nameSize);
        strncpy(frontTitle, deck->getFrontTitle(), (size_t)frontSize);
        strncpy(backTitle, deck->getBackTitle(), (size_t)backSize);
        changeSchemeFieldText();
    } else {
        colorScheme = ColorSchemes::BLUE;
        changeSchemeFieldText();
    }
}

void DeckEditor::changeSchemeFieldText() {
    memset(schemeField, '\0', MAX_DECK_INPUT_LEN);
    switch (colorScheme) {
        case ColorSchemes::BLUE:
            strncpy(schemeField, "Blue", strlen("Blue"));
            break;
        case ColorSchemes::RED:
            strncpy(schemeField, "Red", strlen("Red"));
            break;
        case ColorSchemes::WHITE:
            strncpy(schemeField, "White", strlen("White"));
            break;
    }
}

void DeckEditor::changeScheme() {
    switch (colorScheme) {
        case ColorSchemes::BLUE:
            colorScheme = ColorSchemes::RED;
            break;
        case ColorSchemes::RED:
            colorScheme = ColorSchemes::BLUE;
            break;
        case ColorSchemes::WHITE:
            colorScheme = ColorSchemes::BLUE;
    }
    changeSchemeFieldText();
}

DeckEditor::ReturnCode DeckEditor::getReturnCode() {
    return returnCode;
}

void DeckEditor::launch() {
    ui->openDeckEditor();
    ui->initializeDeckEditor();
    ui->injectFields(name, frontTitle, backTitle, schemeField);
    ui->setEditorIndex(fieldIndex);

    ui->activateCursorForEditor();
    setCursor();

    running = true;

    while (running) {
        ui->draw();
        int input = getch();
        handleInput(input);
    }

    ui->deactivateCursor();
    ui->closeDeckEditor();
}

void DeckEditor::setCursor() {
    int y = fieldIndex + 1;
    int x = 15;
    switch (fieldIndex) {
        case 0:
            x += nameSize;
            break;
        case 1:
            x += frontSize;
            break;
        case 2:
            x += backSize;
            break;
        default:
            ui->deactivateCursor();
            return;
    }
    ui->activateCursorForEditor();
    ui->moveCursor(x, y);
}

void DeckEditor::handleInput(int input) {
    switch (input) {
        case TAB_KEY:
        case KEY_DOWN:
            nextField();
            setCursor();
            return;
        case KEY_UP:
            previousField();
            setCursor();
            return;
        default:
            break;
    }
    if (fieldIndex < 3) {
        if (App::isValidInput(input))
            modifyField(input);
    } else if (fieldIndex == 3) {
        if (input == ' ' || input == ENTER_KEY)
            changeScheme();
    } else if (fieldIndex == 4) {
        if (input == KEY_LEFT || input == KEY_RIGHT) {
            fieldIndex = 5;
            ui->setEditorIndex(fieldIndex);
        }
        else if (input == ENTER_KEY || input == ' ') {
            save();
        }
    } else if (fieldIndex == 5) {
        if (input == KEY_LEFT || input == KEY_RIGHT) {
            fieldIndex = 4;
            ui->setEditorIndex(fieldIndex);
        }
        else if (input == ENTER_KEY || input == ' ') {
            running = false;
            returnCode = ReturnCode::NO_CHANGE;
        }
    }
}

void DeckEditor::save() {
    if (checkFields()) {
        running = false;
        returnCode = ReturnCode::EDITED_DECK;
    } else {
        beep();
        ui->error("Cannot Save, Fields Can't Be Empty");
    }
}

bool DeckEditor::checkFields() {
    return nameSize && frontSize && backSize;
}

void DeckEditor::modifyField(int input) {
    char* field;
    int* fieldSize;
    switch (fieldIndex) {
        case 0:
            field = name;
            fieldSize = &nameSize;
            break;
        case 1:
            field = frontTitle;
            fieldSize = &frontSize;
            break;
        case 2:
            field = backTitle;
            fieldSize = &backSize;
            break;
        case 3:
            field = schemeField;
            fieldSize = 0;
            break;
        default:
            return;
    }
    if (input == DELETE_KEY) {
        if (*fieldSize > 0) {
            field[--(*fieldSize)] = '\0';
            ui->moveCursor(15+(*fieldSize), fieldIndex+1);
        }
    }
    else if (*fieldSize < MAX_DECK_INPUT_LEN) {
        field[(*fieldSize)++] = (char)input;
        ui->moveCursor(15+(*fieldSize), fieldIndex+1);
    }
}

void DeckEditor::nextField() {
    if (fieldIndex < 4) {
        fieldIndex++;
        ui->setEditorIndex(fieldIndex);
    } else if (fieldIndex == 4) {
        fieldIndex = 0;
        ui->setEditorIndex(fieldIndex);
    }
}

void DeckEditor::previousField() {
    if (fieldIndex == 5) {
        fieldIndex = 3;
        ui->setEditorIndex(fieldIndex);
    }
    else if (fieldIndex > 0) {
        fieldIndex--;
        ui->setEditorIndex(fieldIndex);
    } else if (fieldIndex == 0) {
        fieldIndex = 4;
        ui->setEditorIndex(fieldIndex);
    }
}

char* DeckEditor::getName() {
    return name;
}

char* DeckEditor::getFrontTitle() {
    return frontTitle;
}

char* DeckEditor::getBackTitle() {
    return backTitle;
}

ColorSchemes DeckEditor::getColorScheme() {
    return colorScheme;
}

DeckEditor::~DeckEditor() {
    UI::releaseInstance();
}

CardEditor::CardEditor(Card *card) :
        returnCode(ReturnCode::ABORTED), ui(UI::getInstance()), frontTitleSize(0),
        frontSubtitleSize(0), backTitleSize(0), backSubtitleSize(0), fieldIndex(0), running(false)
{
    if (card != nullptr) {
        frontTitleSize = (int)strlen(card->getFrontTitle());
        frontSubtitleSize = (int)strlen(card->getFrontSubtitle());
        backTitleSize = (int)strlen(card->getBackTitle());
        backSubtitleSize = (int)strlen(card->getBackSubtitle());

        strncpy(frontTitle, card->getFrontTitle(), (size_t)frontTitleSize);
        strncpy(frontSubtitle, card->getFrontSubtitle(), (size_t)frontSubtitleSize);
        strncpy(backTitle, card->getBackTitle(), (size_t)backTitleSize);
        strncpy(backSubtitle, card->getBackSubtitle(), (size_t)backSubtitleSize);
    }
}

CardEditor::ReturnCode CardEditor::getReturnCode() {
    return returnCode;
}

void CardEditor::launch() {
    ui->openCardEditorFromList();
    ui->initializeCardEditor();
    ui->injectFields(frontTitle, frontSubtitle, backTitle, backSubtitle);
    ui->setEditorIndex(fieldIndex);

    ui->activateCursorForEditor();
    setCursor();

    running = true;

    while (running) {
        ui->draw();
        int input = getch();
        handleInput(input);
    }

    ui->deactivateCursor();
    ui->closeCardEditorFromList();
}

void CardEditor::setCursor() {
    int y = fieldIndex + 1;
    int x = 10;
    switch (fieldIndex) {
        case 0:
            x += frontTitleSize;
            break;
        case 1:
            x += frontSubtitleSize;
            break;
        case 2:
            x += backTitleSize;
            break;
        case 3:
            x += backSubtitleSize;
            break;
        default:
            ui->deactivateCursor();
            return;
    }
    ui->activateCursorForEditor();
    ui->moveCursor(x, y);
}

void CardEditor::handleInput(int input) {
    switch (input) {
        case TAB_KEY:
        case KEY_DOWN:
            nextField();
            setCursor();
            return;
        case KEY_UP:
            previousField();
            setCursor();
            return;
        default:
            break;
    }
    if (fieldIndex < 4) {
        if (App::isValidInput(input))
            modifyField(input);
    } else if (fieldIndex == 4) {
        if (input == KEY_LEFT || input == KEY_RIGHT) {
            fieldIndex = 5;
            ui->setEditorIndex(fieldIndex);
        }
        else if (input == ENTER_KEY || input == ' ') {
            save();
        }
    } else if (fieldIndex == 5) {
        if (input == KEY_LEFT || input == KEY_RIGHT) {
            fieldIndex = 4;
            ui->setEditorIndex(fieldIndex);
        }
        else if (input == ENTER_KEY || input == ' ') {
            running = false;
            returnCode = ReturnCode::NO_CHANGE;
        }
    }
}

void CardEditor::save() {
    if (checkFields()) {
        running = false;
        returnCode = ReturnCode::EDITED_CARD;
    } else {
        beep();
        ui->error("Cannot Save, Title Fields Can't Be Empty");
    }
}

bool CardEditor::checkFields() {
    return frontTitleSize && backTitleSize;
}

void CardEditor::modifyField(int input) {
    char* field;
    int* fieldSize;
    switch (fieldIndex) {
        case 0:
            field = frontTitle;
            fieldSize = &frontTitleSize;
            break;
        case 1:
            field = frontSubtitle;
            fieldSize = &frontSubtitleSize;
            break;
        case 2:
            field = backTitle;
            fieldSize = &backTitleSize;
            break;
        case 3:
            field = backSubtitle;
            fieldSize = &backSubtitleSize;
            break;
        default:
            return;
    }
    if (input == DELETE_KEY) {
        if (*fieldSize > 0) {
            field[--(*fieldSize)] = '\0';
            ui->moveCursor(10+(*fieldSize), fieldIndex+1);
        }
    }
    else if (*fieldSize < MAX_CARD_INPUT_LEN) {
        field[(*fieldSize)++] = (char)input;
        ui->moveCursor(10+(*fieldSize), fieldIndex+1);
    }
}

void CardEditor::nextField() {
    if (fieldIndex < 4) {
        fieldIndex++;
        ui->setEditorIndex(fieldIndex);
    } else if (fieldIndex == 4) {
        fieldIndex = 0;
        ui->setEditorIndex(fieldIndex);
    }
}

void CardEditor::previousField() {
    if (fieldIndex == 5) {
        fieldIndex = 3;
        ui->setEditorIndex(fieldIndex);
    }
    else if (fieldIndex > 0) {
        fieldIndex--;
        ui->setEditorIndex(fieldIndex);
    } else if (fieldIndex == 0) {
        fieldIndex = 4;
        ui->setEditorIndex(fieldIndex);
    }
}

char* CardEditor::getFrontTitle() {
    return frontTitle;
}

char* CardEditor::getBackTitle() {
    return backTitle;
}

char* CardEditor::getFrontSubtitle() {
    return frontSubtitle;
}

char* CardEditor::getBackSubtitle() {
    return backSubtitle;
}

CardEditor::~CardEditor() {
    UI::releaseInstance();
}

DeckQuiz::DeckQuiz(Deck *deck) :
    returnCode(ReturnCode::ABORTED), ui(UI::getInstance()), deckRef(deck), buttonPointer(4),
    cardIndex(0), nextCardToAnswer(0), deckSize(deck->getSize()), inputLength(0), givingInput(true),
    isFront(true), running(false)
{
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttonMap[i] = ButtonState::DISABLED;
    }
    UI::resolveColorScheme(deck->getColorScheme(), frontColor, backColor);

    quizDeck = new Card*[deckSize];
    for (int i = 0; i < deckSize; i++) {
        quizDeck[i] = deck->getCard(i);
    }
    quizResults = new Results[deckSize];
    for (int i = 0; i < deckSize; i++) {
        quizResults[i] = Results::PENDING;
    }

    shuffleDeck();
}

void DeckQuiz::updateDeckMeter() {
    ui->setDeckPosition(cardIndex);
    ui->resetDeckMeterColors();
    for (int i  = 0; i < deckSize; i++) {
        switch (quizResults[i]) {
            case Results::RIGHT:
                ui->setDeckMeterTickColor(i+1, Colors::GREEN);
                break;
            case Results::WRONG:
                ui->setDeckMeterTickColor(i+1, Colors::RED);
                break;
            case Results::PENDING:
                if (i == cardIndex)
                    ui->setDeckMeterTickColor(i+1, Colors::YELLOW);
                break;
        }
    }
}

void DeckQuiz::launch() {
    running = true;

    // Prepare User Interface
    ui->openQuiz();
    ui->setCardSide(true, deckRef->getFrontTitle(), frontColor);
    ui->injectCurrentCard(quizDeck[0], 0);
    ui->setDeckPosition(0);
    ui->setInputFieldText(inputField);
    updateButtonStates();
    updateDeckMeter();
    startGivingInput();

    while (running) {
        ui->draw();
        int input = getch();
        handleInput(input);
    }

    ui->resetInputField();
    ui->closeQuiz();
}

void DeckQuiz::handleInput(int input) {
    if (givingInput) {
        switch(input) {
            case ESCAPE_KEY:
                stopGivingInput();
                return;
            case ENTER_KEY:
                answer();
                return;
            default:
                break;
        }
        if (App::isValidInput(input))
            modifyField(input);
    }
    else {
        int movement = UI::getMovement(input);
        if (movement < 0) {
            if (input == ' ' || input == ENTER_KEY) {
                switch (buttonPointer) {
                    case BI_ANSWER:
                        startGivingInput();
                        break;
                    case BI_RIGHT_ARROW:
                        nextCard();
                        break;
                    case BI_LEFT_ARROW:
                        prevCard();
                        break;
                    default:
                        running = false;
                        break;
                }
            }
        }
        else {
            moveButtonPointer(movement);
        }
    }
}

void DeckQuiz::modifyField(int input) {
    if (input == DELETE_KEY) {
        if (inputLength > 0) {
            inputField[--inputLength] = '\0';
            ui->moveCursor(1+inputLength, INPUT_Y_AXIS);
        }
    }
    else if (inputLength < INPUT_FIELD_LENGTH) {
        inputField[inputLength++] = (char)input;
        ui->moveCursor(1+inputLength, INPUT_Y_AXIS);
    }
    ui->setInputFieldText(inputField);
}

void DeckQuiz::startGivingInput() {
    givingInput = true;
    ui->activateCursorForQuiz();
    ui->moveCursor(1+inputLength, INPUT_Y_AXIS);
    ui->console("Type Your Answer and Press Enter. Exit with ESC");
    updateButtonStates();
}

void DeckQuiz::stopGivingInput() {
    givingInput = false;
    ui->deactivateCursor();
    ui->console("Press Answer to Give Answer for Other Side");
    updateButtonStates();
}

void DeckQuiz::updateButtonStates() {
    for (int i = 0; i < NUM_BUTTONS; i++) buttonMap[i] = ButtonState::DISABLED;

    if (cardIndex == nextCardToAnswer) buttonMap[BI_ANSWER] = ButtonState::ACTIVE;
    if (cardIndex < deckSize-1 && cardIndex < nextCardToAnswer) buttonMap[BI_RIGHT_ARROW] = ButtonState::ACTIVE;
    if (cardIndex > 0) buttonMap[BI_LEFT_ARROW] = ButtonState::ACTIVE;
    buttonMap[BI_END_QUIZ] = ButtonState::ACTIVE;
    if (!givingInput) {
        if (buttonMap[buttonPointer] == ButtonState::DISABLED) {
            buttonMap[buttonPointer] = ButtonState::DIS_HIGHLIGHT;
        } else {
            buttonMap[buttonPointer] = ButtonState::HIGHTLIGHTED;
        }
    } else {
        buttonMap[BI_ANSWER] = ButtonState::PRESSED;
    }
    ui->updateButtonState(ButtonGroups::QUIZ, buttonMap);
}

void DeckQuiz::moveButtonPointer(int direction) {
    if (buttonMap[BI_ANSWER] == ButtonState::DISABLED && (direction == DIRECTION_DOWN || direction == DIRECTION_UP)) {
        if ((buttonPointer == BI_LEFT_ARROW || buttonPointer == BI_RIGHT_ARROW)) {
            buttonPointer = BI_END_QUIZ;
        } else if (buttonPointer == BI_END_QUIZ) {
            buttonPointer = BI_RIGHT_ARROW;
        }
    } else {
        buttonPointer = movementMap[buttonPointer][direction];
        while (buttonMap[buttonPointer] == ButtonState::DISABLED) {
            buttonPointer = movementMap[buttonPointer][direction];
        }
    }
    updateButtonStates();
}

void DeckQuiz::shuffleDeck() {
    srand(time(NULL));
    for (int i = deckSize-1; i > 0; i--) {
        int j = rand() % (i+1);
        swap(&quizDeck[i], &quizDeck[j]);
    }
}

void DeckQuiz::swap(Card** a, Card** b) {
    Card* temp = *a;
    *a = *b;
    *b = temp;
}

void DeckQuiz::nextCard() {
    if (cardIndex < deckSize - 1 && cardIndex < nextCardToAnswer) {
        cardIndex++;
        ui->setDeckPosition(cardIndex);
        ui->injectCurrentCard(quizDeck[cardIndex], cardIndex);

        if (cardIndex == nextCardToAnswer) {
            if (buttonPointer == BI_RIGHT_ARROW) {
                buttonPointer = BI_LEFT_ARROW;
            }
            ui->resetInputField();
        } else {
            if (quizResults[cardIndex] == Results::WRONG) {
                ui->setInputFieldIncorrect();
            }
            else if (quizResults[cardIndex] == Results::RIGHT) {
                ui->setInputFieldCorrect();
            }
            ui->setInputFieldText(quizDeck[cardIndex]->getBackTitle());
        }

        updateButtonStates();
    }
}

void DeckQuiz::prevCard() {
    if (cardIndex > 0) {
        cardIndex--;
        ui->setDeckPosition(cardIndex);
        ui->injectCurrentCard(quizDeck[cardIndex], cardIndex);

        if (cardIndex == 0 && buttonPointer == BI_LEFT_ARROW) buttonPointer = BI_RIGHT_ARROW;

        if (quizResults[cardIndex] == Results::WRONG) ui->setInputFieldIncorrect();
        else if (quizResults[cardIndex] == Results::RIGHT) ui->setInputFieldCorrect();
        ui->setInputFieldText(quizDeck[cardIndex]->getBackTitle());

        updateButtonStates();
    }
}

void DeckQuiz::answer() {
    if (cardIndex == nextCardToAnswer) {
        char myAnswer[INPUT_FIELD_LENGTH] = {0};
        char correctAnswer[INPUT_FIELD_LENGTH] = {0};
        strncpy(myAnswer, inputField, (size_t)inputLength);
        strncpy(correctAnswer, quizDeck[nextCardToAnswer]->getBackTitle(), (size_t)quizDeck[nextCardToAnswer]->getBackTitleLength());
        for (int i = 0; i < inputLength; i++) myAnswer[i] = (char)tolower(myAnswer[i]);
        for (int i = 0; i < (size_t)quizDeck[nextCardToAnswer]->getBackTitleLength(); i++) correctAnswer[i] = (char)tolower(correctAnswer[i]);

        if (strcmp(myAnswer, correctAnswer) == 0) {
            ui->setInputFieldCorrect();
            quizResults[nextCardToAnswer++] = Results::RIGHT;
        } else {
            beep();
            ui->setInputFieldIncorrect();
            quizResults[nextCardToAnswer++] = Results::WRONG;
        }
        ui->deactivateCursor();
        ui->setCardSide(false, deckRef->getBackTitle(), backColor);
        updateDeckMeter();
        ui->draw();
        getch();
        cardIndex++;
        inputLength = 0;
        memset(inputField, '\0', INPUT_FIELD_LENGTH);
        if (cardIndex >= deckSize) {
            running = false;
        } else {
            ui->activateCursorForQuiz();
            ui->moveCursor(1+inputLength, INPUT_Y_AXIS);
            ui->resetInputField();
            ui->setCardSide(true, deckRef->getFrontTitle(), frontColor);
            ui->setDeckPosition(cardIndex);
            ui->injectCurrentCard(quizDeck[cardIndex], cardIndex);
            updateDeckMeter();
            updateButtonStates();
        }
    }
}

DeckQuiz::~DeckQuiz() {
    UI::releaseInstance();
    delete[] quizDeck;
    delete[] quizResults;
}