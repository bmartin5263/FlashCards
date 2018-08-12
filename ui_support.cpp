//
// Created by Brandon Martin on 7/14/18.
//

#include <cstring>
#include <assert.h>
#include <cmath>
#include "ui_support.h"
#include "ui.h"

UI* UI::INSTANCE = nullptr;
int UI::REF_COUNT = 0;

//void UI::updatePanels() {
//    update_panels();
//    doupdate();
//}

void UI::resolveColorScheme(ColorSchemes scheme, Colors &front, Colors &back) {
    switch (scheme) {
        case ColorSchemes::BLUE:
            front = Colors::C_BLUE_FRONT;
            back = Colors::C_BLUE_BACK;
            break;
        case ColorSchemes::RED:
            front = Colors::C_RED_FRONT;
            back = Colors::C_RED_BACK;
            break;
        case ColorSchemes::WHITE:
            front = Colors::WHITE;
            back = Colors::WHITE;
            break;
    }
}

unsigned int UI::getColorPair(Colors color) {
    switch (color) {
        case Colors::DEFAULT:
        case Colors::WHITE:
            return COLOR_PAIR(1);
        case Colors::BLUE:
            return COLOR_PAIR(2);
        case Colors::RED:
            return COLOR_PAIR(3);
        case Colors::GREEN:
            return COLOR_PAIR(4);
        case Colors::YELLOW:
            return COLOR_PAIR(5);
        case Colors::INVERT:
            return COLOR_PAIR(6);
        case Colors::GRAY:
            return COLOR_PAIR(7);
        case Colors::WHITE_HL:
            return COLOR_PAIR(8);
        case Colors::C_BLUE_FRONT:
            return COLOR_PAIR(9);
        case Colors::C_BLUE_BACK:
            return COLOR_PAIR(10);
        case Colors::C_RED_FRONT:
            return COLOR_PAIR(11);
        case Colors::C_RED_BACK:
            return COLOR_PAIR(12);
        case Colors::YELLOW_HL:
            return COLOR_PAIR(13);
    }
}

unsigned int UI::getPairNumber(Colors color) {
    switch (color) {
        case Colors::DEFAULT:
        case Colors::WHITE:
            return 1;
        case Colors::BLUE:
            return 2;
        case Colors::RED:
            return 3;
        case Colors::GREEN:
            return 4;
        case Colors::YELLOW:
            return 5;
        case Colors::INVERT:
            return 6;
        case Colors::GRAY:
            return 7;
        case Colors::WHITE_HL:
            return 8;
        case Colors::C_BLUE_FRONT:
            return 9;
        case Colors::C_BLUE_BACK:
            return 10;
        case Colors::C_RED_FRONT:
            return 11;
        case Colors::C_RED_BACK:
            return 12;
        case Colors::YELLOW_HL:
            return 13;
    }
}

unsigned int UI::getAttribute(Attributes attr) {
    switch (attr) {
        case Attributes::NORMAL:
            return 0;
        case Attributes::BLINK:
            return A_BLINK;
    }
}

UI* UI::getInstance() {
    if (INSTANCE == nullptr) INSTANCE = new UI();
    REF_COUNT++;
    return INSTANCE;
}

void UI::releaseInstance() {
    REF_COUNT--;
    if (REF_COUNT == 0) delete INSTANCE;
}

bool UI::referenceCountIsZero() {
    return REF_COUNT == 0;
}

int UI::getMovement(int input) {
    switch (input) {
        case KEY_DOWN:
            return DIRECTION_DOWN;
        case KEY_UP:
            return DIRECTION_UP;
        case KEY_LEFT:
            return DIRECTION_LEFT;
        case KEY_RIGHT:
            return DIRECTION_RIGHT;
        default:
            return -1;
    }
}

void UI::closeButtonGroup(ButtonGroups group) {
    switch(group) {
        case ButtonGroups::NONE:
            return;
        case ButtonGroups::CARD_VIEW:
            for (int i = 0; i < NUM_BUTTONS; i++) C_BUTTONS->hideElement(cardViewButtons[i]);
            break;
        case ButtonGroups::DECK_LIST:
            for (int i = 0; i < NUM_BUTTONS; i++) C_BUTTONS->hideElement(deckListButtons[i]);
            break;
        case ButtonGroups::CARD_LIST:
            for (int i = 0; i < NUM_BUTTONS; i++) C_BUTTONS->hideElement(cardListButtons[i]);
            break;
        case ButtonGroups::QUIZ:
            for (int i = 0; i < NUM_BUTTONS; i++) C_BUTTONS->hideElement(quizButtons[i]);
            break;
    }
    this->currentButtonGroup = ButtonGroups::NONE;
}

void UI::openButtonGroup(ButtonGroups group) {
    switch(group) {
        case ButtonGroups::NONE:
            return;
        case ButtonGroups::CARD_VIEW:
            for (int i = 0; i < NUM_BUTTONS; i++) C_BUTTONS->showElement(cardViewButtons[i]);
            break;
        case ButtonGroups::DECK_LIST:
            for (int i = 0; i < NUM_BUTTONS; i++) C_BUTTONS->showElement(deckListButtons[i]);
            break;
        case ButtonGroups::CARD_LIST:
            for (int i = 0; i < NUM_BUTTONS; i++) C_BUTTONS->showElement(cardListButtons[i]);
            break;
        case ButtonGroups::QUIZ:
            for (int i = 0; i < NUM_BUTTONS; i++) C_BUTTONS->showElement(quizButtons[i]);
            break;
    }
    this->currentButtonGroup = group;
}

void UI::switchButtonGroup(ButtonGroups group) {
    if (currentButtonGroup != ButtonGroups::NONE)
        closeButtonGroup(currentButtonGroup);
    if (group != ButtonGroups::NONE)
        openButtonGroup(group);
}

