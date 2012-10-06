#include "alphabetanode.h"
#include "board.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdio>

int AlphaBetaNode::size; /**< size of the states */
int AlphaBetaNode::width; /**< size of the states */

bool AlphaBetaNode::init = false; /**< init */

/**
 * Constructs a new a-b node
 *@param board the board to generate from
 *@param parentNode the parent node
 *@param poitnsRemaining
 */
AlphaBetaNode::AlphaBetaNode(int* board, AlphaBetaNode* parentNode, int pointsRemaining) {
    if (!init) {
        srand(time(0));
        init = true;
    }
    //state = new int[size];
    //memcpy((void *)state, (void *)board, sizeof(int)*size);

    std::vector<int> l3;
    std::vector<int> l4;
    std::vector<int> l0;
    lines = "";
    // generate moves
    for (int i = 1; i < size; i+=2) {
        if (board[i] == BOARD_EMPTY) {
            int sides = 0;
            if (!((i/width)%2)) { // vertical line
                if (i/width > 0) { // not top row
                    if (board[i-width*2] == BOARD_LINE) {
                        sides++;
                    }
                    if (board[i-width-1] == BOARD_LINE) {
                        sides++;
                    }
                    if (board[i-width+1] == BOARD_LINE) {
                        sides++;
                    }
                }
                if (sides != 3 && i/width < size/width-1) { // not bottom row
                    int ns = 0;
                    if (board[i+width*2] == BOARD_LINE) {
                        ns++;
                    }
                    if (board[i+width-1] == BOARD_LINE) {
                        ns++;
                    }
                    if (board[i+width+1] == BOARD_LINE) {
                        ns++;
                    }
                    if (ns > sides) {
                        sides = ns;
                    }
                }
            } else { //horizontal line
                if (i%width > 0) { // not left column
                    if (board[i-2] == BOARD_LINE) {
                        sides++;
                    }
                    if (board[i-1-width] == BOARD_LINE) {
                        sides++;
                    }
                    if (board[i-1+width] == BOARD_LINE) {
                        sides++;
                    }
                }
                if (sides != 3 && i%width < width-1) { // not right column
                    int ns = 0;
                    if (board[i+2] == BOARD_LINE) {
                        ns++;
                    }
                    if (board[i+1-width] == BOARD_LINE) {
                        ns++;
                    }
                    if (board[i+1+width] == BOARD_LINE) {
                        ns++;
                    }
                    if (ns > sides) {
                        sides = ns;
                    }
                }
            }
            switch (sides) {
                case 3:
                    l4.push_back(i);
                    break;
                case 2:
                    l3.push_back(i);
                    break;
                default:
                    l0.push_back(i);
            }
            lines += "0";
        } else {
            lines += "1";
        }
    }

    while (l3.size()) {
        int i = rand()%l3.size();
        moves.push_back(l3[i]);
        l3.erase(l3.begin()+i);
    }
    while (l0.size()) {
        int i = rand()%l0.size();
        moves.push_back(l0[i]);
        l0.erase(l0.begin()+i);
    }
    while (l4.size()) {
        int i = rand()%l4.size();
        moves.push_back(l4[i]);
        l4.erase(l4.begin()+i);
    }


    if (parentNode == NULL) {
        depth = 0;
    } else {
        depth = parentNode->depth+1;
        parents.push_back(parentNode);
        parentNode->kids.push_back(this);
    }
    pointsLeft = pointsRemaining;

    alpha = -9999;
    beta = 9999;
    bestKid = NULL;

    myScore = 0;
    otherScore = 0;
}

/**
 * Frees memory
 */
AlphaBetaNode::~AlphaBetaNode() {
    //delete state;
}


/**
 * Sets the state size
 *@param s the size
 *@param w the size
 */
void AlphaBetaNode::setSize(int s, int w) {
    size = s;
    width = w;
}

/**
 * Determines if this equals that
 *@param board the board to compare with
 *@param poitnsLeft how many points are left
 *@return state == board
 */
