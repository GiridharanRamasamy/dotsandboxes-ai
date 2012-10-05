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
         * Frees memory
         */
        ~AlphaBetaAI();

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

        float alpha_beta(int* _board, int depth, float alpha, float beta, int maxPlayer, int myScore, int otherScore, int pointsRemaining);

        void generateLegalMoves(int *board, vector<int> &v);

        int makeNextMove(int *_board, int move, int maxPlayer, int& myScore, int& otherScore, int& pointsRemaining);

        void unmakeLastMove(int* _board, int move);

        void unmakeLastMove();

        float evaluate(int* _board, int, int, bool);

        int* _board;
        int bestMove;
};

#endif // ALPHABETAAI_H
