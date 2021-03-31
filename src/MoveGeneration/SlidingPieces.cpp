#include "SlidingPieces.h"

namespace ChessEngine::MoveGeneration::SlidingPieces {

    using namespace BitboardUtil;
    using namespace MagicNumbers;

    // NOTE: performance isn't crucial for these functions
    // since they are run once when producing the tables.

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

    /* Generate a move table based on the given function for each board position. */
    std::array<Bitboard, 64> CreateMasksTable(Bitboard getMask(uint8_t, uint8_t)) {
        std::array<Bitboard, 64> mask = {};

        for (uint8_t rank = 0; rank < 8; rank++) {
            for (uint8_t file = 0; file < 8; file++) {
                uint8_t squareIndex = GetSquareIndex(file, rank);
                mask[squareIndex] = getMask(file, rank);
            }
        }

        return mask;
    }

    /* Generate a mask based on an initial position and direction */
    Bitboard GetDirectionalBlockerMask(uint8_t file, uint8_t rank, int8_t dirX, int8_t dirY){
        // Exclude outer edges.
        Bitboard mask = BITBOARD_EMPTY;
        uint8_t x = file + dirX, y = rank + dirY;
        while(x > 0 && x < 7 && y > 0 && y < 7) {
            mask |= SetBit(BITBOARD_EMPTY, GetSquareIndex(x, y));
            x += dirX;
            y += dirY;
        }

        return mask;
    }

    /* Generate an attack mask based on an initial position , direction and occupancies */
    Bitboard GetDirectionalMoves(uint8_t file, uint8_t rank, int8_t dirX, int8_t dirY, Bitboard occupancies){
        // Include outer edges if not blocked till that point.
        Bitboard mask = BITBOARD_EMPTY;
        uint8_t x = file + dirX, y = rank + dirY;
        while(x >= 0 && x <= 7 && y >= 0 && y <= 7) {
            Bitboard currentBit = SetBit(BITBOARD_EMPTY, GetSquareIndex(x, y));
            mask |= currentBit;

            // We found a piece and we should stop.
            if((currentBit & occupancies) != 0)
                break;

            x += dirX;
            y += dirY;
        }

        return mask;
    }

    /*******************************************************/
    /* Rook                                                */
    /*******************************************************/

    Bitboard GetRookMask(uint8_t file, uint8_t rank){
        Bitboard mask = BITBOARD_EMPTY;

        mask |= GetDirectionalBlockerMask(file, rank, 0, 1);
        mask |= GetDirectionalBlockerMask(file, rank, 0, -1);
        mask |= GetDirectionalBlockerMask(file, rank, 1, 0);
        mask |= GetDirectionalBlockerMask(file, rank, -1, 0);

        return mask;
    }

    Bitboard GetRookMoves(uint8_t file, uint8_t rank, Bitboard occupancies){
        Bitboard mask = BITBOARD_EMPTY;

        mask |= GetDirectionalMoves(file, rank, 0, 1, occupancies);
        mask |= GetDirectionalMoves(file, rank, 0, -1, occupancies);
        mask |= GetDirectionalMoves(file, rank, 1, 0, occupancies);
        mask |= GetDirectionalMoves(file, rank, -1, 0, occupancies);

        // Exclude self.
        return mask;
    }

    /*******************************************************/
    /* Bishop                                              */
    /*******************************************************/

    Bitboard GetBishopMask(uint8_t file, uint8_t rank){
        Bitboard mask = BITBOARD_EMPTY;

        mask |= GetDirectionalBlockerMask(file, rank, 1, 1);
        mask |= GetDirectionalBlockerMask(file, rank, 1, -1);
        mask |= GetDirectionalBlockerMask(file, rank, -1, 1);
        mask |= GetDirectionalBlockerMask(file, rank, -1, -1);

        return mask;
    }

    Bitboard GetBishopMoves(uint8_t file, uint8_t rank, Bitboard occupancies){
        Bitboard mask = BITBOARD_EMPTY;

        mask |= GetDirectionalMoves(file, rank, 1, 1, occupancies);
        mask |= GetDirectionalMoves(file, rank, 1, -1, occupancies);
        mask |= GetDirectionalMoves(file, rank, -1, 1, occupancies);
        mask |= GetDirectionalMoves(file, rank, -1, -1, occupancies);

        // Exclude self.
        return mask;
    }

    /* Initialize the move array for each square by calculating each occupancy
     * permutation (key) and its corresponding attack (value) , creating an almost perfect hash */
    void InitSlidingMoves(std::array<Bitboard, permutations>& slidingMoves, uint8_t squareIndex, bool forBishop){
        auto [file, rank] = GetCoordinates(squareIndex);

        Bitboard blockerMask;
        uint8_t bitCount;
        if(forBishop){
            bitCount = bishopMaskBitCounts[squareIndex];
            blockerMask = bishopMasks[squareIndex];
        }else{
            bitCount = rookMaskBitCounts[squareIndex];
            blockerMask = rookMasks[squareIndex];
        }

        for (uint16_t permutationIndex = 0; permutationIndex < (1 << bitCount); permutationIndex++) {
            Bitboard occupanciesPermutation = GetPermutation(blockerMask, permutationIndex);

            uint64_t index;
            Bitboard attacks;
            if(forBishop){
                index = BishopMagicHash(blockerMask, squareIndex, bitCount);
                attacks = GetBishopMoves(file, rank, occupanciesPermutation);
            }else{
                index = RookMagicHash(blockerMask, squareIndex, bitCount);
                attacks = GetRookMoves(file, rank, occupanciesPermutation);
            }

            slidingMoves[index] = attacks;
        }
    }

    /*******************************************************/
    /* Attack masks                                        */
    /*******************************************************/

    // Blocker masks , to be used in magic bitboards.
    std::array<BitboardUtil::Bitboard, 64> rookMasks;
    std::array<BitboardUtil::Bitboard, 64> bishopMasks;

    std::array<uint64_t , 64> rookMaskBitCounts;
    std::array<uint64_t , 64> bishopMaskBitCounts;

    void InitBlockerMasks(){
        rookMasks = CreateMasksTable(GetRookMask);
        bishopMasks = CreateMasksTable(GetBishopMask);

        auto rookBitCount = [](uint8_t file, uint8_t rank) {
            return (uint64_t) BitboardUtil::GetBitCount(rookMasks[BitboardUtil::GetSquareIndex(file, rank)]);
        };
        auto bishopBitCount = [](uint8_t file, uint8_t rank) {
            return (uint64_t) BitboardUtil::GetBitCount(bishopMasks[BitboardUtil::GetSquareIndex(file, rank)]);
        };

        rookMaskBitCounts = CreateMasksTable(rookBitCount);
        bishopMaskBitCounts = CreateMasksTable(bishopBitCount);
    }

}
