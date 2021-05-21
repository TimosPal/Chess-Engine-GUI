# Chess-Engine
A chess engine implemented with bitboards. This project is split into 2 sections.
The backend / chess engine , which is compiled into a library. A frontend gui that links to the
engine.

## Bitboards
Bitboards are used to represent the board and its properties. The basic idea
is that an 8x8 chess board can be represented by a 64 bit variable (in C/C++ that would be a 
long long unsigned int). If our architecture supports 64 bit registers this offers
great computation speeds.

In this engine instead of using such long types an alias called "Bitboard" is used.

## Fen strings
The board is initialized with a fen string. Fen strings describe a board state , whose turn it is ,
if en passant is available , castling rights , piece positions and half/full move counters. 
A fen string parser is implemented with proper error checking support.

## Move generation
Pseudo moves are generated first. A pseudo move is a move that is legal in isolation but might
not protect against a check or let the king be checked.

We differentiate between two piece categories. Leaper pieces and sliding pieces.
- **Leaper pieces** : pawns , king , knights
- **Sliding pieces** : queen , rook , bishop

Attack / move tables are pre-generated at startup. This way we can get all the possible
pseudo moves for a given position without needing to calculate them on the fly.

Exception : pawn pushes are calculated on the spot due to their simplicity and strong correlation
to the occupancy bitboards when calculating double pushes on the 2nd or 7th ranks.

A pseudo move is considered legal if after being applied it leaves no checks. This approach is 
possibly expensive since we have to copy the board for each pseudo move so it can be further optimized.

## Human player
A human player can make a move by :
- Dragging a piece to a destination square.
- Selecting a piece and a destination square.

## Dependencies
SFML , neither the package or the dlls are included in this repo.
Used : sfml-graphics sfml-audio sfml-window sfml-system