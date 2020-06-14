#include "../include/player.h"


Player::~Player() = default;


Player::Player(int r, std::string n, int ordr) {
    race = r * 2;  // 0 For Human 2, 4, 6 For Computer
    name = std::move(n);
    side = ordr;
    active = ordr == 0;
}


std::string Player::getName() { return name; }


bool Player::isActive() const { return active; }


void Player::setActive(int act) { active = act; }


int Player::getRace() const { return race; }


int Player::getSide() const { return side; }


/* void Player::setSide(int sd) { side = sd; }*/


void Player::makeMove(Board &gameBoard, int &shiftingPit, std::vector<int> &lastStone) {
    if (race > 0) {
//        std::cout << "\nRace bigger than 0 therefore minimax should work";
        /**
         * Determine which pit will be shifting is player is machine
         *
            * First player is maximizer and second player is minimizer
         * */
        long long int alpha = std::numeric_limits<long long int>::min();  // Negative Infinity
        long long int beta = std::numeric_limits<long long int>::max();  // Positive Infinity
        minimax(gameBoard, race, side == 0, alpha, beta, shiftingPit);
    }
    shift(gameBoard, shiftingPit, lastStone, side);
}


void Player::shift(Board &gameBoard, int currentPit, std::vector<int> &lastStone, int currentSide) const {
    std::vector<std::vector<int>> pits = gameBoard.getPits();
    int remainingPit = pits[currentPit][currentSide];

    if (pits[currentPit][currentSide] == 1) {
        pits[currentPit][currentSide] = 0;
    } else {
        pits[currentPit][currentSide] = 1;
        --remainingPit;
    }
    ++currentPit;
    lastStone = {currentPit, currentSide};

    while (remainingPit > 0) {
        if (currentPit == MAX_PIT_COUNT_PER_PLAYER) {
            if (currentSide == side) {
                ++pits[currentPit][currentSide];
                lastStone = {currentPit, currentSide};
                --remainingPit;
            }
            currentPit = 0, currentSide = 1 - currentSide % 2;
        } else {
            ++pits[currentPit][currentSide];
            lastStone = {currentPit, currentSide};
            --remainingPit;
            ++currentPit;
        }
    }
    gameBoard.setPits(pits);
}


