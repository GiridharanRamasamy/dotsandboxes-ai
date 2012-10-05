#ifndef MONTECARLONODE_H
#define MONTECARLONODE_H

#include <vector>


class MonteCarloNode
{
    public:
        /**
         * Constructs a new node
         *@param parent the parent node
         *@param pointsRemnainingInGame how many points left in game
         *@param score my score
         *@param opponentScore opponent score
         *@param myTurn whether or not it is my turn
         */
        MonteCarloNode(int* parent, int pointsRemainingInGame, int score, int opponentScore, bool iMadeThis);

        /**
         * Frees memory
         */
        ~MonteCarloNode();

        /**
         * Indicates that this node has been visited
         */
        void visit();

        /**
         * Returns whether or not this node is this board config
         *@param board the board to check
         *@return true if state == board
         */
        bool equals(int* board);

        /**
         * Returns the children node
         *@return children
         */
        std::vector<MonteCarloNode *>& getChildren();

        /**
         * Returns the value of the node
         *@return average of game score
         */
        float getValue();

        /**
         * Returns the visit count of the node
         *@return visitCount
         */
        int getVisitCount();

        /**
         * Returns which child should be expanded
         *@return expansion++
         */
        int getExpansion();

        /**
         * Returns a copy of this board state
         *@return copy of state
         */
        int* copyBoard();

        /**
         * Deletes the copy of the board
         *@param copy the return of copyState()
         */
        void deleteCopy(int* copy);

        /**
         * Sets the board size
         *@param s the board size
         *@param w the board width
         */
        static void setBoardSize(int s, int w);

        /**
         * back propagates the score?
         *@param result result of the simulation
         */
        void backpropagate(int result);

        /**
         * Returns the parent node
         *@return parent
         */
        MonteCarloNode* getParent();

        /**
         * Returns the move that made this node
         *@return move
         */
        int getMove();

        /**
         * Returns how many points are remaining
         *@return pointsRemaining
         */
        int getPointsRemaining();

        /**
         * Returns the evaluation of this node
         *@return evaluation
         */
        float getEvaluation();
    private:
        /**
         * evaluates how good this node is
         */
        void evaluate();

        MonteCarloNode* parent; /**< the parent node */
        std::vector<MonteCarloNode *> children; /**< children nodes */

        int* state; /**< the board state */
        int visitCount; /**< how many times this node has been visited */
        int totalGames; /**< how many games of this state */
        int gameScore; /**< the score of all the games */

        int expansion; /**< the next child that should be expanded */

        int move; /**< the move that made this node */
        int pointsRemaining; /**< how many points are remaining in this node */
        float evaluation; /**< how good this node is */

        int myScore; /**< how many poitns I have */
        int otherScore; /**< how many points arch rival has */
        bool myMove; /**< whether or not my move made this node */

        static int size; /**< the size of the state */
        static int width; /**< the width of the board */
};

#endif // MONTECARLONODE_H
