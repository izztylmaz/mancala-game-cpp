#include "../include/board.h"


Board::~Board() = default;


Board::Board() {
    pits = std::vector<std::vector<int>>(MAX_PIT_COUNT_PER_PLAYER, std::vector<int>(2, MAX_STONE_COUNT_PER_PIT));
    pits.push_back({0, 0});
}


bool Board::isMoveValid(int playerSide, int pitOrder) {
    return !(pitOrder < 0 || pitOrder >= MAX_PIT_COUNT_PER_PLAYER || pits[pitOrder][playerSide] == 0);
}


void Board::drawBoard(int activePlayer, int selectedPit, std::vector<int> tacticInfo) {
//    activePlayer = 0;
    std::system("clear");
    std::cout << std::string(3, '\t');
    std::cout << std::string(MAX_PIT_COUNT_PER_PLAYER * 3 / 2, ' ') << "Board";
    std::cout << "\t\t\tSelected Pit: " << (activePlayer == 0 ? selectedPit : MAX_PIT_COUNT_PER_PLAYER - selectedPit);
    std::cout << '\n';
    std::cout << std::string(3, '\t');
    std::cout << std::string(MAX_PIT_COUNT_PER_PLAYER * 4 - 2, '-');
    if (tacticInfo[TACTIC] != NOTHING) {
        if (tacticInfo[TACTIC] != RIGTH_UP){
            std::cout << "\t\tTactic: " << ((tacticInfo[TACTIC] == EVEN_ENEMY_PIT) ? "Even enemy pit" : "Emtpy pit");
        }else{
            std::cout << "\t\tTactic: " << "Right Up";
        }
    }
    std::cout << "\n\n" << std::string(MAX_PIT_COUNT_PER_PLAYER * 4, ' ');

    for (int i = MAX_PIT_COUNT_PER_PLAYER; i >= 0; --i) {
        std::cout << getPit(i, 1 - activePlayer % 2) << "  ";
    }
    std::cout << std::endl << std::string(MAX_PIT_COUNT_PER_PLAYER * 4 + 3, ' ');
    for (int j = 0; j <= MAX_PIT_COUNT_PER_PLAYER; ++j) {
        std::cout << getPit(j, activePlayer) << "  ";
    }
    std::cout << std::string(3, '\t') <<
              '\n' << std::string(3, '\t') <<
              std::string((MAX_PIT_COUNT_PER_PLAYER * 3) / 2 - 1, ' ') << "Keys" << '\n' << ' ';
    std::cout << std::string(3, '\t') << ' ';
    for (int k = 1; k <= MAX_PIT_COUNT_PER_PLAYER; ++k) {
        std::cout << "  " << k;
    }
    std::cout << '\n';
}


int Board::getPit(int pitNumber, int side) { return pits[pitNumber][side]; }


void Board::setPit(int i, int pitSide, int newValue) { pits[i][pitSide] = newValue; }


std::vector<std::vector<int> > Board::getPits() { return pits; }


void Board::setPits(std::vector<std::vector<int> > p) { pits = std::move(p); }


std::vector<int> Board::validPits(int side) {
    std::vector<int> availablePits;
    for (int i = 0; i < pits.size() - 1; ++i) {
        if (isMoveValid(side, i)) {
            availablePits.push_back(i);
        }
    }
    return availablePits;
}


int Board::getScore() {
    return pits[MAX_PIT_COUNT_PER_PLAYER][0] - pits[MAX_PIT_COUNT_PER_PLAYER][1];
}
