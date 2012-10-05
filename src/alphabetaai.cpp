#include "alphabetaai.h"
#include <limits>
#include <cmath>
#include <cstring>

/**
 * Inits a new AI
 *@param bw board width
 *@param bh board height
 */
AlphaBetaAI::AlphaBetaAI(int bw, int bh): AI(bw, bh) {
    _board = new int[bw*bh];
    bestMove = 0;
}

/**
 * Frees memory
 */
AlphaBetaAI::~AlphaBetaAI() {
    delete _board;
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
    int depth = 5; //needs to be a time limit?..Iterative Deepening
    int inf = numeric_limits<int>::max();

    //Create copy of the board for use in search tree
    //_board = new int[boardWidth*boardHeight];
    /*for (int i=0; i < boardWidth*boardHeight; i++)
    {
        _board[i] = board[i];
    }*/
    // found a probably faster copy function than iterating
    memcpy((void *)_board, (void *)board, sizeof(int)*boardWidth*boardHeight);

    int maxPlayer = 1;
    alpha_beta( _board, depth, -inf, inf, maxPlayer, score, opponentScore, pointsRemaining); //calls the alpha_beta search process

    return bestMove;
}

/**
 * Alph-Beta Pruning
 *@param depth how deep to search the tree
 *@param alpha the minimax value for alpha
 *@param beta the minimax value for beta
 *@return x y coordinates for move
 */
float AlphaBetaAI::alpha_beta(int* _board, int depth, float alpha, float beta, int maxPlayer, int myScore, int otherScore, int pointsRemaining)
{
    if (depth == 0 || pointsRemaining == 0) // or is a terminal node?
    {
        return evaluate(_board, myScore, otherScore, maxPlayer);
    }

    vector<int> legalMoves;
    generateLegalMoves(_board, legalMoves);  //vector of all possible legal moves to make

    while (!legalMoves.empty()) //for each child node of max and min player
    {
        int myPoints = myScore;
        int otherPoints = otherScore;
        int pointsLeft = pointsRemaining;
        int moves = 0;
        do {
            moves++;
            /// TODO: legalMoves[legalMoves.size()-moves]  is where we put move ordering
        } while (moves < legalMoves.size() && makeNextMove(_board, legalMoves[legalMoves.size()-moves], maxPlayer, myPoints, otherPoints, pointsLeft));
        if (moves == legalMoves.size()) {
            moves--;
        }
        //maxPlayer = makeNextMove(_board, legalMoves, maxPlayer); //changes betweens turn unless box completed
        if (maxPlayer)
        {
            float eval = alpha_beta(_board, depth-1, alpha, beta, !maxPlayer, myPoints, otherPoints, pointsLeft);
            //alpha = max(alpha, eval);
            if (eval > alpha) {
                alpha = eval;
                if (depth == 5) {
                    bestMove = legalMoves[legalMoves.size()-1];
                }
            }
            if (beta <= alpha) {
                break; //beta cut-off
            }
        }
        else
        {
            float eval = alpha_beta(_board, depth-1, alpha, beta, !maxPlayer, myPoints, otherPoints, pointsLeft);
//            beta = min(beta, );
            if (eval < beta) {
                beta = eval;
            }
            if (beta <= alpha) {
                break; //alpha cut-off
            }
        }
        while (moves > 0) {
            /// TODO: this needs to be changed when move ordering is implemented
            unmakeLastMove(_board, legalMoves[legalMoves.size()-moves]);
            moves--;
        }
        legalMoves.pop_back();
    }//end while loop of all legalMoves
    if (maxPlayer) {
        return alpha;
    }
    return beta;
}

void AlphaBetaAI::unmakeLastMove(int* _board, int move) {
    _board[move] = BOARD_EMPTY;
}

/**
 * Generates a vector of all legal moves
 * @param board true board (not the copy)
 * @param v vector reference to legalMoves
 */
 void AlphaBetaAI::generateLegalMoves(int *board, vector<int> &v)
 {
     for( int i=0; i<boardWidth*boardHeight; i++)
     {
         if(board[i] == BOARD_EMPTY) {
            v.push_back(i);
         }
     }
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

    /*int pointsRem = _board.getPointsRemaining();
    int nextMove = moves.back();
    _board.playBoardCoord(nextMove);

    if(pointsRem != _board.getPointsRemaining()) //if change in score, then box was completed and maxPlayer gets to go again
        return maxPlayer;

    return !maxPlayer;*/ //no change in score, so minPlayers turn
 /*
    _board[nextMove] = BOARD_LINE;
    if (squaresMade(board, nextMove) > 0)
    {

        //makeNextMove(_board, )
    }
*/
}

/*
/ **
 * Undoes the previous move on search board
 *  and deletes the move from legalMoves
 * @param _board copy board for search
 * @param v vector reference to legalMoves
 *
void unMakeMove(int *_board, vector<int> &v)
{
    int prevMove = v.back();
    b[prevMove] = BOARD_EMPTY;

    v.pop_back();
}
*/

/**
 * Calculates utility function
 *@param _board pointer to the current state of the board COPY used to create tree
 *@return eval the value of the function
 */
float AlphaBetaAI::evaluate(int* board, int myScore, int otherScore, bool myMove)
{
    int s3 = 0;
    int s2 = 0;
    int size = boardWidth*boardHeight;

    // first square starts at 1x1 = width+2
    for (int i = boardWidth+2; i < size; i+=2) {
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

    return 2*myScore-2*otherScore+(0.75f*s3-0.5*s2)*(myMove?1:-1);
}

/*
/ **
 * Checks how many squares are made by the line
 *@param board the game board
 *@param line the line that was placed
 *@return how many squares this line made (0-2)
 *
int AlphaBetaAI::squaresMade(int* board, int line) {
   int x = line%boardWidth;
   int y = line/boardWidth;
   bool vert = y%2;
   int squares = 0;
   if (vert) {
       // line is vertical, need to check square left and square right
       if (x > 1) { // not left-most, so there is a square left
           if (board[line-2] == BOARD_LINE && board[x-1+(y-1)*boardWidth] == BOARD_LINE && board[x-1+(y+1)*boardWidth] == BOARD_LINE) {
               squares++;
           }
       }
       if (x < boardWidth-1) { // not right-most so there is a square right
           if (board[line+2] == BOARD_LINE && board[x+1+(y-1)*boardWidth] == BOARD_LINE && board[x+1+(y+1)*boardWidth] == BOARD_LINE) {
               squares++;
           }
       }
   } else {
       // line is horizontal, need to check square up and square down
       if (y > 1) { // not top-most, so there is a square up
           if (board[line-boardWidth*2] == BOARD_LINE && board[(y-1)*boardWidth+x-1] == BOARD_LINE && board[(y-1)*boardWidth+x+1] == BOARD_LINE) {
               squares++;
           }
       }
       if (y < boardHeight-1) { // not bottom-most so there is a square down
           if (board[line+boardWidth*2] == BOARD_LINE && board[(y+1)*boardWidth+x-1] == BOARD_LINE && board[(y+1)*boardWidth+x+1] == BOARD_LINE) {
               squares++;
           }
       }
   }
   return squares;
}
*/





