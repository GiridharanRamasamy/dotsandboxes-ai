#ifndef AI_H
#define AI_H

#include "board.h"

class AI
{
    public:
        /**
         * Inits a new AI
         *@param bw board width
         *@param bh board height
         */
        AI(int bw, int bh);

        /**
         * Frees memory
         */
        virtual ~AI();

        /**
         * Makes a move
         *@param board the game board visualization
         *@param score my score
         *@param opponentScore the opponent's score
         *@param pointsRemaining how many points are left this game
         *@return my move
         */
        virtual int makeMove(int* board, int score, int opponentScore, int pointsRemaining) = 0;

    protected:
        /**
         * Converts board coordinates to a single int
         *@param bx board x
         *@param by board y
         *@return bx+by*boardWidth
         */
        int coordToInt(int bx, int by);

        int boardWidth; /**< width of the board visualization */
        int boardHeight; /**< height of the board visualization */
};

#endif // AI_H
