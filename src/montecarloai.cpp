#include "montecarloai.h"
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define VISIT_THRESHOLD     3

/**
 * Inits a new AI
 *@param bw board width
 *@param bh board height
 */
MonteCarloAI::MonteCarloAI(int bw, int bh): AI(bw, bh) {
    MonteCarloNode::setBoardSize(bw*bh, bw);
    boardSize = bw*bh;
    heapBoard = new int[boardSize];
}

/**
 * Frees memory
 */
MonteCarloAI::~MonteCarloAI() {
    delete[] heapBoard;
    delete nodes[0]; // this will delete all children nodes as well
    //for (unsigned int i = 0; i < nodes.size(); i++) {
    //    delete nodes[i];
    //}
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

    while (time(0) <= start) {
        //printf("%d x %d \n", time(0), start);

        MonteCarloNode* current = root;
        MonteCarloNode* last = NULL;
        while (in_tree(current)) {
            last = current;
            //printf("select\n");
            current = select(current);
        }
        if (current == NULL) { // shouldn't ever be the case, but just in case
            current = last;
        }
        //printf("expand\n");
        last = expand(last);
        //printf("simulate\n");
        memcpy((void *)heapBoard, (void *)board, sizeof(int)*boardSize);
        int result = simulate_game(last, score, opponentScore);
        //printf("backprop\n");
        while (in_tree(current)) {
            current->backpropagate(result);
            current->visit();
            current = current->getParent();
        }
        //printf("done... %d x %d\n", time(0), start);
    }

    std::vector<MonteCarloNode *>& children = root->getChildren();
    int best = children[0]->getMove();
    int mostVisits = children[0]->getVisitCount();
    for (unsigned int i = 1; i < children.size(); i++) {
        if (children[i]->getVisitCount() > mostVisits) {
            mostVisits = children[i]->getVisitCount();
            best = children[i]->getMove();
        }
    }

    //delete nodes[0];
    //nodes.clear();
    for (unsigned int i = 0; i < garbage.size(); i++) {
        for (unsigned int j = i+1; j < garbage.size(); j++) {
            if (garbage[j] == garbage[i]) {
                garbage.erase(garbage.begin()+j);
                j--;
            }
        }
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
    std::vector<MonteCarloNode *>& children = current->getChildren();
    if (!children.size()) {
        return NULL;
    }
    int bestIndex = rand()%children.size();
    float best = 0.f;

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
    /*if (leaf->getVisitCount() > VISIT_THRESHOLD) { // expand all if past visit threshold
        for (unsigned int i = expansion+1; i < children.size(); i++) {
            nodes.push_back(children[i]);
        }
    }*/
    if (expansion >= 0 && expansion < (signed)children.size()) {
        nodes.push_back(children[expansion]);
        return children[expansion];
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
    //printf("copy board...");
    node->generateBoard(heapBoard);
    //printf("done!\n");
    int pointsRemaining = node->getPointsRemaining();
    bool myTurn = true;
    while (true) {
        int move = 0;
        //printf("searching for move (%d)...", pointsRemaining);
        /*for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                printf("%d", heapBoard[i*9+j]);
            }
            printf("\n");
        }*/
        int tries = 0;
        do {
            move = rand()%boardSize;
            if (tries++ > boardSize*4) {
                break;
            }
        } while (heapBoard[move] != BOARD_EMPTY);
        if (tries >= boardSize*4) {
            break;
        }
        //printf("found move %d!\n", move);
        heapBoard[move] = BOARD_LINE;
        int squares = Board::squaresMade(heapBoard, move);
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

        // for some reason points remaining doesn't like to work all of the time
        bool good = false;
        for (int i = 1; i < boardSize; i+=2) {
            if (heapBoard[i] == BOARD_EMPTY) {
                good = true;
                break;
            }
        }
        if (!good) {
            break;
        }
    }

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
    //nodes.clear();
    //for (unsigned int i = 0; i < nodes.size(); i++) {
    //    garbage.push_back(nodes[i]);
    //}
    nodes.clear();
    nodes.push_back(new MonteCarloNode(board, pointsRemaining, score, opponentScore, true));
    return nodes[0];
}
