#ifndef ALPHABETAAI_H
#define ALPHABETAAI_H

#include <vector>
#include "pthread.h"
#include "ai.h"
#include "alphabetanode.h"

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

        int alpha_beta(AlphaBetaNode* node, int depth, int maxPlayer, int myScore, int otherScore, int pointsRemaining);

        int makeNextMove(int *_board, int move, int maxPlayer, int& myScore, int& otherScore, int& pointsRemaining);

        void unmakeLastMove(int* _board, int move);

        void unmakeLastMove();

        int evaluate(int* _board, int, int, bool);

        /**
         * Finds the root node
         *@param board the current board
         *@return the root node
         *@param pointsLeft how many points are left in board
         */
        AlphaBetaNode* find_root(int* board, AlphaBetaNode* parent, int pointsLeft);

        /**
         * Trims the tree, killing some of the parents
         *@param board the current board
         *@param score my score
         *@param opponentScore other score
         *@param pointsLeft how many points are left in the game
         *@return the root of it all
         */
        AlphaBetaNode* trim_tree(int* board, int score, int opponentScore, int pointsLeft);

        /**
         * Finds nodes located at a certain depth
         *@param depth the depth to search at
         *@param depthNodes (out)
         */
        void find_depth_nodes(int depth, std::vector<AlphaBetaNode *>& depthNodes);

        /**
         * Magically figures out what the best move is
         *@param node the current node
         *@return the best move (ever)
         */
        int best_move(AlphaBetaNode* node);

        /**
         * Evals the alpha/beta of children
         *@param node the node
         *@param max whether or not to max
         */
        int best_move_eval(AlphaBetaNode* node, bool max);

        std::vector<AlphaBetaNode *> nodes; /**< search tree */
        pthread_t thread; /**< the thread for continuing processing */
        pthread_mutex_t mutex; /**< the mutex for locking content */
        volatile bool run; /**< whether or not to process the game */
        int myScore; /**< my points */
        int otherScore; /**< other points */

        int* heapBoard;
        int boxes;
        bool reset;
};

#endif // ALPHABETAAI_H
