#ifndef MONTECARLOAI_H
#define MONTECARLOAI_H

#include "ai.h"
#include "montecarlonode.h"
#include <vector>

class MonteCarloAI: public AI
{
    public:
        /**
         * Inits a new AI
         *@param bw board width
         *@param bh board height
         */
        MonteCarloAI(int bw, int bh);

        /**
         * Frees memory
         */
        ~MonteCarloAI();

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
        /**
         * Checks if this node is already in our search tree
         *@param node the node we want to check
         *@return true if node is in nodes
         */
        bool in_tree(MonteCarloNode* node);

        /**
         * Selects the next node
         *@param current the current node
         *@return the next node
         */
        MonteCarloNode* select(MonteCarloNode* current);

        /**
         * Expands the leaf node
         *@param leaf the leaf node
         *@return something
         */
        MonteCarloNode* expand(MonteCarloNode* leaf);

        /**
         * Simulates the game based on the move
         *@param node the move
         *@param score my score
         *@param opponentScore the opponent's score
         *@return the result of the game (-1, 0, 1)
         */
        int simulate_game(MonteCarloNode* node, int score, int opponentScore);

        /**
         * Finds the root node
         *@param board the game board
         *@param pointsRemaining how many poitns are left in the game
         *@param score my score
         *@param opponentScore other score
         *@return the root node
         */
        MonteCarloNode* find_root(int* board, int pointsRemaining, int score, int opponentScore);

        std::vector<MonteCarloNode *> nodes; /**< the search tree */
        int boardSize; /**< the size of hte board */
        std::vector<MonteCarloNode *> garbage; /**< garbage */
};

#endif // MONTECARLOAI_H
