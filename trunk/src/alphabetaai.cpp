#include "alphabetaai.h"
#include <limits>
#include <cmath>

/**
 * Inits a new AI
 *@param bw board width
 *@param bh board height
 */
AlphaBetaAI::AlphaBetaAI(int bw, int bh): AI(bw, bh) {

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
    int x = 0;
    int y = 0;

    int depth = 5; //needs to be a time limit?..Iterative Deepening
    int inf = numeric_limits<int>::max();

    //Create copy of the board for use in search tree
    _board = new int[boardWidth*boardHeight];
    for (int i=0; i < boardWidth*boardHeight; i++)
    {
        _board[i] = board[i];
    }

    int maxPlayer = 1;
    int val = alpha_beta( _board, depth, -inf, inf, maxPlayer); //calls the alpha_beta search process

    return val;
}

/**
 * Alph-Beta Pruning
 *@param depth how deep to search the tree
 *@param alpha the minimax value for alpha
 *@param beta the minimax value for beta
 *@return x y coordinates for move
 */
int AlphaBetaAI::alpha_beta(int* _board, int depth, int alpha, int beta, int maxPlayer)
{
    if (depth == 0) // or is a terminal node?
    {
        return evaluate(_board);
    }

    vector<int> legalMoves;
    generateLegalMoves(_board, legalMoves);  //vector of all possible legal moves to make

    while (!legalMoves.empty()) //for each child node of max and min player
    {
        maxPlayer = makeNextMove(_board, legalMoves, maxPlayer); //changes betweens turn unless box completed
        if (maxPlayer)
        {
            alpha = max(alpha, alpha_beta(_board, depth-1,alpha, beta, maxPlayer));
            if (beta <= alpha)
                break; //beta cut-off

            return alpha;
        }
        else
        {
            beta = min(beta, alpha_beta(_board, depth-1, alpha, beta, maxPlayer));
            if (beta <= alpha)
                break; //alpha cut-off

            return beta;
        }
    }//end while loop of all legalMoves
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
         if(board[i] == 0)
            v.push_back(i);
     }
 }

/**
 * Makes the next move in search
 * @param _board copy board for search
 * @param moves vector reference to legalMoves
 */
int AlphaBetaAI::makeNextMove(int *_board, vector<int> &moves, int maxPlayer)
{
    int pointsRem = _board.getPointsRemaining();
    int nextMove = moves.back();
    _board.playBoardCoord(nextMove);

    if(pointsRem != _board.getPointsRemaining()) //if change in score, then box was completed and maxPlayer gets to go again
        return maxPlayer;

    return !maxPlayer; //no change in score, so minPlayers turn
 /*
    _board[nextMove] = BOARD_LINE;
    if (squaresMade(board, nextMove) > 0)
    {

        //makeNextMove(_board, )
    }
*/
}

/*
/**
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
int AlphaBetaAI::evaluate(int* _board)
{
    int scoreA = 0;
    int scoreB = 0;
    int line3 = 0;
    int line2 = 0;

    for (int i=1; i<boardWidth*boardHeight; i++)
    {
        if(_board[i] == BOARD_P1_SQUARE)
            scoreA++;
        else if (_board[i] == BOARD_P2_SQUARE)
            scoreB++;

    }


    int eval = 2*scoreA - 2*scoreB + .75*line3 - .5*line2;
    return eval;
}

/*
/**
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





