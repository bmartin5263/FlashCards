#include <iostream>
#include <assert.h>
#include "activities.h"
#include "app.h"
#include <stdlib.h>
#include <signal.h>

bool App::appIsRunning = false;

//void interruptHandler(int sig) {
//    beep();
//    App::appIsRunning = false;
//}

App::App() :
    ui(UI::getInstance()), deckList(nullptr)
{
}

bool App::isValidInput(int input) {
    return  (input >= 32 && input <= 57) || // numbers and symbols
            (input >= 63 && input <= 90) || // uppercase
            (input >= 97 && input <= 122) ||
            input == DELETE_KEY;
}

void App::replaceCharInString(char *string, char find, char replace) {
    int i = 0;
    while (string[i] != '\0') {
        if (string[i] == find) string[i] = replace;
        i++;
    }
}

void App::run() {
    appIsRunning = true;
    //signal(SIGINT, interruptHandler)
    readDecks();
    //deckList = parseDataFile();
    //getTestDeck();

    ui->startupInitialization();
    ui->assignDeckList(deckList);
    DeckLister dl(deckList);
    dl.launch();
    App::saveDecks();
}

DeckLinkedList* App::parseDataFile() {
    return new DeckLinkedList();
}

void App::getTestDeck() {
    Deck* myDeck = new Deck();
    myDeck->setName("My Awesome Deck");
    myDeck->setFrontTitle("FrontSide");
    myDeck->setBackTitle("BackSide");
    myDeck->setColorScheme(ColorSchemes::BLUE);
    Card* myCard = new Card();
    myCard->setFrontTitle("Hello");
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

void App::readDecks() {

    deckList = new DeckLinkedList();

    char const* const fileName = "data.txt";
    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        return;
    }

    bool editingCard = false;
    Deck* newDeck = nullptr;
    Card* newCard = nullptr;
    char* lineSplit[2];

    char line[256] = {0};
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '$') {
            int i = 0;
            char* token = strtok(line, " ");
            while (i < 2 && token != NULL) {
                lineSplit[i++] = token;
                token = strtok(NULL, " ");
            }
        } else if (line[0] == '!') {
            lineSplit[0] = line;
        } else {
            continue;
        }
        if (strcmp(lineSplit[0], "!DECK\n") == 0) {
            if (newDeck != nullptr) {
                if (newCard != nullptr) {
                    newDeck->addCard(newCard);
                    newCard = nullptr;
                }
                deckList->addDeck(newDeck);
            }
            newDeck = new Deck();
            editingCard = false;
        }
        else if (strcmp(lineSplit[0], "!CARD\n") == 0) {
            assert(strcmp(newDeck->getFrontTitle(), "") != 0);
            assert(strcmp(newDeck->getBackTitle(), "") != 0);
            if (newCard != nullptr) {
                newDeck->addCard(newCard);
            }
            newCard = new Card();
            editingCard = true;
        }
        else if (strcmp(lineSplit[0], "$NAME") == 0) {
            char name[MAX_DECK_INPUT_LEN + 1] = {0};
            strncpy(name, lineSplit[1], strlen(lineSplit[1])-1);
            App::replaceCharInString(name, '_', ' ');
            newDeck->setName(name);
        }
        else if (strcmp(lineSplit[0], "$FRONT") == 0) {
            char front[MAX_DECK_INPUT_LEN + 1] = {0};
            strncpy(front, lineSplit[1], strlen(lineSplit[1])-1);
            App::replaceCharInString(front, '_', ' ');
            newDeck->setFrontTitle(front);
        }
        else if (strcmp(lineSplit[0], "$BACK") == 0) {
            char back[MAX_DECK_INPUT_LEN + 1] = {0};
            strncpy(back, lineSplit[1], strlen(lineSplit[1])-1);
            App::replaceCharInString(back, '_', ' ');
            newDeck->setBackTitle(back);
        }
        else if (strcmp(lineSplit[0], "$SCHEME") == 0) {
            if (strcmp(lineSplit[1], "blue\n") == 0) newDeck->setColorScheme(ColorSchemes::BLUE);
            else if (strcmp(lineSplit[1], "red\n") == 0) newDeck->setColorScheme(ColorSchemes::RED);
        }
        else if (strcmp(lineSplit[0], "$FRONT-T") == 0) {
            assert(editingCard);
            char frontTitle[MAX_CARD_INPUT_LEN + 1] = {0};
            strncpy(frontTitle, lineSplit[1], strlen(lineSplit[1])-1);
            App::replaceCharInString(frontTitle, '_', ' ');
            newCard->setFrontTitle(frontTitle);
        }
        else if (strcmp(lineSplit[0], "$BACK-T") == 0) {
            assert(editingCard);
            char backTitle[MAX_CARD_INPUT_LEN + 1] = {0};
            strncpy(backTitle, lineSplit[1], strlen(lineSplit[1])-1);
            App::replaceCharInString(backTitle, '_', ' ');
            newCard->setBackTitle(backTitle);
        }
        else if (strcmp(lineSplit[0], "$FRONT-S") == 0) {
            assert(editingCard);
            char frontSub[MAX_CARD_INPUT_LEN + 1] = {0};
            strncpy(frontSub, lineSplit[1], strlen(lineSplit[1])-1);
            App::replaceCharInString(frontSub, '_', ' ');
            newCard->setFrontSubtitle(frontSub);
        }
        else if (strcmp(lineSplit[0], "$BACK-S") == 0) {
            assert(editingCard);
            char backSub[MAX_CARD_INPUT_LEN + 1] = {0};
            strncpy(backSub, lineSplit[1], strlen(lineSplit[1])-1);
            App::replaceCharInString(backSub, '_', ' ');
            newCard->setBackSubtitle(backSub);
        }
        memset(line, '\0', 256);
    }

    if (newDeck != nullptr) {
        if (newCard != nullptr) {
            newDeck->addCard(newCard);
        }
        deckList->addDeck(newDeck);
    }

    fclose(file);
}

