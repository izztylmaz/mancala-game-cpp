#ifndef MANCALA_MANCALA_H
#define MANCALA_MANCALA_H

#include <numeric>
#include <fstream>
#include "board.h"
#include "player.h"
#include "features.h"


mancalaSave mancala(mancalaSave &previousGame);


int
isGameOver(std::vector<std::vector<int>> pits);  // Checks if game is over or not. If it's over checks which player won


int checkStates(const std::vector<std::vector<int>> &pits, int side);


int doTactics(Board &board, const std::vector<int> &lastStone, int activePlayer, std::vector<int> &tacticInfo);

int findTactic(Board &board, std::vector<int> lastStone, int activePlayer);

void checkSides(Board &board);

bool saveGame(mancalaSave);

bool checkIfGameSaved(bool);

std::string prepareData(mancalaSave);

#endif //MANCALA_MANCALA_H