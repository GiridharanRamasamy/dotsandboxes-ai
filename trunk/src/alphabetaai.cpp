#include "alphabetaai.h"
#include <limits>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <conio.h>

/**
 * Thread
 *@param ai_pte a-b ai
 */
void* ab_thread(void* ai_ptr) {
    //printf("thread start\n");
    AlphaBetaAI* ai = (AlphaBetaAI *)ai_ptr;
    std::vector<AlphaBetaNode *> depthNodes;

    pthread_mutex_lock(&ai->mutex);
    int depth = 1;
    int from_depth = 0;
    while (ai->run) {
        pthread_mutex_unlock(&ai->mutex);
        depthNodes.clear();
        pthread_mutex_lock(&ai->mutex);
        ai->find_depth_nodes(from_depth, depthNodes);
        if (depthNodes.size() && from_depth < ai->boxes) {
            for (unsigned int i = 0; i < depthNodes.size(); i++) {
                //printf("-----------------------------\n");
                if (depthNodes[i]->myScore < ai->myScore || depthNodes[i]->otherScore < ai->otherScore) {
                    depthNodes[i]->myScore += ai->myScore;
                    depthNodes[i]->otherScore += ai->otherScore;
                }
                ai->alpha_beta(depthNodes[i], depth, !(from_depth%2), depthNodes[i]->myScore, depthNodes[i]->otherScore, depthNodes[i]->getPointsLeft());
                //printf("%d-----------------------------%d\n", i, depthNodes.size());
                pthread_mutex_unlock(&ai->mutex);
                pthread_mutex_lock(&ai->mutex);
                if (ai->reset) {
                    //printf("reset\n");
                    ai->reset = false;
                    from_depth = 0;
                    break;
                }
            }
            from_depth++;
        } else {
            from_depth = 0;
        }
    }
    pthread_mutex_unlock(&ai->mutex);
    //printf("thread end\n");
    pthread_exit(NULL);
    return NULL;
}

/**
 * Inits a new AI
 *@param bw board width
 *@param bh board height
 */
AlphaBetaAI::AlphaBetaAI(int bw, int bh): AI(bw, bh) {
    //_board = new int[bw*bh];
    AlphaBetaNode::setSize(bw*bh, bw);
    run = false;
    pthread_mutex_init(&mutex, NULL);
    myScore = 0;
    otherScore = 0;
    heapBoard = new int[bw*bh];
    boxes = bw*bh/4;
    reset = false;
}

/**
 * Frees memory
 */
AlphaBetaAI::~AlphaBetaAI() {
    void* status;
    pthread_mutex_lock(&mutex);
    run = false;
    pthread_mutex_unlock(&mutex);
    pthread_join(thread, &status);
    pthread_mutex_destroy(&mutex);

    for (unsigned int i = 0; i < nodes.size(); i++) {
        delete nodes[i];
    }
    delete heapBoard;
}

/**
 * Makes a move, Using Alph-Beta Pruning
 *@param board the game board visualization
 *@param score my score
 *@param opponentScore the opponent's score
 *@param pointsRemaining how many points are left this game
 *@return my move
 */
int AlphaBetaAI::makeMove(int* board, int score, int opponentScore, int pointsRemaining) {
    AlphaBetaNode* root = trim_tree(board, score, opponentScore, pointsRemaining);

    if (!run) {
        run = true;
        pthread_create(&thread, NULL, ab_thread, (void *)this);
    }

    int start = time(0);
    while (time(0)-start < 1);

    return best_move(root);
}

/**
 * Alph-Beta Pruning
 *@param depth how deep to search the tree
 *@param alpha the minimax value for alpha
 *@param beta the minimax value for beta
 *@return x y coordinates for move
 */
