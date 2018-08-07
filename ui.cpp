//
// Created by Brandon Martin on 7/14/18.
//

#include "ui.h"

void UI::activateCursor(Canvas *hostCanvas) {
    curs_set(1);
    cursorActive = true;
    cursorCanvas = hostCanvas;
}

void UI::moveCursor(int x, int y) {
    cursorX = x;
    cursorY = y;
    wrefresh(cursorCanvas->window);
}

void UI::deactivateCursor() {
    curs_set(0);
    cursorActive = false;
    cursorCanvas = nullptr;
    cursorX = 0;
    cursorY = 0;
}

void UI::activateCursorForEditor() {
    activateCursor(C_EDIT_VIEW);
}

void UI::assignDeckList(DeckLinkedList *deckList) {
    E_DECK_TABLE->setDeckList(deckList);
}

void UI::openDeckLister() {
    switchButtonGroup(ButtonGroups::DECK_LIST);
    C_LIST_VIEW->show();
    E_DECK_NAME->setText("No Deck Loaded", Colors::GRAY, Attributes::BLINK);
    console("Welcome!");
}

void UI::closeDeckLister() {
    C_LIST_VIEW->hide();
}

void UI::openCardViewer() {
    switchButtonGroup(ButtonGroups::CARD_VIEW);
    C_CARD_VIEW->show();
    C_CONSOLE->showElement(CI_DECK_SIZE);
    console("CardViewer");
}

void UI::closeCardViewer() {
    C_CARD_VIEW->hide();
    C_CONSOLE->hideElement(CI_DECK_SIZE);
}

void UI::openQuiz() {
    switchButtonGroup(ButtonGroups::QUIZ);
    C_CARD_VIEW->show();
    C_CONSOLE->showElement(CI_DECK_SIZE);
    console("CardViewer");
}

void UI::closeQuiz() {
    C_CONSOLE->hideElement(CI_DECK_SIZE);
}

void UI::openCardLister() {
    switchButtonGroup(ButtonGroups::CARD_LIST);
    C_LIST_VIEW->show();
    C_LIST_VIEW->hideElement(CI_DECK_TABLE);
    C_LIST_VIEW->showElement(CI_CARD_TABLE);
    C_CONSOLE->showElement(CI_DECK_SIZE);
}

void UI::closeCardLister() {
    C_LIST_VIEW->hide();
    C_CONSOLE->hideElement(CI_DECK_SIZE);
    C_LIST_VIEW->hideElement(CI_CARD_TABLE);
    C_LIST_VIEW->showElement(CI_DECK_TABLE);
}

void UI::openDeckEditor() {
    C_LIST_VIEW->freezeElement(CI_DECK_TABLE);
    C_EDIT_VIEW->show();
    C_EDIT_VIEW->burnText(4, 0, "Deck");
}

void UI::closeDeckEditor() {
    C_LIST_VIEW->thawElement(CI_DECK_TABLE);
    C_EDIT_VIEW->hide();
}

void UI::openCardEditorFromList() {
    C_LIST_VIEW->freezeElement(CI_CARD_TABLE);
    C_CARD_VIEW->freezeElement(CI_CARD_BOX);
    C_EDIT_VIEW->show();
    C_EDIT_VIEW->burnText(4, 0, "Card");
}

void UI::closeCardEditorFromList() {
    C_LIST_VIEW->thawElement(CI_CARD_TABLE);
    C_CARD_VIEW->thawElement(CI_CARD_BOX);
    C_EDIT_VIEW->hide();
}

void UI::initializeCardEditor() {
    E_EDIT_FORM->initializeForCard();
}

void UI::console(const char *message) {
    E_CONSOLE->setText(message, Colors::YELLOW, Attributes::NORMAL);
}

void UI::error(const char *message) {
    E_CONSOLE->setText(message, Colors::RED, Attributes::NORMAL);
}

void UI::injectCurrentDeck(Deck *deck) {
    char str[3];
    sprintf(str, "%d", deck->getSize());
    E_DECK_SIZE->setText(str);
    E_DECK_NAME->setText(deck->getName());
    E_DECK_NAME->setAttribute(Attributes::NORMAL);
    E_DECK_METER->setDeck(deck);
    E_CARD_TABLE->setDeck(deck);
    E_DECK_METER->resetColors();
}

void UI::initializeDeckEditor() {
    E_EDIT_FORM->initializeForDeck();
}

void UI::injectCurrentCard(Card* card, int index) {
    if (card == nullptr) {
        E_CARD_NUM->setText("  ");
    } else {
        char str[3];
        sprintf(str, "%d", index);
        E_CARD_NUM->setText(str);
    }
    E_CARD_BOX->setCard(card);
}

void UI::injectFields(char *field1, char *field2, char *field3, char *field4) {
    E_EDIT_FORM->setFormFields(field1, field2, field3, field4);
}

void UI::setCardSide(bool isFront, const char* sideName, Colors color) {
    E_SIDE_NAME->setText(sideName);
    E_CARD_BOX->setSide(isFront);
    E_CARD_BOX->setColor(color);

    E_DECK_NAME->setColor(color);
    E_DECK_SIZE->setColor(color);
    E_DECK_METER->setColor(color);
    E_DECK_METER->resetColors();
    E_SIDE_NAME->setColor(color);
    E_CARD_NUM->setColor(color);
}

void UI::setDeckListIndex(int index) {
    E_DECK_TABLE->setIndex(index);
}

void UI::setCardListIndex(int index) {
    E_CARD_TABLE->setIndex(index);
}

void UI::setDeckPosition(int index) {
    E_DECK_METER->setPosition(index);
}

void UI::setEditorIndex(int index) {
    E_EDIT_FORM->setIndex(index);
}

void UI::startupInitialization() {
    // Assuming No Deck Is Loaded Automatically
    C_CONSOLE->show();
    C_BUTTONS->show();
    C_CONSOLE->hideElement(CI_DECK_SIZE);
    C_LIST_VIEW->hideElement(CI_CARD_TABLE);
    E_RIGHT_ARROW->setState(ButtonState::ACTIVE);
    E_LEFT_ARROW->setState(ButtonState::ACTIVE);
    E_EXIT_BUTTON->setState(ButtonState::ACTIVE);
}

void UI::updateButtonState(ButtonGroups group, ButtonState states[NUM_BUTTONS]) {
    switch(group) {
        case ButtonGroups::NONE:
            return;
        case ButtonGroups::CARD_VIEW:
            for (int i = 0; i < NUM_BUTTONS; i++) ((Button*)C_BUTTONS->getElement(cardViewButtons[i]))->setState(states[i]);
            break;
        case ButtonGroups::DECK_LIST:
            for (int i = 0; i < NUM_BUTTONS; i++) ((Button*)C_BUTTONS->getElement(deckListButtons[i]))->setState(states[i]);
            break;
        case ButtonGroups::CARD_LIST:
            for (int i = 0; i < NUM_BUTTONS; i++) ((Button*)C_BUTTONS->getElement(cardListButtons[i]))->setState(states[i]);
            break;
        case ButtonGroups::QUIZ:
            for (int i = 0; i < NUM_BUTTONS; i++) ((Button*)C_BUTTONS->getElement(quizButtons[i]))->setState(states[i]);
            break;
    }
}