bool AlphaBetaNode::equals(int* board, int pointsRemaining) {
    /*if (pointsLeft != pointsRemaining) {
        return false;
    }*/
    for (int i = 1; i < size; i+=2) {
        if ((board[i] == BOARD_EMPTY && lines[i/2] == '0') ||
            (board[i] == BOARD_LINE && lines[i/2] == '1')) {
            return false;
        }
    }
    return true;
}

/**
 * Returns the moves that can be made in this state
 *@return moves
 */
std::vector<int>& AlphaBetaNode::getMoves() {
    return moves;
}

/**
 * Returns the board state
 *@return state
 */
void AlphaBetaNode::generateBoard(int* board) {
    for (int i = 1; i < size; i+=2) {
        board[i] = lines[i/2]-'0';
    }
}

/**
 * Returns how many points are left in this state
 *@return pointsLeft
 */
int AlphaBetaNode::getPointsLeft() {
    return pointsLeft;
}

/**
 * Alerts this node that it has yet another parent
 *@param parent the new parent
 */
void AlphaBetaNode::hasParent(AlphaBetaNode* parent) {
    for (unsigned int i = 0; i < parents.size(); i++) {
        if (parents[i] == parent) {
            return;
        }
    }
    parents.push_back(parent);
}

/**
 * Alerts this node that it had yet another kid
 *@param kid the new kid
 */
void AlphaBetaNode::hasKid(AlphaBetaNode* kid) {
    for (unsigned int i = 0; i < kids.size(); i++) {
        if (kids[i] == kid) {
            return;
        }
    }
    kids.push_back(kid);
}

/**
 * Returns the depth of the node
 *@return depth
 */
int AlphaBetaNode::getDepth() {
    return depth;
}

/**
 * Raises this node
 *@param d how much to raise this node
 */
void AlphaBetaNode::raise(int d) {
    depth -= d;
    if (depth < 0) {
        depth = 0;
    }
}

/**
 * Kills itself and its kids
 */
void AlphaBetaNode::killSelfAndKids() {
    parents.clear();
    //printf("telling kids to kill themselves\n");
    for (unsigned int i = 0; i < kids.size(); i++) {
        kids[i]->considerKillingSelfAndKids(this);
    }
}

/**
 * Contemplates whether or not it should kill itself and its kids
 *@param parent parent that just killed itself
 */
void AlphaBetaNode::considerKillingSelfAndKids(AlphaBetaNode* parent) {
    //printf("maybe kill self?\n");
    for (std::vector<AlphaBetaNode *>::iterator i = parents.begin(); i != parents.end(); i++) {
        //printf("%d === %d   (%d)\n", *i, parent, parents.size());
        if (*i == parent) {
            parents.erase(i);
            //printf("did erase %d   x %d\n", isDead(), kids.size());
            break;
        }
    }
    if (isDead()) {
        for (unsigned int i = 0; i < kids.size(); i++) {
            kids[i]->considerKillingSelfAndKids(this);
        }
    }
}


/**
 * Checks if this node is dead
 *@return true if it is dead
 */
bool AlphaBetaNode::isDead() {
    return(parents.size()==0);
}

/**
 * Returns the children
 *@return kids
 */
std::vector<AlphaBetaNode *>& AlphaBetaNode::getChildren() {
    return kids;
}

/**
 * Figures out the move that satisfies bestKid
 *@param moveQueue (out) the best moves ever
 */
int AlphaBetaNode::bestMove() {
    if (bestKid != NULL) {
        /*for (int i = 1; i < size; i+=2) {
            if (bestKid->state[i] != state[i]) {
                return i;
            }
        }*/
        for (unsigned int i = 0; i < moves.size(); i++) {
            if (bestKid->lines[moves[i]/2] != lines[moves[i]/2]) {
                return moves[i];
            }
        }
    }
    //printf("WTF?!?!?!?!?!?!?!!?!?");
    return moves[0]; // something has gone horribly wrong
}