Canvas::Canvas(int x, int y, int cols, int rows, int maxElements, Colors colorPair, Borders border) :
        x(x), y(y), cols(cols), rows(rows), numElements(0), maxElements(maxElements), defaultColor(colorPair)
{
    // The literal canvas
    window = newwin(rows, cols, y, x);
    panel = new_panel(window);

    // Give it a basic border, if needed
    if (border != Borders::NONE)
        box(window, 0, 0);

    // Adjust the border based on position
    switch (border) {
        case Borders::TOP:
            mvwaddch(window, rows-1, 0, ACS_LTEE);
            mvwaddch(window, rows-1, cols-1, ACS_RTEE);
            break;
        case Borders::BOTTOM:
            mvwaddch(window, 0, 0, ACS_LTEE);
            mvwaddch(window, 0, cols-1, ACS_RTEE);
            break;
        case Borders::MIDDLE:
            mvwaddch(window, 0, 0, ACS_LTEE);
            mvwaddch(window, 0, cols-1, ACS_RTEE);
            mvwaddch(window, rows-1, 0, ACS_LTEE);
            mvwaddch(window, rows-1, cols-1, ACS_RTEE);
            break;
        case Borders::NONE:
        case Borders::FLOAT:        // Just a box, does not connect to another
            break;
    }

    wbkgd(window, UI::getColorPair(defaultColor));

    this->elements = new Element*[maxElements];

    wnoutrefresh(window);
    hide();
}

// Bake a row onto the window. Cannot change.
void Canvas::burnRow(int y) {
    mvwaddch(window, y, 0, ACS_LTEE);
    mvwaddch(window, y, cols-1, ACS_RTEE);
    for (int i = 1 ; i < cols-1 ; i++) {
        mvwaddch(window, y, i, ACS_HLINE);
    }
    wnoutrefresh(window);
}

// Bake text onto the window. Cannot change.
void Canvas::burnText(int x, int y, const char *text) {
    mvwaddstr(window, y, x, text);
    wnoutrefresh(window);
}

void Canvas::burnText(int x, int y, const char *text, Colors color) {
    mvwaddstr(window, y, x, text);
    mvwchgat(window, y, x, (int)strlen(text), UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(color), nullptr);
    wnoutrefresh(window);
}

void Canvas::addElement(Element *e) {
    assert(numElements < maxElements);
    elements[numElements++] = e;
}

void Canvas::render() {
    if (!isHidden()) {
        for (int i = 0; i < numElements; i++) {
            elements[i]->render(window);
        }
    }
}

void Canvas::hide() {
    hide_panel(panel);
    hidden = 1;
}

void Canvas::show() {
    show_panel(panel);
    hidden = 0;
}

int Canvas::isHidden() const {
    return hidden;
}

Element* Canvas::getElement(int index) {
    return elements[index];
}

void Canvas::hideElement(int elementIndex) {
    elements[elementIndex]->hide(window);
}

void Canvas::showElement(int elementIndex) {
    elements[elementIndex]->show();
}

void Canvas::thawElement(int elementIndex) {
    elements[elementIndex]->thaw();
}

void Canvas::freezeElement(int elementIndex) {
    elements[elementIndex]->freeze();
}

Canvas::~Canvas() {
    // Delete the Window and Panel
    del_panel(panel);
    delwin(window);
    delete[] elements;
}

//////////////////////////////////////////////////////////////////////////////

Element::Element(int x, int y, int length, int height) :
    x(x), y(y), length(length), height(height), hidden(false)
{
}

void Element::getDimensions(int &length, int &height) {
    length = this->length;
    height = this->height;
}

int Element::getLength() {
    return length;
}

void Element::getXY(int &x, int &y) {
    x = this->x;
    y = this->y;
}

//void Element::move(int x, int y) {
//    this->x = x;
//    this->y = y;
//}

void Element::freeze() {
    setHidden(true);
}

void Element::thaw() {
    setHidden(false);
}

void Element::setHidden(bool hidden) {
    //assert(this->hidden != hidden);
    this->hidden = hidden;
}

bool Element::isHidden() {
    return hidden;
}

//////////////////////////////////////////////////////////////////////////////

Label::Label(int x, int y, int length, Colors color, Attributes attribute) :
    Element(x, y, length, 1), color(color), attribute(attribute)
{
    body = new char[length+1];
    memset(body, '*', (size_t) length);
    body[length] = '\0';
}

void Label::setText(const char* text) {
    if (text != nullptr) {
        strncpy(body, text, (size_t) getLength());
    }
}

void Label::setText(const char *text, Colors color, Attributes attribute) {
    setColor(color);
    setAttribute(attribute);
    this->setText(text);
}

void Label::render(WINDOW* window) {
    if (!isHidden()) {
        clear(window);
        int x;
        int y;
        int length = getLength();
        getXY(x, y);
        assert(body != nullptr);
        mvwaddstr(window, y, x, body);
        mvwchgat(window, y, x, length, UI::getAttribute(attribute), (short)UI::getPairNumber(color), nullptr);
        wnoutrefresh(window);
    }
}

void Label::hide(WINDOW * window) {
    setHidden(true);
    clear(window);
    wnoutrefresh(window);
}

void Label::show() {
    setHidden(false);
}

void Label::clear(WINDOW* window) {
    int x;
    int y;
    int length = getLength();
    getXY(x, y);
    mvwprintw(window, y, x, "%*s", length, "");
}

void Label::setColor(Colors color) {
    this->color = color;
}

void Label::setAttribute(Attributes attr) {
    this->attribute = attr;
}

Label::~Label() {
    delete[] body;
}

//////////////////////////////////////////////////////////////////////////////

DeckMeter::DeckMeter(int x, int y, int length, Colors color) :
    Element(x, y, length, 1), color(color), size(0), position(0)
{
    colorMap = new Colors[length];
    resetColors();
}

