#ifndef ALPHABETANODE_H
#define ALPHABETANODE_H

#include <vector>
#include <string>

/**
 * A node in the alpha beta search tree
 */
class AlphaBetaNode
{
    public:
        /**
         * Constructs a new a-b node
         *@param board the board to generate from
         *@param parentNode the parent node
         *@param poitnsRemaining
         */
        AlphaBetaNode(int* board, AlphaBetaNode* parentNode, int pointsRemaining);

        /**
         * Frees memory
         */
        ~AlphaBetaNode();

        /**
         * Sets the state size
         *@param s the size
         *@param w the size
         */
        static void setSize(int s, int w);

        /**
         * Determines if this equals that
         *@param board the board to compare with
         *@param poitnsLeft how many points are left
         *@return state == board
         */
        bool equals(int* board, int pointsRemaining);

        /**
         * Returns the moves that can be made in this state
         *@return moves
         */
        std::vector<int>& getMoves();

        /**
         * Returns the board state
         *@return state
         */
        void generateBoard(int* board);

        /**
         * Returns how many points are left in this state
         *@return pointsLeft
         */
        int getPointsLeft();

        /**
         * Alerts this node that it has yet another parent
         *@param parent the new parent
         */
        void hasParent(AlphaBetaNode* parent);

        /**
         * Alerts this node that it had yet another kid
         *@param kid the new kid
         */
        void hasKid(AlphaBetaNode* kid);

        /**
         * Returns the depth of the node
         *@return depth
         */
        int getDepth();

        /**
         * Raises this node
         *@param d how much to raise this node
         */
        void raise(int d);

        /**
         * Kills itself and its kids
         */
        void killSelfAndKids();

        /**
         * Checks if this node is dead
         *@return true if it is dead
         */
        bool isDead();

        /**
         * Returns the children
         *@return kids
         */
        std::vector<AlphaBetaNode *>& getChildren();

        /**
         * Figures out the move that satisfies bestKid
         *@param moveQueue (out) the best moves ever
         */
        int bestMove();

        int alpha; /**< alpha at this node */
        int beta; /**< beta at this node */
        AlphaBetaNode* bestKid; /**< everyon has favorites */

        int myScore; /**< my score */
        int otherScore; /**< other score */
        int pointsLeft; /**< points left */

    private:
        /**
         * Contemplates whether or not it should kill itself and its kids
         *@param parent parent that just killed itself
         */
        void considerKillingSelfAndKids(AlphaBetaNode* parent);

        //int* state; /**< the board state */
        std::vector<int> moves; /**< the moves that can be made in this state */
        std::vector<AlphaBetaNode *> parents; /**< the parent node */
        std::vector<AlphaBetaNode *> kids; /**< the parent node */
        int depth; /**< how many moves deep this node is */

        static int size; /**< size of the states */
        static int width; /**< size of the states */
        static bool init; /**< init */

        std::string lines;
};

#endif // ALPHABETANODE_H
