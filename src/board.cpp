#include "board.h"
#include <cstring>

/**
 * Creates a new dots and boxes game
 *@param w width
 *@param h height
 */
Board::Board(int w, int h) {
    width = w;
    height = h;

    boxes = new Box[width*height];

    board = new int[(width*2+1)*(height*2+1)];
    memset((void *)board, BOARD_EMPTY, sizeof(int)*(width*2+1)*(height*2+1));
    for (int i = 0; i < height*2+1; i+=2) {
        for (int j = 0; j < width*2+1; j+=2) {
            board[i*(width*2+1)+j] = BOARD_DOT;
            if (j != width*2 && i != height*2) {
                board[(i+1)*(width*2+1)+j+1] = BOARD_FREE_SQUARE;
            }
        }
    }

    current_player = 0;
    p1score = 0;
    p2score = 0;

    pointsRemaining = width*height;
}

/**
 * Frees memory
 */
Board::~Board() {
    if (boxes != NULL) {
        delete[] boxes;
    }
    boxes = NULL;
    if (board != NULL) {
        delete[] board;
    }
    board = NULL;
}

/**
 * Returns the board visualization
 *@return board
 */
int* Board::getBoard() {
    return board;
}

/**
 * Returns the width of the board visualization
 *@return width*2+1
 */
int Board::getBoardWidth() {
    return width*2+1;
}

/**
 * Returns the height of the board visualization
 *@return height*2+1
 */
int Board::getBoardHeight() {
    return height*2+1;
}

/**
 * Plays a line based on board coordinate
 *@param bcoord board coord
 *@return true if this was a valid move, otherwise false
 */
bool Board::playBoardCoord(int bcoord) {
    int bx = bcoord%getBoardWidth();
    int by = bcoord/getBoardWidth();

    int index = bx+by*getBoardWidth();
    if (board[index]) {
        return false;
    }
    board[index] = BOARD_LINE;

    bool scored = false;
    // check for filled boxes
    if (bx > 0 && board[index-1] == BOARD_FREE_SQUARE) { // to left
        if (boxes[(bx-1)/2+by/2*width].fillRight()) {
            board[index-1] = BOARD_P1_SQUARE+current_player;
            if (current_player) {
                p2score++;
            } else {
                p1score++;
            }
            pointsRemaining--;
            scored = true;
        }
    }
    if (bx != getBoardWidth()-1 && board[index+1] == BOARD_FREE_SQUARE) { // to right
        if (boxes[(bx+1)/2+by/2*width].fillLeft()) {
            board[index+1] = BOARD_P1_SQUARE+current_player;
            if (current_player) {
                p2score++;
            } else {
                p1score++;
            }
            pointsRemaining--;
            scored = true;
        }
    }
    if (by > 0 && board[index-getBoardWidth()] == BOARD_FREE_SQUARE) { // to up
        if (boxes[bx/2+(by-1)/2*width].fillBottom()) {
            board[index-getBoardWidth()] = BOARD_P1_SQUARE+current_player;
            if (current_player) {
                p2score++;
            } else {
                p1score++;
            }
            pointsRemaining--;
            scored = true;
        }
    }
    if (by != getBoardHeight()-1 && board[index+getBoardWidth()] == BOARD_FREE_SQUARE) { // to bottom
        if (boxes[bx/2+(by+1)/2*width].fillTop()) {
            board[index+getBoardWidth()] = BOARD_P1_SQUARE+current_player;
            if (current_player) {
                p2score++;
            } else {
                p1score++;
            }
            pointsRemaining--;
            scored = true;
        }
    }

    if (!scored) {
        current_player = (~current_player)&1;
    }

    return true;
}

/**
 * Returns the width of the board
 *@return width
 */
int Board::getWidth() {
    return width;
}

/**
 * Returns the height of the board
 *@return height
 */
int Board::getHeight() {
    return height;
}

/**
 * Returns a player's score
 *@param player which player's score (0/1)
 *@return the player's score
 */
int Board::getScore(int player) {
    if (player) {
        return p2score;
    }
    return p1score;
}

/**
 * Returns how many boxes haven't been scored yet
 *@return pointsRemaining
 */
int Board::getPointsRemaining() {
    return pointsRemaining;
}

/**
 * Returns who's turn it is
 *@return 0/1
 */
int Board::getCurrentPlayer() {
    return current_player;
}