void DeckMeter::setPosition(const int pos) {
    this->position = pos;
}

void DeckMeter::render(WINDOW* window) {
    if (!isHidden()) {
        int x;
        int y;
        getXY(x, y);
        clear(window);
        mvwaddch(window, y, x, '[');
        mvwchgat(window, y, x, 1, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(colorMap[0]), nullptr);
        int i = 1;
        if (deck != nullptr) {
            int pos = position + 1;
            int size = deck->getSize();
            while (i <= size) {
                if (i == pos) mvwaddch(window, y, x+i, '|');
                else mvwaddch(window, y, x+i, '-');
                mvwchgat(window, y, x+i, 1, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(colorMap[i]), nullptr);
                i++;
            }
        }
        mvwaddch(window, y, x+i, ']');
        mvwchgat(window, y, x+i, 1, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(colorMap[i]), nullptr);
        wnoutrefresh(window);
    }
}

void DeckMeter::hide(WINDOW * window) {
    setHidden(true);
    clear(window);
    wnoutrefresh(window);
}

void DeckMeter::show() {
    setHidden(false);
}

void DeckMeter::clear(WINDOW *window) {
    int x;
    int y;
    int length = getLength();
    getXY(x, y);
    mvwprintw(window, y, x, "%*s", length, "");
}

void DeckMeter::resetColors() {
    for (int i = 0; i < getLength(); i++) colorMap[i] = color;
}

void DeckMeter::setColor(Colors color) {
    this->color = color;
}

void DeckMeter::setColorForTick(const int index, Colors color) {
    colorMap[index] = color;
}

void DeckMeter::setDeck(Deck *deck) {
    this->deck = deck;
}

DeckMeter::~DeckMeter() {
    delete[] colorMap;
}

//////////////////////////////////////////////////////////////////////////////

Button::Button(int x, int y, int length, const char *text, int textLen, Colors color, int textHighlight) :
    Element(x, y, length, 3), labelLen(textLen), color(color), textHighlightOffset(textHighlight)
{
    label = new char[textLen+1];
    strncpy(label, text, (size_t)textLen);
    label[textLen] = '\0';
}

void Button::render(WINDOW * window) {
    if (!isHidden()) {

        clear(window);

        int length;
        int height;
        int x;
        int y;
        getDimensions(length, height);
        getXY(x, y);

        Colors renderColor;

        switch (state) {
            case ButtonState::ACTIVE:
                renderColor = color;
                break;
            case ButtonState::DIS_HIGHLIGHT:
            case ButtonState::DISABLED:
                renderColor = Colors::GRAY;
                break;
            case ButtonState::HIGHTLIGHTED:
                renderColor = Colors::YELLOW;
                break;
            case ButtonState::PRESSED:
                renderColor = Colors::GREEN;
                break;
        }

        wattron(window, UI::getColorPair(renderColor));
        for (int i = 1; i < length-1; i++) {
            mvwaddch(window, y, x+i, ACS_HLINE);
            mvwaddch(window, y+2, x+i, ACS_HLINE);
        }
        mvwaddch(window, y, x, ACS_ULCORNER);
        mvwaddch(window, y+height-1, x, ACS_LLCORNER);
        mvwaddch(window, y+1, x, ACS_VLINE);
        mvwaddch(window, y+1, x+length-1, ACS_VLINE);
        mvwaddch(window, y, x+length-1, ACS_URCORNER);
        mvwaddch(window, y+height-1, x+length-1, ACS_LRCORNER);

        int textField = length - 2;
        int center = (int)floor(textField/2);
        int offset = (int)floor(labelLen/2);
        mvwaddstr(window, y+1, x+1+center-offset, label);
        if (state == ButtonState::DIS_HIGHLIGHT)
            mvwchgat(window, y+1, x+1+center-offset, labelLen, UI::getAttribute(Attributes::NORMAL),  (short)UI::getPairNumber(Colors::YELLOW),
                     nullptr);
        if (textHighlightOffset >= 0 && state == ButtonState::ACTIVE) {
            mvwchgat(window, y+1, x+1+center-offset+textHighlightOffset, 1, UI::getAttribute(Attributes::NORMAL),  (short)UI::getPairNumber(Colors::YELLOW),
                     nullptr);
        }
        wattroff(window, UI::getColorPair(renderColor));

        wnoutrefresh(window);
    }
}

void Button::show() {
    setHidden(false);
}

void Button::hide(WINDOW * window) {
    setHidden(true);
    clear(window);
    wnoutrefresh(window);
}

void Button::clear(WINDOW *window) {
    int length;
    int height;
    int x;
    int y;
    getDimensions(length, height);
    getXY(x, y);

    // Clear the button area
    for (int i = 0; i < height; i++) {
        mvwprintw(window, y+i, x, "%*s", length, "");
    }
}

void Button::setState(ButtonState state) {
    this->state = state;
}

Button::~Button() {
    delete[] label;
}

//////////////////////////////////////////////////////////////////////////////

CardBox::CardBox(int x, int y, int length) :
    Element(x, y, length, CARD_BOX_HEIGHT), card(nullptr), front(false),
    isRed(false), isGreen(false)
{
//    int labelLen = length-2;
//    title = new char[labelLen+1];
//    subtitle = new char[labelLen+1];
//    clearText();
}