int AlphaBetaAI::alpha_beta(AlphaBetaNode* node, int depth, int maxPlayer, int myScore, int otherScore, int pointsRemaining)
{
    node->myScore = myScore;
    node->otherScore = otherScore;

    node->generateBoard(heapBoard);

    if (depth <= 0 || pointsRemaining == 0) // or is a terminal node?
    {
        //printf("leaf node!\n");
        int eval = evaluate(heapBoard, myScore, otherScore, maxPlayer);
        return eval;
    }

    vector<int> legalMoves(node->getMoves());
    //std::vector<int> moves;
    //generateLegalMoves(_board, legalMoves);  //vector of all possible legal moves to make

    while (legalMoves.size()) //for each child node of max and min player
    {
        //printf("legal moves: %d\n", legalMoves.size());
        int myPoints = myScore;
        int otherPoints = otherScore;
        int pointsLeft = pointsRemaining;

        bool scored = makeNextMove(heapBoard, legalMoves[legalMoves.size()-1], maxPlayer, myPoints, otherPoints, pointsLeft);
        AlphaBetaNode* kid = find_root(heapBoard, node, pointsLeft);
        node->hasKid(kid);
        kid->hasParent(node);

        //printf("getting eval %d\n", kid);
        float eval = alpha_beta(kid, 0, scored?maxPlayer:!maxPlayer, myPoints, otherPoints, pointsLeft);
        /*if (scored) {
            eval += eval+1;
        }*/
        //printf("got! eval %d\n", kid);

        if (maxPlayer)
        {
            if (eval > node->alpha) {
                node->alpha = eval;
            }
            //printf("%d  %d\n", node->alpha, node->beta);
            if (node->beta <= node->alpha) {
                //printf("max-cut\n");
                unmakeLastMove(heapBoard, legalMoves[legalMoves.size()-1]);
                break; //beta cut-off
            }
        }
        else
        {
            if (eval < node->beta) {
                node->beta = eval;
            }
            //printf("\t\t\t%d  %d\n", node->alpha, node->beta);
            if (node->beta <= node->alpha) {
                //printf("min-cut\n");
                unmakeLastMove(heapBoard, legalMoves[legalMoves.size()-1]);
                break; //alpha cut-off
            }
        }
        //while (moves.size()) {
            unmakeLastMove(heapBoard, legalMoves[legalMoves.size()-1]);
        //    moves.pop_back();
        //}
        legalMoves.pop_back();
        break;
    }//end while loop of all legalMoves

    //printf("about to return thingy\n");
    if (maxPlayer) {
        return node->alpha;
    }
    return node->beta;
}

void AlphaBetaAI::unmakeLastMove(int* _board, int move) {
    _board[move] = BOARD_EMPTY;
}

/**
 * Makes the next move in search
 * @param _board copy board for search
 * @param moves vector reference to legalMoves
 */
int AlphaBetaAI::makeNextMove(int *_board, int move, int maxPlayer, int& myScore, int& otherScore, int& pointsRemaining) {
    _board[move] = BOARD_LINE;
    int squares = Board::squaresMade(_board, move);
    if (squares) {
        pointsRemaining -= squares;
        if (maxPlayer) {
            myScore += squares;
        } else {
            otherScore += squares;
        }
    }
    return squares;
}

/**
 * Calculates utility function
 *@param _board pointer to the current state of the board COPY used to create tree
 *@return eval the value of the function
 */
int AlphaBetaAI::evaluate(int* board, int myScore, int otherScore, bool myMove)
{
    int s3 = 0;
    int s2 = 0;
    int size = boardWidth*boardHeight;

    // first square starts at 1x1 = width+2
    for (int i = boardWidth+1; i < size; i+=2) {
        if (board[i] == BOARD_DOT) { // advance to next row
            i += boardWidth+1;
            if (i >= size) {
                break;
            }
        }
        // check if s2 or s3
        int sides = 0;
        if (board[i-1] == BOARD_LINE) {
            sides++;
        }
        if (board[i+1] == BOARD_LINE) {
            sides++;
        }
        if (board[i+boardWidth] == BOARD_LINE) {
            sides++;
        }
        if (board[i-boardWidth] == BOARD_LINE) {
            sides++;
        }
        if (sides == 2) {
            s2++;
        } else if (sides == 3) {
            s3++;
        }
    }

    /*if (myMove) {
        return 8*myScore-8*otherScore-3*s3+2*s2;
    }
    return 8*otherScore-8*myScore-3*s3+2*s2;*/
    return 8*myScore-8*otherScore+3*s3-2*s2;
}

/**
 * Finds the root node
 *@param board the current board
 *@param parent the parent node
 *@param pointsLeft how many points are left in board
 *@return the root node
 */
