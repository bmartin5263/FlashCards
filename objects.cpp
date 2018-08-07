//
// Created by Brandon Martin on 7/22/18.
//

#include <curses.h>
#include <cstring>
#include <assert.h>
#include "objects.h"

DeckNode::DeckNode(Deck* deck, DeckNode* next, DeckNode* prev) :
    deck(deck), next(next), prev(prev)
{
}

DeckNode::~DeckNode() {
    delete deck;
}

DeckLinkedList::DeckLinkedList() :
    head(nullptr), size(0)
{
}

void DeckLinkedList::addDeck(Deck *deck) {
    if (head == nullptr) head = new DeckNode(deck, nullptr, nullptr);
    else {
        head = new DeckNode(deck, head, nullptr);
        head->next->prev = head;
    }
    size++;
}

void DeckLinkedList::removeDeck(int index) {
    assert(index < size);
    if (index == 0) {
        DeckNode* next = head->next;
        if (next != nullptr)
            next->prev = nullptr;
        DeckNode* oldHead = head;
        head = head->next;
        delete oldHead;
        size--;
        return;
    } else {
        DeckNode* current = head;
        int currentIndex = 0;
        while (current != nullptr) {
            if (currentIndex == index) {
                DeckNode* prev = current->prev;
                DeckNode* next = current->next;
                if (prev != nullptr) {
                    prev->next = current->next;
                }
                if (next != nullptr) {
                    next->prev = current->prev;
                }
                delete current;
                size--;
                return;
            }
            current = current->next;
            currentIndex++;
        }
    }
}

Deck* DeckLinkedList::getDeck(int index) {
    assert(index < size);
    int currentIndex = 0;
    DeckNode* current = head;
    while (current != nullptr) {
        if (currentIndex == index) {
            return current->deck;
        }
        current = current->next;
        currentIndex++;
    }
    return nullptr;
}

int DeckLinkedList::getSize() {
    return size;
}

DeckNode* DeckLinkedList::getHead() {
    return head;
}

DeckLinkedList::~DeckLinkedList() {
    DeckNode* current = head;
    while (current != nullptr) {
        DeckNode* old = current;
        current = current->next;
        delete old;
    }
    head = nullptr;
}

Deck::Deck() :
        colorScheme(ColorSchemes::WHITE), size(0)
{
}

void Deck::setName(const char *name) {
    memset(this->name, '\0', (size_t) MAX_DECK_INPUT_LEN);
    strncpy(this->name, name, MAX_DECK_INPUT_LEN);
}

void Deck::setFrontTitle(const char *front) {
    memset(this->frontTitle, '\0', (size_t) MAX_DECK_INPUT_LEN);
    strncpy(this->frontTitle, front, MAX_DECK_INPUT_LEN);
}

void Deck::setBackTitle(const char *back) {
    memset(this->backTitle, '\0', (size_t) MAX_DECK_INPUT_LEN);
    strncpy(this->backTitle, back, MAX_DECK_INPUT_LEN);
}

char* Deck::getFrontTitle() {
    return frontTitle;
}

char* Deck::getBackTitle() {
    return backTitle;
}

char* Deck::getName() {
    return name;
}

int Deck::getSize() {
    return size;
}

void Deck::addCard(Card *card) {
    cards[size++] = card;
}

Card* Deck::getCard(int index) {
    return cards[index];
}

void Deck::setColorScheme(ColorSchemes scheme) {
    colorScheme = scheme;
}

ColorSchemes Deck::getColorScheme() {
    return colorScheme;
}

void Deck::removeCard(int index) {
    delete cards[index];
    cards[index] = nullptr;
    for (int i = index; i < size; i++) {
        if (index + 1 < MAX_CARDS) {
            cards[i] = cards[i + 1];
        }
        else {
            cards[i] = nullptr;
        }
    }
    size--;
}

Deck::~Deck() {
    for (int i = 0; i < MAX_CARDS; i++) {
        if (cards[i] != nullptr) {
            delete cards[i];
        }
    }
}

Card::Card() :
    frontTitleLen(0), backTitleLen(0), frontSubtitleLen(0), backSubtitleLen(0)
{
}

void Card::setFrontTitle(const char *frontTitle) {
    frontTitleLen = (int)strlen(frontTitle);
    memset(this->frontTitle, '\0', (size_t) MAX_CARD_INPUT_LEN);
    strncpy(this->frontTitle, frontTitle, MAX_CARD_INPUT_LEN);
}

void Card::setBackTitle(const char *backTitle) {
    backTitleLen = (int)strlen(backTitle);
    memset(this->backTitle, '\0', (size_t) MAX_CARD_INPUT_LEN);
    strncpy(this->backTitle, backTitle, MAX_CARD_INPUT_LEN);
}

void Card::setFrontSubtitle(const char *frontSubtitle) {
    frontSubtitleLen = (int)strlen(frontSubtitle);
    memset(this->frontSubtitle, '\0', (size_t) MAX_CARD_INPUT_LEN);
    strncpy(this->frontSubtitle, frontSubtitle, MAX_CARD_INPUT_LEN);
}

void Card::setBackSubtitle(const char *backSubtitle) {
    backSubtitleLen = (int)strlen(backSubtitle);
    memset(this->backSubtitle, '\0', (size_t) MAX_CARD_INPUT_LEN);
    strncpy(this->backSubtitle, backSubtitle, MAX_CARD_INPUT_LEN);
}

char* Card::getFrontTitle() { return frontTitle; }
char* Card::getBackTitle() { return backTitle; }
char* Card::getFrontSubtitle() { return frontSubtitle; }
char* Card::getBackSubtitle() { return backSubtitle; }
int Card::getFrontTitleLength() { return frontTitleLen; }
int Card::getBackTitleLength() { return backTitleLen; }
int Card::getFrontSubtitleLength() { return frontSubtitleLen; }
int Card::getBackSubtitleLength() { return backSubtitleLen; }

Card::~Card() {
}