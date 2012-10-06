#include "montecarlonode.h"
#include "board.h"
#include <cstring>
#include <cstdio>
#include <algorithm>

int MonteCarloNode::size; /**< the size of the state */
int MonteCarloNode::width; /**< the width of the board */

/**
 * Compares two monte carlo nodes
 *@param m1
 *@param m2
 *@return
 */
bool montecmp(MonteCarloNode* m1, MonteCarloNode* m2) {
    return(m1->getValue()>m2->getValue());
}

/**
 * Constructs a new node
 *@param parent the parent node
 *@param pointsRemnainingInGame how many points left in game
 *@param score my score
 *@param opponentScore opponent score
 *@param myTurn whether or not it is my turn
 */
MonteCarloNode::MonteCarloNode(int* _parent, int pointsRemainingInGame, int score, int opponentScore, bool myTurn) {
    //state = new int[size];
    //memcpy((void *)state, (void *)parent, sizeof(int)*size);
    lines = "";
    for (int i = 1; i < size; i+=2) {
        if (_parent[i] == BOARD_EMPTY) {
            lines += "0";
        } else {
            lines += "1";
        }
    }

    visitCount = 0;
    totalGames = 0;
    gameScore = 0;
    move = 0;
    expansion = 0;
    pointsRemaining = pointsRemainingInGame;
    parent = NULL;

    myScore = score;
    otherScore = opponentScore;

    myMove = myTurn;
}

/**
 * Constructs a new node
 *@param parent the parent node
 *@param pointsRemnainingInGame how many points left in game
 *@param score my score
 *@param opponentScore opponent score
 *@param myTurn whether or not it is my turn
 */
MonteCarloNode::MonteCarloNode(std::string _parent, int pointsRemainingInGame, int score, int opponentScore, bool myTurn) {
    //state = new int[size];
    //memcpy((void *)state, (void *)parent, sizeof(int)*size);
    lines = _parent;

    visitCount = 0;
    totalGames = 0;
    gameScore = 0;
    move = 0;
    pointsRemaining = pointsRemainingInGame;
    parent = NULL;

    myScore = score;
    otherScore = opponentScore;

    myMove = myTurn;
}

/**
 * Frees memory
 */
MonteCarloNode::~MonteCarloNode()
{
    //delete[] state;
    for (unsigned int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

/**
 * Indicates that this node has been visited
 */
void MonteCarloNode::visit() {
    visitCount++;
}

/**
 * Returns whether or not this node is this board config
 *@param board the board to check
 *@return true if state == board
 */
bool MonteCarloNode::equals(int* board) {
    for (int i = 1; i < size; i+=2) {
        if ((board[i] == BOARD_EMPTY && lines[i/2] == '1') ||
            (board[i] == BOARD_LINE && lines[i/2] == '0')) {
            return false;
        }
    }
    return true;
}

/**
 * Returns the children node
 *@return children
 */
std::vector<MonteCarloNode *>& MonteCarloNode::getChildren() {
    if (!children.size()) {
        int* state = new int[size];
        generateBoard(state);
        for (int i = 1; i < size; i+=2) {
            if (lines[i/2] == '0') {
                int squares = Board::squaresMade(state, i);
                MonteCarloNode* child = new MonteCarloNode(state, pointsRemaining-squares, myMove?myScore+squares:myScore, !myMove?otherScore+squares:otherScore, squares?myMove:!myMove);
                child->parent = this;
                child->lines[i/2] = '1';
                child->move = i;
                child->evaluate();
                children.push_back(child);
            }
        }
        delete[] state;
    } else {
        //std::sort(children.begin(), children.end(), montecmp);
        //std::random_shuffle(children.begin(), children.end());
    }
    return children;
}

/**
 * Returns the value of the node
 *@return value
 */
float MonteCarloNode::getValue() {
    if (totalGames == 0) {
        return 0;
    }
    return (float)gameScore/(float)totalGames;
}

/**
 * Returns the visit count of the node
 *@return visitCount
 */
int MonteCarloNode::getVisitCount() {
    return visitCount;
}

/**
 * Returns which child should be expanded
 *@return expansion++
 */
int MonteCarloNode::getExpansion() {
    return expansion++;
}

/**
 * Returns a copy of this board state
 *@return copy of state
 */
void MonteCarloNode::generateBoard(int* copy) {
    //memset((void *)copy, BOARD_EMPTY, sizeof(int)*size);
    /*for (int i = 0; i < lines.size(); i++) {
        if (lines[i] == '1') {
            copy[i*2+1] = BOARD_LINE;
        } else {
            copy[i*2+1] = BOARD_EMPTY;
        }
    }*/
    if (parent != NULL) {
        copy[move] = BOARD_LINE;
        parent->generateBoard(copy);
    }
}

/**
 * Sets the board size
 *@param s the board size
 *@param w the board width
 */
void MonteCarloNode::setBoardSize(int s, int w) {
    size = s;
    width = w;
}

/**
 * back propagates the score?
 *@param result result of the simulation
 */
void MonteCarloNode::backpropagate(int result) {
    totalGames++;
    gameScore += result;
}

/**
 * Returns the parent node
 *@return parent
 */
MonteCarloNode* MonteCarloNode::getParent() {
    return parent;
}

/**
 * Returns the move that made this node.
 *@return move
 */
int MonteCarloNode::getMove() {
    return move;
}

/**
 * Returns how many points are remaining
 *@return pointsRemaining
 */
int MonteCarloNode::getPointsRemaining() {
    return pointsRemaining;
}

/**
 * Returns the evaluation of this node
 *@return evaluation
 */
float MonteCarloNode::getEvaluation() {
    return evaluation;
}

/**
 * evaluates how good this node is
 */
void MonteCarloNode::evaluate() {
    int* state = new int[size];
    generateBoard(state);

    int s3 = 0;
    int s2 = 0;

    // first square starts at 1x1 = width+1
    for (int i = width+1; i < size; i+=2) {
        if (state[i] == BOARD_DOT) { // advance to next row
            i += width+1;
            if (i >= size) {
                break;
            }
        }
        // check if s2 or s3
        int sides = 0;
        if (state[i-1] == BOARD_LINE) {
            sides++;
        }
        if (state[i+1] == BOARD_LINE) {
            sides++;
        }
        if (state[i+width] == BOARD_LINE) {
            sides++;
        }
        if (state[i-width] == BOARD_LINE) {
            sides++;
        }
        if (sides == 2) {
            s2++;
        } else if (sides == 3) {
            s3++;
        }
    }

    delete[] state;

    evaluation = 2*myScore-2*otherScore+0.75f*s3-0.5f*s2;
}
