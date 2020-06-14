//
// Created by lapestand on 18.05.2020.
//

#ifndef MANCALA_FEATURES_H
#define MANCALA_FEATURES_H

#include <iostream>
#include <vector>


/******************* GAME FEATURES *******************/
#define PLAYER_COUNT 2
#define MAX_PIT_COUNT_PER_PLAYER 6
#define MAX_STONE_COUNT_PER_PIT 4

/******************* MAIN MENU OPTIONS *******************/
#define NEW_GAME 1
#define LOAD_GAME 2
#define HIGH_SCORES 3
#define HOW_TO_PLAY 4
#define CREDITS 5
#define QUIT 6

/******************** GAME MENU OPTIONS ************************/
#define OPEN_MENU -1
#define OPEN_MENU_NEW_GAME 2
#define OPEN_MENU_SAVE_GAME 3
#define OPEN_MENU_MAIN_MENU 4
#define OPEN_MENU_QUIT 5


/******************* GAME RESULT *******************/
#define DRAW 3
#define FIRST_WIN 0
#define SECOND_WIN 1
#define CONTINUES 2
#define TURN_BACK_MAIN_MENU 4
#define START_NEW_GAME 5

/******************* TACTICS *******************/
#define NOTHING 0
#define RIGTH_UP 1
#define EMPTY_SIDE 2
#define EVEN_ENEMY_PIT 3
/************/
#define TACTIC 0
#define LAST_STONE_PIT 1
#define LAST_STONE_SIDE 2
#define ENEMY_PIT 3
#define ENEMY_SIDE 4
#define VALUE_IN_LAST_STONES_PIT_BEFORE 5
#define VALUE_IN_LAST_STONES_PIT_AFTER 6
#define ENEMY_BEFORE 7
#define ENEMY_AFTER 8
#define STOLEN_STONES 9
#define TOTAL_STONE_GAIN 10
#define TACTIC_INFO_COUNT 11


/******************* VS OPTIONS *******************/
#define HUMAN 0
#define HUMAN_VS_HUMAN "1"
#define HUMAN_VS_AI "2"
#define AI_VS_AI "3"

/**************************************************/
#define SCORE 1
#define HIGH_SCORE_CAPACITY 5
/**************************************************/

/**************************** INFO DATA *********************************/
struct playerSave {
    std::string name;
    int race;  // Or level
    int side;
    bool isActive;
};

struct boardSave {
    std::vector<std::vector<int>> pits;
};

struct mancalaSave {
    std::vector<playerSave> players = std::vector<playerSave>(PLAYER_COUNT);
    boardSave board;
    /**
     * State saves which mode returned
     * */
    int state;

    /**
     * lastMove to shows last move before game saved
     * */
    int lastMove;
};

#define HIGH_SCORE_FILE "../mancalaFiles/highScores.txt"
#define SAVED_GAMES_FILE "../mancalaFiles/games.mancala"


#endif //MANCALA_FEATURES_H