void CardBox::render(WINDOW *window) {
    if (!isHidden()) {
        clear(window);

        int length;
        int height;
        int x;
        int y;
        getDimensions(length, height);
        getXY(x, y);

        Colors color;
        if (card == nullptr) {
            color = Colors::GRAY;
        } else {
            if (isRed) color = Colors::RED;
            else if (isGreen) color = Colors::GREEN;
            else color = this->color;
        }

        wattron(window, UI::getColorPair(color));
        for (int i = 1; i < length-1; i++) {
            mvwaddch(window, y, x+i, ACS_HLINE);
            mvwaddch(window, y+height-1, x+i, ACS_HLINE);
        }
        for (int i = 1; i < height - 1; i++) {
            mvwaddch(window, y+i, x, ACS_VLINE);
            mvwaddch(window, y+i, x+length-1, ACS_VLINE);
        }
        mvwaddch(window, y, x, ACS_ULCORNER);
        mvwaddch(window, y+height-1, x, ACS_LLCORNER);
        mvwaddch(window, y, x+length-1, ACS_URCORNER);
        mvwaddch(window, y+height-1, x+length-1, ACS_LRCORNER);

        int textField = length - 2;
        int center = (int)floor(textField/2);
        if (card == nullptr) {
            int offset = (int)floor(strlen("Deck Is Currently Empty")/2);
            mvwaddstr(window, y+4, x+1+center-offset, "Deck Is Currently Empty");
        }
        else {
            int titleLen;
            int subtitleLen;
            char* title;
            char* sub;
            if (front) {
                titleLen = card->getFrontTitleLength();
                subtitleLen = card->getFrontSubtitleLength();
                title = card->getFrontTitle();
                sub = card->getFrontSubtitle();
            } else {
                titleLen = card->getBackTitleLength();
                subtitleLen = card->getBackSubtitleLength();
                title = card->getBackTitle();
                sub = card->getBackSubtitle();
            }
            if (titleLen > 0) {
                int titleOffset = (int)floor(titleLen/2);
                if (titleLen % 2 != 0) titleOffset += 1;
                if (subtitleLen > 0)
                    mvwaddstr(window, y+3, x+1+center-titleOffset, title);
                else
                    mvwaddstr(window, y+4, x+1+center-titleOffset, title);
            }
            if (subtitleLen > 0) {
                int subtitleOffset = (int)floor(subtitleLen/2);
                mvwaddstr(window, y+5, x+1+center-subtitleOffset, sub);
            }
        }
        wattroff(window, UI::getColorPair(color));
        wnoutrefresh(window);
    }
}

void CardBox::setSide(bool isFront) {
    this->front = isFront;
}

void CardBox::setCard(Card *card) {
    this->card = card;
}

void CardBox::setColor(Colors color) {
    this->color = color;
}

void CardBox::makeGreen() {
    isGreen = true;
}

void CardBox::makeRed() {
    isRed = true;
}

void CardBox::resetColor() {
    isGreen = false;
    isRed = false;
}

void CardBox::hide(WINDOW *window) {
    setHidden(true);
    clear(window);
    wnoutrefresh(window);
}

void CardBox::show() {
    setHidden(false);
}

void CardBox::clear(WINDOW *window) {
    int length;
    int height;
    int x;
    int y;
    getDimensions(length, height);
    getXY(x, y);

    // Clear the button area
    for (int i = 0; i < height; i++) {
        mvwprintw(window, y+i, x, "%*s", length, "");
    }
}

CardBox::~CardBox() {
//    delete[] title;
//    delete[] subtitle;
}

DeckTable::DeckTable(int x, int y, int length, int height) :
    Element(x, y, length, height), decks(nullptr), deckIndex(-1)
{
}

void DeckTable::render(WINDOW *window) {
    if (!isHidden()) {
        clear(window);
        int length;
        int height;
        int x;
        int y;
        getDimensions(length, height);
        getXY(x, y);
        if (decks == nullptr) {
            mvwaddstr(window, y, x, "Error: DeckList is Null");
            mvwchgat(window, y, x, 48, UI::getAttribute(Attributes::BLINK), (short) UI::getPairNumber(Colors::RED), nullptr);
        } else {
            DeckNode* current = decks->getHead();
            if (current == nullptr) {
                mvwaddstr(window, y, x, "No Decks Created Yet...");
                mvwaddstr(window, y+1, x, "Press 'New Deck' To Create Your First Deck");
                mvwchgat(window, y, x, 48, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::GRAY), nullptr);
                mvwchgat(window, y+1, x, 48, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::GRAY), nullptr);
            } else {
                int index = 0;
                for (int i = y; i < y + height; i++) {
                    mvwaddstr(window, i, x, current->deck->getName());
                    if (deckIndex == index) {
                        mvwchgat(window, i, x, length, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::WHITE_HL), nullptr);
                    }
                    current = current->next;
                    if (current == nullptr) break;
                    index++;
                }
            }
        }
    }
    wnoutrefresh(window);
}

void DeckTable::setDeckList(DeckLinkedList* deckList) {
    decks = deckList;
}

void DeckTable::setIndex(int index) {
    deckIndex = index;
}

void DeckTable::hide(WINDOW *window) {
    setHidden(true);
    clear(window);
    wnoutrefresh(window);
}

void DeckTable::clear(WINDOW *window) {
    int length;
    int height;
    int x;
    int y;
    getDimensions(length, height);
    getXY(x, y);
    for (int i = 0; i < height; i++) {
        mvwprintw(window, y+i, x, "%*s", length, "");
    }
}

void DeckTable::show() {
    setHidden(false);
}

DeckTable::~DeckTable() {
    // Don't release decks.
}

CardTable::CardTable(int x, int y, int length, int height) :
        Element(x, y, length, height), deck(nullptr), cardIndex(0)
{
}

