#ifndef MANCALA_PLAYER_H
#define MANCALA_PLAYER_H

#include <iostream>
#include <vector>
#include <limits>
#include "mancala.h"
#include "board.h"
#include "features.h"

class Player {
private:
    std::string name;
    int race;
    int side;
    bool active;
public:
    Player(int r, std::string n, int ordr);

    ~Player();

/*    void setSide(int sd);  */

    int getSide() const;

    int getRace() const;

    bool isActive() const;

    void setActive(int);

    std::string getName();

    void shift(Board &gameBoard, int pit, std::vector<int> &lastStone, int currentSide) const;

    void makeMove(Board &gameBoard, int &shiftingPit, std::vector<int> &lastPit);

    long int
    minimax(Board &gameBoard, int depth, bool isMax, long long int alpha, long long int beta, int &shiftingPit);
};


#endif //MANCALA_PLAYER_H