void App::saveDecks() {

    DeckNode* current = deckList->getHead();

    char const* const fileName = "data.txt";
    FILE* file = fopen(fileName, "w");

    if (current == nullptr) {
        fclose(file);
        return;
    }
    while (current->next != nullptr) current = current->next;

    while (current != nullptr) {
        Deck* deck = current->deck;

        fprintf(file, "!DECK\n");
        // Name
        char name[MAX_DECK_INPUT_LEN+1] = {0};
        strncpy(name, deck->getName(), strlen(deck->getName()));
        App::replaceCharInString(name, ' ', '_');
        fprintf(file, "$NAME %s\n", name);
        // Front
        char front[MAX_DECK_INPUT_LEN+1] = {0};
        strncpy(front, deck->getFrontTitle(), strlen(deck->getFrontTitle()));
        App::replaceCharInString(front, ' ', '_');
        fprintf(file, "$FRONT %s\n", front);
        // Back
        char back[MAX_DECK_INPUT_LEN+1] = {0};
        strncpy(back, deck->getBackTitle(), strlen(deck->getBackTitle()));
        App::replaceCharInString(back, ' ', '_');
        fprintf(file, "$BACK %s\n", back);
        // Scheme
        ColorSchemes schemes = deck->getColorScheme();
        switch (schemes) {
            case ColorSchemes::BLUE:
                fprintf(file, "$SCHEME blue\n");
                break;
            case ColorSchemes::RED:
                fprintf(file, "$SCHEME red\n");
                break;
            default:
                fprintf(file, "$SCHEME white\n");
                break;
        }

        for (int i = 0; i < deck->getSize(); i++) {
            Card* card = deck->getCard(i);
            fprintf(file, "\n!CARD\n");

            char frontTitle[MAX_CARD_INPUT_LEN+1] = {0};
            char backTitle[MAX_CARD_INPUT_LEN+1] = {0};
            int frontTitleLen = card->getFrontTitleLength();
            int backTitleLen = card->getBackTitleLength();
            int frontSubtitleLen = card->getFrontSubtitleLength();
            int backSubtitleLen = card->getBackSubtitleLength();

            strncpy(frontTitle, card->getFrontTitle(), (size_t)frontTitleLen);
            App::replaceCharInString(frontTitle, ' ', '_');
            strncpy(backTitle, card->getFrontTitle(), (size_t)backTitleLen);
            App::replaceCharInString(backTitle, ' ', '_');
            fprintf(file, "$FRONT-T %s\n", frontTitle);
            fprintf(file, "$BACK-T %s\n", backTitle);

            if (frontSubtitleLen > 0) {
                char frontSubtitle[MAX_CARD_INPUT_LEN+1] = {0};
                strncpy(frontSubtitle, card->getFrontSubtitle(), (size_t)frontSubtitleLen);
                App::replaceCharInString(frontSubtitle, ' ', '_');
                fprintf(file, "$FRONT-S %s\n", frontSubtitle);
            }
            if (backSubtitleLen > 0) {
                char backSubtitle[MAX_CARD_INPUT_LEN+1] = {0};
                strncpy(backSubtitle, card->getBackSubtitle(), (size_t)backSubtitleLen);
                App::replaceCharInString(backSubtitle, ' ', '_');
                fprintf(file, "$BACK-S %s\n", backSubtitle);
            }
        }
        fprintf(file, "\n");
        current = current->prev;
    }

    fclose(file);

}

App::~App() {
    UI::releaseInstance();
    assert(UI::referenceCountIsZero());
    delete deckList;
}

int main() {
    char escapeString[strlen("ESCDELAY=25") + 1];
    strncpy(escapeString, "ESCDELAY=25", strlen("ESCDELAY=25"));
    putenv(escapeString); // Turns off delay when pressing escape key
    App program;
    program.run();
}