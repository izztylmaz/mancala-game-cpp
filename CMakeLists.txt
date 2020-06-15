cmake_minimum_required(VERSION 3.16)
project(mancala)

set(CMAKE_CXX_STANDARD 20)

add_executable(mancala main.cpp include/board.h src/board.cpp include/player.h src/player.cpp include/mancala.h src/mancala.cpp)