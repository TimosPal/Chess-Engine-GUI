cmake_minimum_required(VERSION 3.16)
project(Chess_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chess_engine src/main.cpp src/Board/Board.cpp src/Board/Board.h src/Board/Bitboard.h src/Utilities/EngineUtil.h src/MoveGeneration/LeaperPieces.h src/MoveGeneration/PseudoMoves.h src/FenParser/FenParser.h src/FenParser/FenParser.cpp src/Board/BoardState.h src/Board/BitBoard.cpp src/MoveGeneration/PseudoMoves.cpp src/Utilities/EngineUtil.cpp src/MoveGeneration/SlidingPieces.h src/MoveGeneration/MoveTables.h src/MoveGeneration/MagicNumbers.h src/MoveGeneration/MoveTables.cpp src/MoveGeneration/LeaperPieces.cpp src/MoveGeneration/SlidingPieces.cpp src/MoveGeneration/Move.h src/MoveGeneration/Move.cpp src/Board/BoardUtilities.h src/Board/BoardUtilities.cpp src/MoveGeneration/MoveGeneration.h)