#include "ai.h"

/**
 * Inits a new AI
 *@param bw board width
 *@param bh board height
 */
AI::AI(int bw, int bh) {
    boardWidth = bw;
    boardHeight = bh;
}

/**
 * Converts board coordinates to a single int
 *@param bx board x
 *@param by board y
 *@return bx+by*boardWidth
 */
int AI::coordToInt(int bx, int by) {
    return bx+by*boardWidth;
}
