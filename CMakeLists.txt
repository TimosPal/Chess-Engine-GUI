cmake_minimum_required(VERSION 3.16)
project(Chess_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chess_engine src/main.cpp src/Board.cpp src/Board.h src/Bitboard.h src/EngineUtil.h src/NonSlidingPieces.h src/MoveGeneration.h src/FenParser.h src/FenParser.cpp src/BoardState.h src/BitBoard.cpp src/MoveGeneration.cpp src/EngineUtil.cpp)