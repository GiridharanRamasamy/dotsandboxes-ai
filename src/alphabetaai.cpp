#include "alphabetaai.h"
#include <limits>

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

    int depth = 5; //needs to be a time limit
    int inf = numeric_limits<int>::max();

    val = alpha_beta(depth, -inf, inf); //calls the alpha_beta search process


    return coordToInt(x, y);
}

/**
 * Alph-Beta Pruning
 *@param depth how deep to search the tree
 *@param alpha the minimax value for alpha
 *@param beta the minimax value for beta
 *@return x y coordinates for move
 */
int alpha_beta(int* board, int depth, int alpha, int beta)
{
    //iterate through and copy values with nested for loops
    _board = new int[boardWidth*boardHeight];
 //   memset((void *)_board, BOARD_EMPTY, sizeof(int)*(width*2+1)*(height*2+1));

    _board board; //copy of board to create search tree with
    legalMoves
    if (depth ==0)
    {
        return evaluate();
    }
    generateLegalMoves();
    while (movesLeft())
    {
        makeNextMove();
        val = -alpha_beta(depth-1,-beta,-alpha);
        unMakeMove();
        if(val >= beta)
            return beta;
        if(val > alpha)
            alpha = val;
    }
    return alpha;
}

/**
 * Calculates utility function
 *@param _board pointer to the current state of the board COPY used to create tree
 *@return eval the value of the function
 */
int evaluate(int* _board)
{
    int eval = 0;


    return eval;
}








