#ifndef MANCALA_BOARD_H
#define MANCALA_BOARD_H

#include <iostream>
#include <vector>
#include <utility>
#include "features.h"

class Board {
private:
    std::vector<std::vector<int>> pits;
public:
    ~Board();

    Board();  // New Game
    void drawBoard(int activePlayer, int selectedPit, std::vector<int> tacticInfo);  // Draw Board
    int getPit(int pitNumber, int side);  // Gets stone count in Pit
    void setPit(int i, int pitSide, int newValue);  // Sets stone count in Pit
    std::vector<std::vector<int>> getPits();  // Gets all pits stones counts
    void setPits(std::vector<std::vector<int>> p);  // Sets all pits stones
    bool isMoveValid(int playerSide, int pitOrder);  // Return if move valid or not
    std::vector<int> validPits(int side);  // Returns available pits
    int getScore();  // Returns current score
};

#endif //MANCALA_BOARD_H