long int
Player::minimax(Board &gameBoard, int depth, bool isMax, long long int alpha, long long int beta, int &shiftingPit) {

    Board currentBoard = gameBoard;
    /**
      * Storage for moves
      **/
    std::vector<long long int> moves(MAX_PIT_COUNT_PER_PLAYER);  // Storage for moves, default
    long long int unValidMove;
    /**
     * Default unvalid move for minimizer is int_max from limit library {std::numeric_limits<long long int>::min()}
     * Default unvalid move for maximizer is int_min from limit library {std::numeric_limits<long long int>::max()}
     *
     **/
    long long int maxEval = std::numeric_limits<long long int>::min();
    long long int minEval = std::numeric_limits<long long int>::max();
    /**
   * */
    std::vector<int> currentLastStone, tacticInfo = std::vector<int>(TACTIC_INFO_COUNT);
    int currentTactic, currentSide;
    if (isMax) {
        currentSide = 0;
        /**----------------------------------------------------------------------------------------------
         * Return winner if game ended */
        if (checkStates(gameBoard.getPits(), side) != 0) return checkStates(gameBoard.getPits(), currentSide);
        /**
         *
       * ----------------------------------------------------------------------------------------------*/
        unValidMove = maxEval;
        for (int i = 0; i < MAX_PIT_COUNT_PER_PLAYER; ++i) {
            if (currentBoard.isMoveValid(currentSide, i)) {
                /**
                 * Make move on Current Board
                 * */
                shift(currentBoard, i, currentLastStone, currentSide);
                currentTactic = doTactics(currentBoard, currentLastStone, currentSide, tacticInfo);
                checkSides(currentBoard);
                if (checkStates(gameBoard.getPits(), 0) != 0) {
                    moves[i] = checkStates(gameBoard.getPits(), currentSide);
                } else {
                    if (currentTactic == RIGTH_UP) {
                        /**
                         * If last stone in storehouse Run minimax for changed board with same depth and minimax mode(max - min)
                         * */
                        moves[i] = minimax(currentBoard, depth, true, alpha, beta, shiftingPit);
                    } else {
                        if (depth == 0) {
                            /**
                             * If max depth is exceeded
                             * */
                            moves[i] = currentBoard.getScore();
                        } else {
                            moves[i] = minimax(currentBoard, depth - 1, false, alpha, beta, shiftingPit);
                        }
                    }
                    maxEval = std::max(maxEval, moves[i]);
                    alpha = std::max(alpha, moves[i]);
                    if (beta <= alpha) break;
                    /**
                     * Refresh currentBoard after changing
                     * */
//             std::cout << "\nVALID - Current Mode: " << (isMax ? "MAX":"MIN") << " Current move: "<< moves[i];
                    currentBoard = gameBoard;
                }
            } else {
                moves[i] = unValidMove;
//             std::cout << "\nUNVALID - Current Mode: " << (isMax ? "MAX":"MIN") << " Current move: "<< moves[i];
            }
        }

        shiftingPit = std::max_element(moves.begin(), moves.end()) - moves.begin();  // Determine best option
//         std::cout << "\nMax value: " << *std::max_element(moves.begin(), moves.end());
        return *std::max_element(moves.begin(), moves.end());  // Return best option gain
    } else {
        currentSide = 1;
        /**----------------------------------------------------------------------------------------------
         * Return winner if game ended */
        if (checkStates(gameBoard.getPits(), side) != 0) return checkStates(gameBoard.getPits(), currentSide);
        /**
         *
       * ----------------------------------------------------------------------------------------------*/
        unValidMove = minEval;
        for (int i = 0; i < MAX_PIT_COUNT_PER_PLAYER; ++i) {
            if (currentBoard.isMoveValid(currentSide, i)) {
                /**
                 * Make move on Current Board
                 * */
                shift(currentBoard, i, currentLastStone, currentSide);
                currentTactic = doTactics(currentBoard, currentLastStone, currentSide, tacticInfo);
                checkSides(currentBoard);
                if (checkStates(gameBoard.getPits(), currentSide) != 0) {
                    moves[i] = checkStates(gameBoard.getPits(), currentSide);
                } else {
                    if (currentTactic == RIGTH_UP) {
                        /**
                         * If last stone in storehouse Run minimax for changed board with same depth and minimax mode(max - min)
                         * */
                        moves[i] = minimax(currentBoard, depth, false, alpha, beta, shiftingPit);
                    } else {
                        if (depth == 0) {
                            /**
                             * If max depth is exceeded
                             * */
                            moves[i] = currentBoard.getScore();
                        } else {
                            moves[i] = minimax(currentBoard, depth - 1, true, alpha, beta, shiftingPit);
                        }
                    }
                    minEval = std::min(minEval, moves[i]);
                    beta = std::min(beta, moves[i]);
                    if (beta <= alpha) break;
                    /**
                     * Refresh currentBoard after changing
                     * */
//             std::cout << "\nVALID - Current Mode: " << (isMax ? "MAX":"MIN") << " Current move: "<< moves[i];
                    currentBoard = gameBoard;
                }
            } else {
                moves[i] = unValidMove;
//             std::cout << "\nUNVALID - Current Mode: " << (isMax ? "MAX":"MIN") << " Current move: "<< moves[i];
            }
        }

        shiftingPit = std::min_element(moves.begin(), moves.end()) - moves.begin();  // Determine best option
//         std::cout << "\nMin value: " << *std::min_element(moves.begin(), moves.end());
        return *std::min_element(moves.begin(), moves.end());  // Return best option gain
    }
}