void CardTable::render(WINDOW *window) {
    if (!isHidden()) {
        clear(window);
        int length;
        int height;
        int x;
        int y;
        getDimensions(length, height);
        getXY(x, y);
        if (deck == nullptr) {
            mvwaddstr(window, y, x, "Error: Deck is Null");
            mvwchgat(window, y, x, 48, UI::getAttribute(Attributes::BLINK), (short) UI::getPairNumber(Colors::RED), nullptr);
        } else {
            int size = deck->getSize();
            if (size == 0) {
                wattron(window, UI::getColorPair(Colors::GRAY));
                mvwaddstr(window, y, x, "This Deck is Empty...");
                mvwaddstr(window, y+1, x, "Press 'Add Card' To Create Flash Cards");
                wattroff(window, UI::getColorPair(Colors::GRAY));
            } else {
                for (int i = 0; i < size && i < height; i++) {
                    Card* card = deck->getCard(i);
                    char* frontTitle = card->getFrontTitle();
                    char* backTitle = card->getBackTitle();
                    int frontLen = card->getFrontTitleLength();
                    int backLen = card->getBackTitleLength();
                    mvwaddnstr(window, y+i, x, frontTitle, CARD_TABLE_COLUMN_LEN);
                    if (frontLen > CARD_TABLE_COLUMN_LEN) {
                        mvwaddnstr(window, y + i, x + CARD_TABLE_COLUMN_LEN - 1, "-", 1);
                    }
                    mvwaddnstr(window, y+i, x+CARD_TABLE_COLUMN_LEN+2, backTitle, CARD_TABLE_COLUMN_LEN);
                    if (backLen > CARD_TABLE_COLUMN_LEN) {
                        mvwaddnstr(window, y + i, x + CARD_TABLE_COLUMN_LEN+CARD_TABLE_COLUMN_LEN+1, "-",
                                   1);
                    }
                    if (cardIndex == i) {
                        mvwchgat(window, y+i, x, length, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::WHITE_HL), nullptr);
                    }
                }
            }
        }
    }
    wnoutrefresh(window);
}

void CardTable::setDeck(Deck* deck) {
    this->deck = deck;
}

void CardTable::setIndex(int index) {
    cardIndex = index;
}

void CardTable::hide(WINDOW *window) {
    setHidden(true);
    clear(window);
    wnoutrefresh(window);
}

void CardTable::clear(WINDOW *window) {
    int length;
    int height;
    int x;
    int y;
    getDimensions(length, height);
    getXY(x, y);
    for (int i = 0; i < height; i++) {
        mvwprintw(window, y+i, x, "%*s", length, "");
    }
}

void CardTable::show() {
    setHidden(false);
}

CardTable::~CardTable() {
    // Don't release decks.
}

//////////////////////////////////////////////////////////////////////////////

Form::Form(int x, int y, int length, int height) :
    Element(x, y, length, height), field1(nullptr), field2(nullptr),
    field3(nullptr), field4(nullptr), fieldIndex(0), isDeckEdit(false),
    initialized(false)
{
}

void Form::initializeForDeck() {
    isDeckEdit = true;
    initialized = true;
}

void Form::initializeForCard() {
    isDeckEdit = false;
    initialized = true;
}

void Form::setFormFields(char *field1, char *field2, char *field3, char *field4) {
    this->field1 = field1;
    this->field2 = field2;
    this->field3 = field3;
    this->field4 = field4;
}

void Form::render(WINDOW *window) {
    if (initialized && !isHidden()) {
        clear(window);
        int length;
        int height;
        int x;
        int y;
        getDimensions(length, height);
        getXY(x, y);

        if (isDeckEdit) {
            mvwaddstr(window, y, x,   "        Name:");
            mvwaddstr(window, y+1, x, " Front Label:");
            mvwaddstr(window, y+2, x, "  Back Label:");
            mvwaddstr(window, y+3, x, "Color Scheme:");
            mvwaddstr(window, y+4, x+29, "Save");
            mvwaddstr(window, y+4, x+37, "Cancel");
            mvwchgat(window, y+4, x+29, 4, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::YELLOW), nullptr);
            mvwchgat(window, y+4, x+37, 6, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::YELLOW), nullptr);

            mvwaddstr(window, y, x+14, field1);
            mvwaddstr(window, y+1, x+14, field2);
            mvwaddstr(window, y+2, x+14, field3);
            mvwaddstr(window, y+3, x+14, field4);

            if (fieldIndex < 4)
                mvwchgat(window, y+fieldIndex, x+14, MAX_DECK_INPUT_LEN, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::WHITE_HL), nullptr);
            else if (fieldIndex == 4) {
                mvwchgat(window, y+4, x+29, 4, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::YELLOW_HL), nullptr);
            }
            else if (fieldIndex == 5) {
                mvwchgat(window, y+4, x+37, 6, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::YELLOW_HL), nullptr);
            }
        } else {
            mvwaddstr(window, y, x,   "F Title:");
            mvwaddstr(window, y+1, x, "  F Sub:");
            mvwaddstr(window, y+2, x, "B Title:");
            mvwaddstr(window, y+3, x, "  B Sub:");
            if (fieldIndex < 4)
                mvwchgat(window, y+fieldIndex, 10, length-9, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::WHITE_HL), nullptr);

            mvwaddstr(window, y+4, x+29, "Save");
            mvwaddstr(window, y+4, x+37, "Cancel");
            mvwchgat(window, y+4, x+29, 4, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::YELLOW), nullptr);
            mvwchgat(window, y+4, x+37, 6, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::YELLOW), nullptr);

            mvwaddstr(window, y, x+9, field1);
            mvwaddstr(window, y+1, x+9, field2);
            mvwaddstr(window, y+2, x+9, field3);
            mvwaddstr(window, y+3, x+9, field4);

            if (fieldIndex < 4)
                mvwchgat(window, y+fieldIndex, x+9, MAX_CARD_INPUT_LEN, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::WHITE_HL), nullptr);
            else if (fieldIndex == 4) {
                mvwchgat(window, y+4, x+29, 4, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::YELLOW_HL), nullptr);
            }
            else if (fieldIndex == 5) {
                mvwchgat(window, y+4, x+37, 6, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::YELLOW_HL), nullptr);
            }

        }
        wnoutrefresh(window);
    }
}

void Form::show() {
    setHidden(false);
}

