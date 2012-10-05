#ifndef ALPHABETAAI_H
#define ALPHABETAAI_H

#include <vector>
#include "ai.h"

using namespace std;

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

        int* _board;

        int makeMove(int* board, int score, int opponentScore, int pointsRemaining);


        int alpha_beta(int* _board, int depth, int alpha, int beta, int maxPlayer);

        void generateLegalMoves(int *board, vector<int> &v);

        int makeNextMove(int *_board, vector<int> &moves, int maxPlayer);

        int evaluate(int* _board);


    private:
};

#endif // ALPHABETAAI_H
