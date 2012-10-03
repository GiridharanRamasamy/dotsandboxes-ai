#include "alphabetaai.h"

/**
 * Inits a new AI
 *@param bw board width
 *@param bh board height
 */
AlphaBetaAI::AlphaBetaAI(int bw, int bh): AI(bw, bh) {

}

/**
 * Makes a move, randomly
 *@param board the game board visualization
 *@param score my score
 *@param opponentScore the opponent's score
 *@param pointsRemaining how many points are left this game
 *@return my move
 */
int AlphaBetaAI::makeMove(int* board, int score, int opponentScore, int pointsRemaining) {
    int x = 0;
    int y = 0;

    return coordToInt(x, y);
}
