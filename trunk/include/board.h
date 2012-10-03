#ifndef BOARD_H
#define BOARD_H

#include "box.h"

#define BOARD_EMPTY         0
#define BOARD_LINE          1
#define BOARD_DOT           2
#define BOARD_FREE_SQUARE   3
#define BOARD_P1_SQUARE     4
#define BOARD_P2_SQUARE     5

/**
 * The dots and boxes game
 */
class Board
{
    public:
        /**
         * Creates a new dots and boxes game
         *@param w width
         *@param h height
         */
        Board(int w, int h);

        /**
         * Frees memory
         */
        ~Board();

        /**
         * Returns the board visualization
         *@return board
         */
        int* getBoard();

        /**
         * Returns the width of the board visualization
         *@return width*2+1
         */
        int getBoardWidth();

        /**
         * Returns the height of the board visualization
         *@return height*2+1
         */
        int getBoardHeight();

        /**
         * Plays a line based on board coordinate
         *@param bcoord board coord
         *@return true if this was a valid move, otherwise false
         */
        bool playBoardCoord(int bcoord);

        /**
         * Returns the width of the board
         *@return width
         */
        int getWidth();

        /**
         * Returns the height of the board
         *@return height
         */
        int getHeight();

        /**
         * Returns a player's score
         *@param player which player's score (0/1)
         *@return the player's score
         */
        int getScore(int player);

        /**
         * Returns how many boxes haven't been scored yet
         *@return pointsRemaining
         */
        int getPointsRemaining();

        /**
         * Returns who's turn it is
         *@return 0/1
         */
        int getCurrentPlayer();
    private:
        int width; /**< width of the board (in dots) */
        int height; /**< height of the board (in dots) */

        Box* boxes; /**< the boxes on the board */

        int* board; /**< a visualization of the board
                         0 - Empty
                         1 - Line
                         2 - Dot
                         3 - UnOwned Square
                         4 - Player1 Square
                         5 - Player2 Square
                     */

        int current_player; /**< whose turn it is */
        int p1score; /**< player 1 score */
        int p2score; /**< player 2 score */

        int pointsRemaining; /**< how many boxes are unowned */
};

#endif // DOTSANDBOXES_H
