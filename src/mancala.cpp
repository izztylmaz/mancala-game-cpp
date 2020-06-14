#include "../include/mancala.h"

#include <utility>

int openGameMenu() {
    std::string opt;
    do {
        system("clear");
        std::cout << "\nWelcome to Mancala Game!\t\t\t\tMade By Izzet Yilmaz" <<
                  "\n\t1) Resume Game" <<
                  "\n\t2) New Game" <<
                  "\n\t3) Save Game" <<
                  "\n\t4) Main Menu" <<
                  "\n\t5) Quit" <<
                  "\nEnter Option: ";
        std::cin >> opt;
    } while (opt != "1" && opt != "2" && opt != "3" && opt != "4" && opt != "5");
    return std::stoi(opt);
}

int isGameOver(std::vector<std::vector<int>> pits) {
    int totalStone = MAX_STONE_COUNT_PER_PIT * MAX_PIT_COUNT_PER_PLAYER * PLAYER_COUNT;
    if (pits.back()[0] > (int) totalStone / 2) { return FIRST_WIN; }
    if (pits.back()[1] > (int) totalStone / 2) { return SECOND_WIN; }
    if (pits.back()[0] == pits.back()[1] == (int) totalStone / 2) { return DRAW; }
    return CONTINUES;
}

int checkStates(const std::vector<std::vector<int>> &pits, int side) {
    int currentState = isGameOver(pits);
    if (currentState == FIRST_WIN) {
//        std::cout << "FIRSTWIN";
        return 100;
    } else if (currentState == SECOND_WIN) {
//        std::cout << "SECONDWIN";
        return -100;
    } else if (currentState == DRAW) {
//        std::cout << "DRAW";
        return side == 0 ? -50 : 50;
    }
    return 0;
}

int findTactic(Board &board, std::vector<int> lastStone, int activePlayer) {
    int lastStonePit = lastStone[0], lastStoneSide = lastStone[1];
    if (lastStoneSide == activePlayer) {  // Taş kendi tarafındaysa
        if (lastStonePit == MAX_PIT_COUNT_PER_PLAYER) {  // Hazineyse
            return RIGTH_UP;  // 1 Yeni hak
        } else if (board.getPit(lastStonePit, lastStoneSide) == 1) {  // Kendi bölgesinde boş bir kuyuya denk geldiyse
            return EMPTY_SIDE; // Rakibin karşı kuyusundaki taşları al
        }
    } else {  // Taş rakibin tarafındaysa
        if (board.getPit(lastStonePit, lastStoneSide) % 2 == 0) {  // Rakibin kuyusunu çift yaparsa
            return EVEN_ENEMY_PIT;
        }
    }
    return NOTHING;
}

int doTactics(Board &board, const std::vector<int> &lastStone, int activePlayer, std::vector<int> &tacticInfo) {
    int tactic = findTactic(board, lastStone, activePlayer);
    std::vector<int> enemyPit;
    tacticInfo[TACTIC] = tactic;
    tacticInfo[LAST_STONE_PIT] = lastStone[0];
    tacticInfo[LAST_STONE_SIDE] = lastStone[1];
    tacticInfo[ENEMY_PIT] = MAX_PIT_COUNT_PER_PLAYER - 1 - lastStone[0];
    tacticInfo[ENEMY_SIDE] = 1 - lastStone[1] % 2;
    tacticInfo[VALUE_IN_LAST_STONES_PIT_BEFORE] = board.getPit(lastStone[0], lastStone[1]);
    tacticInfo[ENEMY_AFTER] = 0;
    tacticInfo[VALUE_IN_LAST_STONES_PIT_AFTER] = 0;
    int stolenStones;
    if (tactic == EMPTY_SIDE) {
        /**
         * Eğer kendi tarafındaki boş kuyuya denk getirirsen rakibin kuyusundaki taşları alırsın
         */
        enemyPit = {MAX_PIT_COUNT_PER_PLAYER - 1 - lastStone[0], 1 - lastStone[1] % 2};
        if (board.getPit(enemyPit[0], enemyPit[1]) > 0) {
            stolenStones = board.getPit(enemyPit[0], enemyPit[1]);
            board.setPit(enemyPit[0], enemyPit[1], 0);
            tacticInfo[STOLEN_STONES] = stolenStones;
            tacticInfo[ENEMY_BEFORE] = stolenStones;
            stolenStones += board.getPit(lastStone[0], lastStone[1]);
            board.setPit(lastStone[0], lastStone[1], 0);
            tacticInfo[TOTAL_STONE_GAIN] = stolenStones;

            stolenStones += board.getPit(MAX_PIT_COUNT_PER_PLAYER, lastStone[1]);
            board.setPit(MAX_PIT_COUNT_PER_PLAYER, lastStone[1], stolenStones);
        }else{
            tacticInfo[TACTIC] = NOTHING;
        }
    } else if (tactic == EVEN_ENEMY_PIT) {
        /**
         * Eğer rakibin kuyusunu çift yaparsan karşılıklı kuyudaki taşları alırsın
         */
        stolenStones = board.getPit(lastStone[0], lastStone[1]);
        board.setPit(lastStone[0], lastStone[1], 0);
        tacticInfo[STOLEN_STONES] = stolenStones;
        tacticInfo[ENEMY_BEFORE] = stolenStones;

        stolenStones += board.getPit(MAX_PIT_COUNT_PER_PLAYER, 1 - lastStone[1] % 2);
        board.setPit(MAX_PIT_COUNT_PER_PLAYER, 1 - lastStone[1] % 2, stolenStones);

    }
    return tactic;
}

