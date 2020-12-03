/**
 * TO-DO
 * Board class - DONE
 * Player class - DONE
 * Game functionality - DONE
 * MiniMax - DONE
 * Alpha-Beta pruning - DONE
 * OpenGL
 * */


#include <iostream>
#include <utility>
#include <bits/stdc++.h>
#include "include/features.h"
#include "include/mancala.h"

/************************** PROTOTYPES **************************/
int mainMenu();
void howToPlay();
void credits();
void highScores();
bool loadGame();
mancalaSave getInfo(std::string);
void gameMenu(const mancalaSave &);
void welcome();
std::vector<playerSave> getPlayers(std::vector<std::string>);
std::vector<std::string> splitIntoWords(std::string);
void printGameState(mancalaSave);
void gameCall();
bool isHighScore(int);
bool addHighScore(const std::string &, int);
/******************************************************************************/

std::vector<std::string> readLines(std::ifstream &infile);

std::vector<std::string> readLines(std::ifstream &infile) {
    std::vector<std::string> lines;
    std::string tmpString;
    while (getline(infile, tmpString)) {
        lines.push_back(tmpString);
    }
    return lines;
}

bool addHighScore(const std::string &name, int score) {
    std::ifstream fileStream;
    fileStream.open(HIGH_SCORE_FILE);

    if (fileStream.fail()) {
        std::cerr << "\nError: " << strerror(errno);
        return false;
    }
    std::vector<std::string> lines = readLines(fileStream);
    fileStream.close();
    std::vector<std::string> tmpVec;
    std::string scoreString = name + " " + std::to_string(score) + "\n";
    if (lines.empty()) {
        lines.push_back(scoreString);
    } else {
        bool isAdded = false;
        for (int i = 0; i < lines.size(); ++i) {
            tmpVec = splitIntoWords(lines[i]);
            lines[i] += '\n';
            if (score > std::stoi(tmpVec[SCORE]) && !isAdded) {
                std::cout << "\nNEW HIGH SCORE!\n";
                lines.insert(lines.begin() + i, scoreString);
                isAdded = true;
            }

        }
    }
    if (lines.size() > HIGH_SCORE_CAPACITY) {
        lines.resize(HIGH_SCORE_CAPACITY);
    }
    std::ofstream outFileStream;
    outFileStream.open(HIGH_SCORE_FILE);
    for (const auto &line : lines) outFileStream << line;
    outFileStream.close();
    return true;
}

bool isHighScore(int score) {
    std::ifstream fileStream;

    fileStream.open(HIGH_SCORE_FILE);

    if (fileStream.fail()) {
        std::cerr << "\nError: " << strerror(errno);
    } else {
        std::vector<std::string> lines = readLines(fileStream);
        fileStream.close();
        if (lines.size() < 5) {
            return true;
        } else {
            std::vector<std::string> splittedLastLine = splitIntoWords(lines.back());
            if (std::stoi(splittedLastLine[1]) < score)
                return true;
        }
    }
    return false;
}

void printGameState(mancalaSave game) {
    system("clear");
    std::cout << "\n\t\t\tGAME OVER\n";
    if (game.state == FIRST_WIN) {
        std::cout << "\nWinner is: " << game.players[0].name;
        std::cout << "\nLoser is: " << game.players[1].name;
    } else if (game.state == SECOND_WIN) {
        std::cout << "\nWinner is: " << game.players[1].name;
        std::cout << "\nLoser is: " << game.players[0].name;
    } else {
        std::cout << "\nDRAW!";
    }
    std::vector<int> scores;
    scores.push_back(game.board.pits[MAX_PIT_COUNT_PER_PLAYER][0]);
    scores.push_back(game.board.pits[MAX_PIT_COUNT_PER_PLAYER][1]);

    std::cout << "\nFirst Player score: " << scores[0];
    std::cout << "\nSecond Player score: " << scores[1];

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        std::cout << "\nPlayer " << i + 1 << " checking..";
        if (game.players[i].race == HUMAN) {
            if (isHighScore(scores[i])) {
                addHighScore(game.players[i].name, scores[i]);
            }
        }
    }
}

void gameMenu(const mancalaSave &game) {
    switch (game.state) {
        case FIRST_WIN:
        case SECOND_WIN:
        case DRAW:
            printGameState(game);
            break;
        case TURN_BACK_MAIN_MENU:
            welcome();
            break;
        case START_NEW_GAME:
            gameCall();
            break;
        default:
            std::cout << "\nError";
            break;
    }
}

