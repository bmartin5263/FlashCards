//
// Created by Brandon Martin on 7/23/18.
//

#ifndef FLASHCARDS_CONSTANTS_H
#define FLASHCARDS_CONSTANTS_H

#define MAX_CARDS 46
#define MAX_CARD_INPUT_LEN 39
#define MAX_DECK_INPUT_LEN 34
#define NUM_BUTTONS 7
#define CARD_TABLE_COLUMN_LEN 23
#define INPUT_FIELD_LENGTH 48

#define INPUT_Y_AXIS 13

#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3

#define ENTER_KEY 10
#define DELETE_KEY 127
#define TAB_KEY 9
#define ESCAPE_KEY 27

// BI = Button Index
#define BI_LEFT_ARROW 0
#define BI_RIGHT_ARROW 1
#define BI_NEW_DECK 2
#define BI_DELETE_DECK 3
#define BI_OPEN 4
#define BI_EXIT 5
#define BI_EDIT_DECK 6
#define BI_ADD_CARD 2
#define BI_CHANGE_DECK 3
#define BI_FLIP 4
#define BI_QUIZ 5
#define BI_MODIFY_DECK 6
#define BI_EDIT 4
#define BI_DELETE 5
#define BI_CARD_VIEW 6
#define BI_END_QUIZ 5
#define BI_ANSWER 4

// Indices of Elements in the Array maintained by the canvases
// Perhaps a hashmap would be easier for the future
// Console Canvas:
#define CI_CONSOLE 0
#define CI_DECK_NAME 1
#define CI_DECK_SIZE 2
// Card View Canvas:
#define CI_SIDE_NAME 0
#define CI_CARD_NUM 1
#define CI_DECK_METER 2
#define CI_INPUT 3
#define CI_CARD_BOX 4
// List View Canvas:
#define CI_CARD_TABLE 0
#define CI_DECK_TABLE 1
// Buttons Canvas:
#define CI_LEFT_ARROW 0
#define CI_RIGHT_ARROW 1
#define CI_FLIP_BUTTON 2
#define CI_EDIT_BUTTON 3
#define CI_QUIZ_BUTTON 4
#define CI_END_QUIZ_BUTTON 5
#define CI_EXIT_BUTTON 6
#define CI_CHANGE_DECK_BUTTON 7
#define CI_MODIFY_DECK_BUTTON 8
#define CI_ADD_CARD_BUTTON 9
#define CI_DELETE_BUTTON 10
#define CI_CARD_VIEW_BUTTON 11
#define CI_OPEN_BUTTON 12
#define CI_DELETE_DECK_BUTTON 13
#define CI_EDIT_DECK_BUTTON 14
#define CI_NEW_DECK_BUTTON 15
#define CI_ANSWER_BUTTON 16
// Edit Canvas:
#define CI_EDIT_FORM 0

enum class ButtonState {
    ACTIVE, DISABLED, HIGHTLIGHTED, PRESSED, DIS_HIGHLIGHT
};

static int movementMap[NUM_BUTTONS][4] = {{4,1,4,3},{4,2,4,0},{5,3,5,1},
                                          {6,0,6,2},{0,5,0,6},{2,6,2,4},
                                          {3,4,3,5}};

#endif //FLASHCARDS_CONSTANTS_H