add_library(
        Engine STATIC
        Board/Board.cpp
        Board/Board.h
        Board/Bitboard.h
        Utilities/Utilities.h
        MoveGeneration/LeaperPieces.h
        MoveGeneration/PseudoMoves.h
        FenParser/FenParser.h
        FenParser/FenParser.cpp
        Board/BoardState.h
        Board/Bitboard.cpp
        MoveGeneration/PseudoMoves.cpp
        Utilities/Utilities.cpp
        MoveGeneration/SlidingPieces.h
        MoveGeneration/MoveTables.h
        MoveGeneration/MagicNumbers.h
        MoveGeneration/MoveTables.cpp
        MoveGeneration/LeaperPieces.cpp
        MoveGeneration/SlidingPieces.cpp
        MoveGeneration/Move.h
        MoveGeneration/Move.cpp
        Board/BoardOccupancies.h
        Board/BoardOccupancies.cpp
        MoveGeneration/MoveGeneration.h
        MoveGeneration/MoveGeneration.cpp
        MoveGeneration/Draw.h MoveGeneration/Draw.cpp)

target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")