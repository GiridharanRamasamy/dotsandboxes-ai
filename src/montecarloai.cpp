#include "montecarloai.h"
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#define VISIT_THRESHOLD     30

/**
 * Inits a new AI
 *@param bw board width
 *@param bh board height
 */
MonteCarloAI::MonteCarloAI(int bw, int bh): AI(bw, bh) {
    MonteCarloNode::setBoardSize(bw*bh, bw);
    boardSize = bw*bh;
}

/**
 * Frees memory
 */
MonteCarloAI::~MonteCarloAI() {
    delete nodes[0]; // this will delete all children nodes as well
}

/**
 * Makes a move, randomly
 *@param board the game board visualization
 *@param score my score
 *@param opponentScore the opponent's score
 *@param pointsRemaining how many points are left this game
 *@return my move
 */
int MonteCarloAI::makeMove(int* board, int score, int opponentScore, int pointsRemaining) {
    int start = time(0);
    if (!nodes.size()) {
        nodes.push_back(new MonteCarloNode(board, pointsRemaining, score, opponentScore, true));
    }

    MonteCarloNode* root = find_root(board, pointsRemaining, score, opponentScore);

    while (time(0)-start < 1) {
        //printf("%d x %d \n", time(0), start);

        MonteCarloNode* current = root;
        MonteCarloNode* last = NULL;
        while (in_tree(current)) {
            last = current;
            //printf("select\n");
            current = select(current);
        }
        if (current == NULL) {
            current = last;
        }
        //printf("expand\n");
        last = expand(last);
        //printf("simulate\n");
        int result = simulate_game(last, score, opponentScore);
        //printf("backprop\n");
        while (in_tree(current)) {
            current->backpropagate(result);
            current->visit();
            current = current->getParent();
        }
    }

    int mostVisits = 0;
    std::vector<MonteCarloNode *>& children = root->getChildren();
    int best = children[0]->getMove();
    for (unsigned int i = 0; i < children.size(); i++) {
        if (children[i]->getVisitCount() > mostVisits) {
            mostVisits = children[i]->getVisitCount();
            best = children[i]->getMove();
        }
    }

    for (unsigned int i = 0; i < garbage.size(); i++) {
        delete garbage[i];
    }
    garbage.clear();

    return best;
}

/**
 * Checks if this node is already in our search tree
 *@param node the node we want to check
 *@return true if node is in nodes
 */
bool MonteCarloAI::in_tree(MonteCarloNode* node) {
    if (node == NULL) {
        return false;
    }
    for (unsigned int i = 0; i < nodes.size(); i++) {
        //if (nodes[i]->equals(node)) {
        if (nodes[i] == node) {
            return true;
        }
    }
    return false;
}

/**
 * Selects the next node
 *@param current the current node
 *@return the next node
 */
MonteCarloNode* MonteCarloAI::select(MonteCarloNode* current) {
    float best = 0.f;
    std::vector<MonteCarloNode *>& children = current->getChildren();
    if (!children.size()) {
        return NULL;
    }
    int bestIndex = rand()%children.size();

    for (unsigned int i = 0; i < children.size(); i++) {
        if (children[i]->getVisitCount() > VISIT_THRESHOLD) {
            //printf("wow, actually here!");
            float score = children[i]->getValue()+0.7f*sqrt((float)log(current->getVisitCount())/(float)children[i]->getVisitCount());
            if (score > best) {
                best = score;
                bestIndex = i;
            }
        } else {
            float score = children[i]->getEvaluation();
            if (score > best) {
                best = score;
                bestIndex = i;
            }
        }
    }

    return children[bestIndex];
}

/**
 * Expands the leaf node
 *@param leaf the leaf node
 *@return something
 */
MonteCarloNode* MonteCarloAI::expand(MonteCarloNode* leaf) {
    int expansion = leaf->getExpansion();
    std::vector<MonteCarloNode *>& children = leaf->getChildren();
    if (expansion < (signed)children.size()) {
        nodes.push_back(children[expansion]);
        return nodes[nodes.size()-1];
    }
    return leaf;
}

/**
 * Simulates the game based on the move
 *@param node the move
 *@param score my score
 *@param opponentScore the opponent's score
 *@return the result of the game (-1, 0, 1)
 */
int MonteCarloAI::simulate_game(MonteCarloNode* node, int score, int opponentScore) {
    int* board = node->copyBoard();
    int pointsRemaining = node->getPointsRemaining();
    bool myTurn = true;
    while (pointsRemaining > 0) {
        int move = 0;
        //printf("searching for move (%d)...", pointsRemaining);
        do {
            move = rand()%boardSize;
        } while (board[move] != BOARD_EMPTY);
        //printf("found move!\n");
        board[move] = BOARD_LINE;
        int squares = Board::squaresMade(board, move);
        if (!squares) {
            myTurn = !myTurn;
        } else {
            pointsRemaining -= squares;
            if (myTurn) {
                score += squares;
            } else {
                opponentScore += squares;
            }
        }
    }

    node->deleteCopy(board);

    if (score > opponentScore) {
        return 1;
    } else if (score < opponentScore) {
        return -1;
    }
    return 0;
}

/**
 * Finds the root node
 *@param board the game board
 *@param pointsRemaining how many poitns are left in the game
 *@param score my score
 *@param opponentScore other score
 *@return the root node
 */
MonteCarloNode* MonteCarloAI::find_root(int* board, int pointsRemaining, int score, int opponentScore) {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (nodes[i]->equals(board)) {
            return nodes[i];
        }
    }
    // never considered this board config before
    garbage.push_back(nodes[0]);
    nodes.clear();
    nodes.push_back(new MonteCarloNode(board, pointsRemaining, score, opponentScore, true));
    return nodes[0];
}
