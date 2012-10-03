#ifndef ALPHABETAAI_H
#define ALPHABETAAI_H

#include "ai.h"

class AlphaBetaAI: public AI
{
    public:
        /**
         * Inits a new AI
         *@param bw board width
         *@param bh board height
         */
        AlphaBetaAI(int bw, int bh);

        /**
         * Makes a move, randomly
         *@param board the game board visualization
         *@param score my score
         *@param opponentScore the opponent's score
         *@param pointsRemaining how many points are left this game
         *@return my move
         */
        int makeMove(int* board, int score, int opponentScore, int pointsRemaining);
    private:
};

#endif // ALPHABETAAI_H