void howToPlay() {
    char c;
    do {
        system("clear");
        std::cout << "\n\t\t************ DISPLAY ************\n\n\n";
        std::cout << "Active player's side shown in below and storage is on right side;\n"
                     "\nPassive player's side shown in above and storage is on left side.\n";
        std::cout << "\n\n\t\t************ TACTICS ************\n\n\n";
        std::cout << "1)RIGHT UP: If last stone's pit in your storage, you will get another turn!\n\n";
        std::cout << "2)EVEN ENEMY PIT: If last stone's pit in enemy's side and if stone count in pit "
                     "is even after last stone added you will get all stones in that pit\n\n";
        std::cout << "3)EMPTY PIT: If last stone's pit your side and the pit is empty, you stole"
                     "stones in enemy's opposite pit and added your storage with your last stone\n\n";
        std::cout << "4)AFTER ALL: If pits' stone count in any side, the player on other side will get"
                     "all stones on the board\n\n";
        std::cout << "\n\t\t************ GAME END ************\n\n\n";
        std::cout << "If stones count in any storage much more than half of the TOTAL_STONE_COUNT(48/2 = 24 in "
                     "default) player on that side wins the game!\n\n";
        std::cout << "\n\nType 'e' to exit!: ";
        std::cin >> c;
    } while (c != 'e' && c != 'E');
}

void credits() {
    char c;
    do {
        system("clear");
        std::cout << "\nDeveloper: Izzet Yilmaz";
        std::cout << "\nGraphics Designer: Izzet Yilmaz";
        std::cout << "\nProducer: Izzet Yilmaz";
        std::cout << "\nThanks to: Izzet Yilmaz";
        std::cout << "\n\n\t\t\tType 'e' to exit!: ";
        std::cin >> c;
    } while (c != 'e' && c != 'E');
}

void highScores() {
    char c;
    do {
        system("clear");
        std::ifstream fileStream;

        fileStream.open(HIGH_SCORE_FILE);

        if (fileStream.fail()) {
            std::cerr << "\nError: " << strerror(errno);
        } else {
            std::cout << "\t\t\t************High Scores************\t\t\t";
            std::vector<std::string> lines = readLines(fileStream);
            for (int i = 0; i < lines.size(); ++i) {
                std::cout << '\n' << i + 1 << ") " << lines[i];
            }

            if (lines.empty()) {
                std::cout << "\n There is no score to show!";
            }
            fileStream.close();
        }
        std::cout << "\n\nType 'e' to exit!: ";
        std::cin >> c;
    } while (c != 'e' && c != 'E');

}

std::vector<std::string> splitIntoWords(std::string targetStr) {
    std::vector<std::string> result;
    std::string i_delim = " ";

    size_t found = targetStr.find(i_delim);
    size_t startIndex = 0;

    while (found != std::string::npos) {
        std::string temp(targetStr.begin() + startIndex, targetStr.begin() + found);
        result.push_back(temp);
        startIndex = found + i_delim.size();
        found = targetStr.find(i_delim, startIndex);
    }
    if (startIndex != targetStr.size())
        result.emplace_back(targetStr.begin() + startIndex, targetStr.end());
    if (result.back().back() == '\n') result.back().pop_back();  // Remove '\n' from last string
    return result;
}

std::vector<playerSave> getPlayers(std::vector<std::string> info) {
    std::vector<playerSave> players = std::vector<playerSave>(PLAYER_COUNT);

    players[0].name = info[0];
    players[0].race = std::stoi(info[1]);
    players[0].side = std::stoi(info[2]);
    players[0].isActive = info[3] == std::string("true");


    players[1].name = info[4];
    players[1].race = std::stoi(info[5]);
    players[1].side = std::stoi(info[6]);
    players[1].isActive = info[7] == std::string("true");

    return players;
}

boardSave getBoard(std::vector<std::string> info) {
    boardSave result;
    for (int i = 9; i <= info.size() - 2; i += 2) {
        result.pits.push_back({std::stoi(info[i]), std::stoi(info[i + 1])});
    }

    return result;
}

mancalaSave getInfo(std::string targetGameSave) {
    /**
     * Info order:
     * First player represented as F
     * Second player represented as S
     * Board represented as B
     *
        * F.name, F.race, F.side, F.isActive, S.name, S.race, S.side, S.isActive, B.pits
     * */
    mancalaSave game;

    std::vector splittedInfo = splitIntoWords(std::move(targetGameSave));

    game.players = getPlayers(splittedInfo);
    game.lastMove = std::stoi(splittedInfo[8]);
    game.board = getBoard(splittedInfo);
    game.state = CONTINUES;
    return game;
}