void Form::hide(WINDOW *window) {
    clear(window);
    setHidden(true);
    wnoutrefresh(window);
}

void Form::clear(WINDOW *window) {
    int length;
    int height;
    int x;
    int y;
    getDimensions(length, height);
    getXY(x, y);
    for (int i = 0; i < height; i++) {
        mvwprintw(window, y+i, x, "%*s", length, "");
    }
}

void Form::setIndex(int index) {
    fieldIndex = index;
}

Form::~Form() {
}

//////////////////////////////////////////////////////////////////////////////

QuizModeTable::QuizModeTable(int x, int y, int length, int height) :
        Element(x, y, length, height), frontSide(nullptr), backSide(nullptr), index(0)
{
}

void QuizModeTable::render(WINDOW *window) {
    if (!isHidden()) {
        clear(window);
        int length;
        int height;
        int x;
        int y;
        getDimensions(length, height);
        getXY(x, y);
        int currentIndex = 0;
        for (int i = y; i < y + height; i++) {
            if (currentIndex == 0) {
                mvwaddnstr(window, i, x, frontSide, length);
                if (strlen(frontSide) > length) mvwaddnstr(window, i, length-2, "...", 3);
            } else if (currentIndex == 1) {
                mvwaddnstr(window, i, x, backSide, length);
                if (strlen(backSide) > length) mvwaddnstr(window, i, length-2, "...", 3);
            } else {
                mvwaddstr(window, i, x, "Cancel");
            }
            if (currentIndex == index) {
                mvwchgat(window, i, x, length, UI::getAttribute(Attributes::NORMAL), (short) UI::getPairNumber(Colors::WHITE_HL), nullptr);
            }
            currentIndex++;
        }
        wnoutrefresh(window);
    }
}

void QuizModeTable::setIndex(int index) {
    this->index = index;
}

void QuizModeTable::setSideNames(char *front, char *back) {
    this->frontSide = front;
    this->backSide = back;
}

void QuizModeTable::hide(WINDOW *window) {
    setHidden(true);
    clear(window);
    wnoutrefresh(window);
}

void QuizModeTable::clear(WINDOW *window) {
    int length;
    int height;
    int x;
    int y;
    getDimensions(length, height);
    getXY(x, y);
    for (int i = 0; i < height; i++) {
        mvwprintw(window, y+i, x, "%*s", length, "");
    }
}

void QuizModeTable::show() {
    setHidden(false);
}

QuizModeTable::~QuizModeTable() {
    frontSide = nullptr;
    backSide = nullptr;
}

//////////////////////////////////////////////////////////////////////////////

UI::UI() :
    currentButtonGroup(ButtonGroups::NONE), cursorActive(false), cursorX(0), cursorY(0)
{
    initializeCurses();
    initializeCanvases();
    initializeElements();
}

void UI::initializeCanvases() {
    // Canvas(xPos, yPos, cols, rows, color, borderType)

    C_CONSOLE = new Canvas(0, 0, 50, 5, 3, Colors::WHITE, Borders::TOP);
    C_BUTTONS = new Canvas(0, 18, 50, 8, 17, Colors::WHITE, Borders::BOTTOM);
    C_CARD_VIEW = new Canvas(0, 4, 50, 15, 5, Colors::WHITE, Borders::MIDDLE);
    C_LIST_VIEW = new Canvas(0, 4, 50, 15, 10, Colors::WHITE, Borders::MIDDLE);
    C_EDIT_VIEW = new Canvas(0, 12, 50, 7, 1, Colors::WHITE, Borders::MIDDLE);
    C_QUIZ_MODE = new Canvas(9, 7, 32, 7, 1, Colors::WHITE, Borders::FLOAT);
    C_RESULTS = new Canvas(9, 5, 32, 11, 4, Colors::WHITE, Borders::FLOAT);
    top_panel(C_QUIZ_MODE->panel);
    top_panel(C_EDIT_VIEW->panel);
    top_panel(C_RESULTS->panel);
    update_panels();
    doupdate();

    C_EDIT_VIEW->burnText(2, 0, "[      ]");
    C_CONSOLE->burnRow(2);
    C_CONSOLE->burnText(1, 3, "Deck:");
    C_CONSOLE->burnText(40, 3, "Size:");
    C_CARD_VIEW->burnRow(12);
    C_QUIZ_MODE->burnText(7, 1, "Select Side to Quiz");
    C_QUIZ_MODE->burnRow(2);
    C_RESULTS->burnRow(2);
    C_RESULTS->burnText(13, 1, "Result");
    C_RESULTS->burnText(10, 4, "Correct:");
    C_RESULTS->burnText(8, 5, "Incorrect:");
    C_RESULTS->burnText(10, 6, "Skipped:");
    C_RESULTS->burnText(12, 7, "Grade:");
    C_RESULTS->burnText(23, 9, "Finish", Colors::YELLOW_HL);
}

/*
 * Initialize Curses with a standard configuration and start up color functionality.
 */
void UI::initializeCurses()
{
    // Standard Curses Setup with Custom Colors
    printf("\e[8;26;50t");  // Resize Terminal to 33 Rows and 70 Cols
    initscr();
    start_color();
    curs_set(0);
    init_pair(1, WHITE_VAL, COLOR_BLACK);
    init_pair(2, BLUE_VAL, COLOR_BLACK);
    init_pair(3, RED_VAL, COLOR_BLACK);
    init_pair(4, GREEN_VAL, COLOR_BLACK);
    init_pair(5, YELLOW_VAL, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, WHITE_VAL);
    init_pair(7, GRAY_VAL, COLOR_BLACK);
    init_pair(8, WHITE_VAL, BLUE_BG);
    init_pair(9, CARD_BLUE_FRONT, COLOR_BLACK);
    init_pair(10, CARD_BLUE_BACK, COLOR_BLACK);
    init_pair(11, CARD_RED_FRONT, COLOR_BLACK);
    init_pair(12, CARD_RED_BACK, COLOR_BLACK);
    init_pair(13, YELLOW_VAL, BLUE_BG);
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    box(stdscr, 0, 0);
    bkgd(getColorPair(Colors::GRAY));
    refresh();
}