AlphaBetaNode* AlphaBetaAI::find_root(int* board, AlphaBetaNode* parent, int pointsLeft) {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (parent != nodes[i] && nodes[i]->equals(board, pointsLeft)) {
            return nodes[i];
        }
    }
    nodes.push_back(new AlphaBetaNode(board, parent, pointsLeft));
    return nodes[nodes.size()-1];
}

/**
 * Trims the tree, killing some of the parents
 *@param board the current board
 *@param score my score
 *@param opponentScore other score
 *@param pointsLeft how many points left in the game
 *@return the root of it all
 */
AlphaBetaNode* AlphaBetaAI::trim_tree(int* board, int score, int opponentScore, int pointsLeft) {
    pthread_mutex_lock(&mutex);
    AlphaBetaNode* root = find_root(board, NULL, pointsLeft);
    //printf("found root");
    int zeroD = root->getDepth();
    // surely there is a better way of doing this...

    // kill parents and reshape tree
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (nodes[i] != root && nodes[i]->getDepth() <= zeroD) {
            //printf("going to kill self\n");
            nodes[i]->killSelfAndKids();
            //printf("managed to kill self\n");
        } else {
            nodes[i]->raise(zeroD);
        }
    }
    // dispose of bodies
    for (int i = 0; i < (signed)nodes.size(); i++) {
        if (nodes[i] != root && nodes[i]->isDead()) {
            //printf("killed one\n");
            delete nodes[i];
            nodes.erase(nodes.begin()+i);
            i--;
        }
    }

    myScore = score;
    otherScore = opponentScore;
    reset = true;

    pthread_mutex_unlock(&mutex);
    return root;
}

/**
 * Finds nodes located at a certain depth
 *@param depth the depth to search at
 *@param depthNodes (out)
 */
void AlphaBetaAI::find_depth_nodes(int depth, std::vector<AlphaBetaNode *>& depthNodes) {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (nodes[i]->getDepth() == depth) {
            depthNodes.push_back(nodes[i]);
        }
    }
}

/**
 * Magically figures out what the best move is
 *@param node the current node
 *@param max whether or not we want to max
 *@return the best move (ever)
 */
int AlphaBetaAI::best_move(AlphaBetaNode* node) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < (signed)node->getChildren().size(); i++) {
        //printf("%d(%d)\n", i, node->getChildren().size());
        float eval = best_move_eval(node->getChildren()[i], false);
        //printf("got eval\n");
        if (eval > node->alpha || node->bestKid == NULL) {
            node->alpha = eval;
            node->bestKid = node->getChildren()[i];
        }
    }
    //for (int i = 0; i < node->getMoves().size(); i++) {
    //    printf("%d ", node->getMoves()[i]);
    //}
    //getch();
    //printf("I think score should be: %d >< %d\n", node->bestKid->otherScore, node->bestKid->myScore);
    int best = node->bestMove();
    pthread_mutex_unlock(&mutex);
    return best;
}

/**
 * Evals the alpha/beta of children
 *@param node the node
 *@param max whether or not to max
 */
int AlphaBetaAI::best_move_eval(AlphaBetaNode* node, bool max) {
    //printf("%d      [%d]   ... %d\n", node, node->getChildren().size(), node->isInbred());
    for (int i = 0; i < (signed)node->getChildren().size(); i++) {
        if (max) {
            //printf("about to max: %d (%d)\n", node->getChildren()[i], node->getChildren()[i]->getChildren().size());
            float eval = best_move_eval(node->getChildren()[i], !max);
            //printf("max done: %d\n", node->getChildren()[i]);
            if (eval > node->alpha) {
                node->alpha = eval;
            }
            if (node->beta <= node->alpha) {
                break; //beta cut-off
            }
        } else {
            //printf("about to min: %d\n", node->getChildren()[i]);
            float eval = best_move_eval(node->getChildren()[i], !max);
            //printf("min done: %d\n", node->getChildren()[i]);
            if (eval < node->beta) {
                node->beta = eval;
            }
            if (node->beta <= node->alpha) {
                break; //beta cut-off
            }
        }
    }
    if (max) {
        return node->alpha;
    }
    return node->beta;
}