bool loadGame() {
    char c;
    do {
        system("clear");
        std::ifstream fileStream;
        fileStream.open(SAVED_GAMES_FILE);

        if (fileStream.fail()) {
            std::cerr << "\nError: " << strerror(errno);
        } else {
            std::cout << "\t\t\t************Saved Games************\t\t\t\n";
            std::string currentLine;
            std::vector<std::string> games = readLines(fileStream);
            fileStream.close();
            int i = 1;
            int opt = -1;
            for (int j = 0; j < games.size(); ++j) {
                std::cout << '\n' << j + 1 << ") " << games[j];
            }

            if (games.empty()) {
                std::cout << "\nThere is no game to load!";
            } else {
                do {
                    std::cout << "\nPlease Enter which game you want to play(0 to exit): ";
                    std::cin >> opt;
                } while (opt != 0 && (opt < 1 || opt > games.size()));

                if (opt != 0) {
                    mancalaSave game = getInfo(games[opt - 1]);

                    game = mancala(game);
                    gameMenu(game);
                    char d;
                    do {
                        std::cout << "\nType 'e' to continue: ";
                        std::cin >> d;
                    } while (d != 'e' && d != 'E');
                    welcome();
                    return true;
                } else {
                    return false;
                }
            }
            std::cout << "\n\nType 'e' to exit!: ";
            std::cin >> c;
        }
    } while (c != 'e' && c != 'E');
    return false;
}

int mainMenu() {
    std::string opt;
    do {
        system("clear");
        std::cout << "\nWelcome to Mancala Game!\t\t\t\tMade By Izzet Yilmaz" <<
                  "\n\t1) New Game" <<
                  "\n\t2) Load Game" <<
                  "\n\t3) High Scores" <<
                  "\n\t4) How To Play" <<
                  "\n\t5) Credits" <<
                  "\n\t6) Quit" <<
                  "\nEnter Option: ";
        std::cin >> opt;
    } while (opt != "1" && opt != "2" && opt != "3" && opt != "4" && opt != "5" && opt != "6");
    return std::stoi(opt);
}

std::vector<int> new_game_options() {
    std::string c, d, order;
    do {
        system("clear");
        std::cout << "\n1- Human & Human \n" <<
                  "2- Human & AI\n" <<
                  "3- AI & AI\n" <<
                  "Please Enter the option: ";
        std::cin >> c;
        if (c == HUMAN_VS_HUMAN) {
            return {0, 0};
        }
        if (c == HUMAN_VS_AI) {
            do {
                system("clear");
                std::cout << "\nEnter AI level{1-2-3}: ";
                std::cin >> c;
                std::cout << "\nWho is first, 1 for human another key for AI: ";
                std::cin >> order;
            } while (1 > std::stoi(c) || 3 < std::stoi(c));
            if (order == "1") {
                return {0, std::stoi(c)};
            } else return {std::stoi(c), 0};
        }
        if (c == AI_VS_AI) {
            do {
                system("clear");
                std::cout << "\nEnter AI's level{1-2-3 {WhiteSpace} 1-2-3}: ";
                std::cin >> c;
                std::cin >> d;
            } while ((1 > std::stoi(c) || 3 < std::stoi(c)) && (1 > std::stoi(d) || 3 < std::stoi(d)));
            std::cout << "\nWho is first, 1 for regular another key for reverse: ";
            std::cin >> order;
            if (order == "1") return {std::stoi(c), std::stoi(d)};
            else return {std::stoi(d), std::stoi(c)};
        } else std::cout << "\nEnter between [1-3]\n";
    } while (1 > std::stoi(c) || 3 < std::stoi(c));
    return {};  // Error!
}

void gameCall() {
    std::vector<int> opt;
    int flag = 0;
    while (opt.empty()) {
        opt = new_game_options();
    }
    system("clear");
    std::string name;
    std::vector<playerSave> players(PLAYER_COUNT);
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        if (opt[i] == 0) {
            std::cout << "Please Enter player name: ";
            std::cin >> name;
        } else {
            if (flag == 1) {
                name = "Lucy";
            } else {
                name = "Izzet";
                flag = 1;
            }
        }
        players[i].isActive = i == 0;
        players[i].side = i;
        players[i].race = opt[i];
        players[i].name = name;
    }
    mancalaSave game;
    Board tmpBoard;
    game.board.pits = tmpBoard.getPits();
    game.players = players;
    game = mancala(game);
    gameMenu(game);
    char c;
    std::cout << "Type 'e' to continue: ";
    do {
        std::cin >> c;
    } while (c != 'e' && c != 'E');
    welcome();
}

void welcome() {
    int opt;
    bool isLoaded = false;
    system("clear");
    do {
        opt = mainMenu();
        if (opt == NEW_GAME) {
            gameCall();
        } else if (opt == LOAD_GAME) {
            isLoaded = loadGame();
        } else if (opt == HIGH_SCORES) {
            highScores();
        } else if (opt == CREDITS) {
            credits();
        } else if (opt == HOW_TO_PLAY) {
            howToPlay();
        }
    } while (opt != NEW_GAME && opt != QUIT && !isLoaded);
}

int main() {
    welcome();
    std::cout << "\nQuiting";
    return 0;
}