void UI::initializeElements()
{
    E_CONSOLE = new Label(1, 1, CONSOLE_LABEL_LEN, Colors::YELLOW, Attributes::NORMAL);
    E_DECK_NAME = new Label(7, 3, DECK_LABEL_LEN, Colors::WHITE, Attributes::NORMAL);
    E_DECK_SIZE = new Label(46, 3, DECK_SIZE_LABEL_LEN, Colors::WHITE, Attributes::NORMAL);
    E_SIDE_NAME = new Label(4, 1, SIDE_LABEL_LEN, Colors::WHITE, Attributes::NORMAL);
    E_CARD_NUM = new Label(44, 1, DECK_SIZE_LABEL_LEN, Colors::WHITE, Attributes::NORMAL);
    E_INPUT = new Label(1, 13, INPUT_LABEL_LEN, Colors::WHITE, Attributes::NORMAL);
    E_CORRECT = new Label(19, 4, 2, Colors::GREEN, Attributes::NORMAL);
    E_INCORRECT = new Label(19, 5, 2, Colors::RED, Attributes::NORMAL);
    E_SKIPPED = new Label(19, 6, 2, Colors::YELLOW, Attributes::NORMAL);
    E_GRADE = new Label(19, 7, 4, Colors::WHITE, Attributes::NORMAL);

    E_DECK_METER = new DeckMeter(1, 11, DECK_METER_LEN, Colors::WHITE);

    E_LEFT_ARROW = new Button(1, 1, ARROW_BUTTON_LEN, "<--", 3, Colors::WHITE, 0);
    E_RIGHT_ARROW = new Button(8, 1, ARROW_BUTTON_LEN, "-->", 3, Colors::WHITE, 2);
    E_FLIP_BUTTON = new Button(1, 4, FLIP_BUTTON_LEN, "Flip", 4, Colors::WHITE, 0);
    E_EDIT_BUTTON = new Button(1, 4, FLIP_BUTTON_LEN, "Edit", 4, Colors::WHITE, 0);
    E_OPEN_BUTTON = new Button(1, 4, FLIP_BUTTON_LEN, "Open", 4, Colors::WHITE, 0);
    E_QUIZ_BUTTON = new Button(16, 4, EVEN_BUTTON_LEN, "Quiz", 4, Colors::WHITE, 0);
    E_END_QUIZ_BUTTON = new Button(16, 4, EVEN_BUTTON_LEN, "End Quiz", 8, Colors::WHITE, 0);
    E_CHANGE_DECK_BUTTON = new Button(32, 1, ODD_BUTTON_LEN, "Change Deck", 11, Colors::WHITE, 0);
    E_MODIFY_DECK_BUTTON = new Button(32, 4, ODD_BUTTON_LEN, "Modify Deck", 11, Colors::WHITE, 0);
    E_ADD_CARD_BUTTON = new Button(16, 1, EVEN_BUTTON_LEN, "Add Card", 8, Colors::WHITE, 0);
    E_DELETE_BUTTON  = new Button(16, 4, EVEN_BUTTON_LEN, "Delete", 6, Colors::WHITE, 0);
    E_CARD_VIEW_BUTTON  = new Button(32, 4, ODD_BUTTON_LEN, "Card View", 9, Colors::WHITE, 0);
    E_DELETE_DECK_BUTTON = new Button(32, 1, ODD_BUTTON_LEN, "Delete Deck", 11, Colors::WHITE, 0);
    E_EDIT_DECK_BUTTON = new Button(32, 4, ODD_BUTTON_LEN, "Edit Deck", 9, Colors::WHITE, 0);
    E_NEW_DECK_BUTTON = new Button(16, 1, EVEN_BUTTON_LEN, "New Deck", 8, Colors::WHITE, 0);
    E_EXIT_BUTTON  = new Button(16, 4, EVEN_BUTTON_LEN, "Exit", 4, Colors::WHITE, 1);
    E_ANSWER_BUTTON = new Button(1, 4, FLIP_BUTTON_LEN, "Answer", 6, Colors::WHITE, 0);

    E_DECK_TABLE = new DeckTable(1, 1, DECK_TABLE_LENGTH, DECK_TABLE_HEIGHT);
    E_CARD_TABLE = new CardTable(1, 1, DECK_TABLE_LENGTH, DECK_TABLE_HEIGHT);
    E_QUIZ_MODE_TABLE = new QuizModeTable(1, 3, QUIZ_TABLE_LENGTH, QUIZ_TABLE_HEIGHT);

    E_CARD_BOX = new CardBox(3, 2, CARD_BOX_LENGTH);

    E_EDIT_FORM = new Form(1, 1, FORM_LENGTH, FORM_HEIGHT);

    // Assign Elements to Canvases
    C_CONSOLE->addElement(E_CONSOLE);
    C_CONSOLE->addElement(E_DECK_NAME);
    C_CONSOLE->addElement(E_DECK_SIZE);
    C_CARD_VIEW->addElement(E_SIDE_NAME);
    C_CARD_VIEW->addElement(E_CARD_NUM);
    C_CARD_VIEW->addElement(E_DECK_METER);
    C_CARD_VIEW->addElement(E_INPUT);
    C_CARD_VIEW->addElement(E_CARD_BOX);
    C_LIST_VIEW->addElement(E_CARD_TABLE);
    C_LIST_VIEW->addElement(E_DECK_TABLE);
    C_BUTTONS->addElement(E_LEFT_ARROW);
    C_BUTTONS->addElement(E_RIGHT_ARROW);
    C_BUTTONS->addElement(E_FLIP_BUTTON);
    C_BUTTONS->addElement(E_EDIT_BUTTON);
    C_BUTTONS->addElement(E_QUIZ_BUTTON);
    C_BUTTONS->addElement(E_END_QUIZ_BUTTON);
    C_BUTTONS->addElement(E_EXIT_BUTTON);
    C_BUTTONS->addElement(E_CHANGE_DECK_BUTTON);
    C_BUTTONS->addElement(E_MODIFY_DECK_BUTTON);
    C_BUTTONS->addElement(E_ADD_CARD_BUTTON);
    C_BUTTONS->addElement(E_DELETE_BUTTON);
    C_BUTTONS->addElement(E_CARD_VIEW_BUTTON);
    C_BUTTONS->addElement(E_OPEN_BUTTON);
    C_BUTTONS->addElement(E_DELETE_DECK_BUTTON);
    C_BUTTONS->addElement(E_EDIT_DECK_BUTTON);
    C_BUTTONS->addElement(E_NEW_DECK_BUTTON);
    C_BUTTONS->addElement(E_ANSWER_BUTTON);
    C_EDIT_VIEW->addElement(E_EDIT_FORM);
    C_QUIZ_MODE->addElement(E_QUIZ_MODE_TABLE);
    C_RESULTS->addElement(E_CORRECT);
    C_RESULTS->addElement(E_INCORRECT);
    C_RESULTS->addElement(E_SKIPPED);
    C_RESULTS->addElement(E_GRADE);

    // Create Group Arrays
    deckListButtons[0] = CI_LEFT_ARROW;
    deckListButtons[1] = CI_RIGHT_ARROW;
    deckListButtons[2] = CI_NEW_DECK_BUTTON;
    deckListButtons[3] = CI_DELETE_DECK_BUTTON;
    deckListButtons[4] = CI_OPEN_BUTTON;
    deckListButtons[5] = CI_EXIT_BUTTON;
    deckListButtons[6] = CI_EDIT_DECK_BUTTON;

    cardViewButtons[0] = CI_LEFT_ARROW;
    cardViewButtons[1] = CI_RIGHT_ARROW;
    cardViewButtons[2] = CI_ADD_CARD_BUTTON;
    cardViewButtons[3] = CI_CHANGE_DECK_BUTTON;
    cardViewButtons[4] = CI_FLIP_BUTTON;
    cardViewButtons[5] = CI_QUIZ_BUTTON;
    cardViewButtons[6] = CI_MODIFY_DECK_BUTTON;

    cardListButtons[0] = CI_LEFT_ARROW;
    cardListButtons[1] = CI_RIGHT_ARROW;
    cardListButtons[2] = CI_ADD_CARD_BUTTON;
    cardListButtons[3] = CI_CHANGE_DECK_BUTTON;
    cardListButtons[4] = CI_EDIT_BUTTON;
    cardListButtons[5] = CI_DELETE_BUTTON;
    cardListButtons[6] = CI_CARD_VIEW_BUTTON;

    quizButtons[0] = CI_LEFT_ARROW;
    quizButtons[1] = CI_RIGHT_ARROW;
    quizButtons[2] = CI_ADD_CARD_BUTTON;
    quizButtons[3] = CI_CHANGE_DECK_BUTTON;
    quizButtons[4] = CI_ANSWER_BUTTON;
    quizButtons[5] = CI_END_QUIZ_BUTTON;
    quizButtons[6] = CI_MODIFY_DECK_BUTTON;

    C_BUTTONS->hideElement(CI_LEFT_ARROW);
    C_BUTTONS->hideElement(CI_RIGHT_ARROW);
    C_BUTTONS->hideElement(CI_FLIP_BUTTON);
    C_BUTTONS->hideElement(CI_EDIT_BUTTON);
    C_BUTTONS->hideElement(CI_EXIT_BUTTON);
    C_BUTTONS->hideElement(CI_QUIZ_BUTTON);
    C_BUTTONS->hideElement(CI_END_QUIZ_BUTTON);
    C_BUTTONS->hideElement(CI_CHANGE_DECK_BUTTON);
    C_BUTTONS->hideElement(CI_MODIFY_DECK_BUTTON);
    C_BUTTONS->hideElement(CI_ADD_CARD_BUTTON);
    C_BUTTONS->hideElement(CI_DELETE_BUTTON);
    C_BUTTONS->hideElement(CI_CARD_VIEW_BUTTON);
    C_BUTTONS->hideElement(CI_NEW_DECK_BUTTON);
    C_BUTTONS->hideElement(CI_DELETE_DECK_BUTTON);
    C_BUTTONS->hideElement(CI_EDIT_DECK_BUTTON);
    C_BUTTONS->hideElement(CI_OPEN_BUTTON);
    C_BUTTONS->hideElement(CI_ANSWER_BUTTON);
    C_CARD_VIEW->hideElement(CI_INPUT);

    // Sanity Check, Screen Should Be Blank
    //draw();

}

void UI::draw() {
    curs_set(0);
    C_CONSOLE->render();
    C_CARD_VIEW->render();
    C_BUTTONS->render();
    C_LIST_VIEW->render();
    C_EDIT_VIEW->render();
    C_QUIZ_MODE->render();
    C_RESULTS->render();
    update_panels();
    doupdate();
    if (cursorActive) {
        curs_set(1);
        wmove(cursorCanvas->window, cursorY, cursorX);
        update_panels();
        doupdate();
    }
}

/*
 *  Delete all canvases and elements stored in the UI
 *  one by one...
 */
UI::~UI()
{
    delete C_CARD_VIEW;
    delete C_CONSOLE;
    delete C_LIST_VIEW;
    delete C_BUTTONS;
    delete C_EDIT_VIEW;
    delete C_QUIZ_MODE;
    endwin();
}