void checkSides(Board &board) {
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        int allEmpty = true;
        for (int j = 0; j < MAX_PIT_COUNT_PER_PLAYER; ++j) {
            if (board.getPits()[j][i] != 0) {
                allEmpty = false;
                break;
            }
        }
        if (allEmpty) {
            int sum = 0;
            for (auto &el :board.getPits()) {
                sum += el[i];
            }
            board.setPit(MAX_PIT_COUNT_PER_PLAYER, 1 - i % 2, board.getPit(MAX_PIT_COUNT_PER_PLAYER, 1 - i % 2) + sum);
            for (int j = 0; j < MAX_PIT_COUNT_PER_PLAYER; ++j) {
                board.setPit(j, i, 0);
            }
            break;
        }
    }
}

bool saveGame(mancalaSave currentGame) {
    std::string outputData = prepareData(std::move(currentGame));
    std::ofstream outfile;
    outfile.open(SAVED_GAMES_FILE, std::ios_base::app); // append instead of overwrite
    if (outfile.is_open()) {
        outfile << outputData;
        return true;
    }
    return false;
}

bool checkIfGameSaved(bool isSaved) {
    std::string opt;
    if (!isSaved) {
        do {
            std::cout << "Game not saved, are you sure?(y/n)";
            std::cin >> opt;
        } while (opt != "y" && opt != "Y" && opt != "n" && opt != "N");
        return opt == "y" || opt == "Y";
    } else {
        return true;
    }
}

std::string prepareData(mancalaSave game) {
    std::string gameSave;
    gameSave = game.players[0].name + " ";
    gameSave += std::to_string(game.players[0].race) + " ";
    gameSave += std::to_string(game.players[0].side) + " ";
    gameSave += (game.players[0].isActive ? std::string("true") : std::string("false")) + " ";

    gameSave += game.players[1].name + " ";
    gameSave += std::to_string(game.players[1].race) + " ";
    gameSave += std::to_string(game.players[1].side) + " ";
    gameSave += (game.players[1].isActive ? std::string("true") : std::string("false")) + " ";

    gameSave += std::to_string(game.lastMove) + " ";
    for (auto &pit : game.board.pits) {
        for (int j = 0; j < PLAYER_COUNT; ++j) {
            gameSave += std::to_string(pit[j]) + " ";
        }
    }
    gameSave.pop_back();
    gameSave += "\n";
    return gameSave;
}

mancalaSave mancala(mancalaSave &previousGame) {
    std::vector<playerSave> playerInfos = previousGame.players;
    Board gameBoard;
    gameBoard.setPits(previousGame.board.pits);
    std::vector<Player> players;
    players.reserve(PLAYER_COUNT);
    for (int j = 0; j < PLAYER_COUNT; ++j) {
        if (playerInfos[j].race > 3) playerInfos[j].race /= 2;
        players.emplace_back(Player(playerInfos[j].race, playerInfos[j].name, playerInfos[j].side));
    }
    mancalaSave currentGame;
    int activePlayer, move = previousGame.lastMove, tactic;
    std::vector<int> lastStone, tacticInfo = std::vector<int>(TACTIC_INFO_COUNT);
    bool isSaved = false;

    while (isGameOver(gameBoard.getPits()) == CONTINUES) {
        activePlayer = players[0].isActive() ? 0 : 1;
        gameBoard.drawBoard(activePlayer, move, tacticInfo);
        std::cout << "Player is: " << players[activePlayer].getName() << '\n';
        if (players[activePlayer].getRace() == HUMAN) {
            std::cout << "\nEnter move(0 for menu): ";
            std::cin >> move;
            --move;
            if (move == OPEN_MENU) {
                int menuOpt = openGameMenu();
                if (menuOpt == OPEN_MENU_NEW_GAME) {
                    if (checkIfGameSaved(isSaved)) {
                        currentGame.state = START_NEW_GAME;
                        return currentGame;
                    }
                } else if (menuOpt == OPEN_MENU_SAVE_GAME) {
                    isSaved = saveGame(currentGame);
                } else if (menuOpt == OPEN_MENU_MAIN_MENU) {
                    if (checkIfGameSaved(isSaved)) {
                        currentGame.state = TURN_BACK_MAIN_MENU;
                        return currentGame;
                    }
                } else if (menuOpt == OPEN_MENU_QUIT) {
                    if (checkIfGameSaved(isSaved)) {
                        std::cout << "\nSee you Later";
                        currentGame.state = QUIT;
                        return currentGame;
                    }
                }
                continue;
            } else if (!gameBoard.isMoveValid(activePlayer, move)) {
                std::cout << "\nUNVALID MOVE";
                continue;
            }
        } else {
            std::cout << "\nCalculating...\n";
        }


        players[activePlayer].makeMove(gameBoard, move, lastStone);
        if (move != OPEN_MENU) currentGame.lastMove = move;

        if (players[activePlayer].getRace() == HUMAN) {
            std::cout << "\nMove valid\n";
        }

        isSaved = false;

        /***/
        players[activePlayer].setActive(0);
        players[1 - activePlayer % 2].setActive(1);
        /***/


        int tmpTactic = doTactics(gameBoard, lastStone, activePlayer, tacticInfo);

        if (tmpTactic == RIGTH_UP) {
            players[activePlayer].setActive(1);
            players[1 - activePlayer % 2].setActive(0);
        }

        checkSides(gameBoard);
        for (int i = 0; i < PLAYER_COUNT; ++i) {
            currentGame.players[i].name = players[i].getName();
            currentGame.players[i].race = players[i].getRace();
            currentGame.players[i].side = players[i].getSide();
            currentGame.players[i].isActive = players[i].isActive();
        }
        currentGame.board.pits = gameBoard.getPits();
        currentGame.state = isGameOver(gameBoard.getPits());
    }
    return currentGame;
}