#include "randomai.h"
#include <cstdlib>
#include <ctime>

bool RandomAI::has_seeded = false; /**< don't see again, we'll be fucked */

/**
 * Inits a new AI
 *@param bw board width
 *@param bh board height
 */
RandomAI::RandomAI(int bw, int bh) : AI(bw, bh) {
    if (!has_seeded) {
        srand(time(0));
        has_seeded = true;
    }
}

/**
 * Makes a move, randomly
 *@param board the game board visualization
 *@param score my score
 *@param opponentScore the opponent's score
 *@param pointsRemaining how many points are left this game
 *@return my move
 */
int RandomAI::makeMove(int* board, int score, int opponentScore, int pointsRemaining) {
    int pos = 0;
    do {
        pos = rand()%(boardWidth*boardHeight);
    } while (board[pos]);

    return pos;
}
