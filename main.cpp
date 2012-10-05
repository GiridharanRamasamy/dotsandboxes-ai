#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include "board.h"
#include "randomai.h"
#include "montecarloai.h"
#include "alphabetaai.h"

#include <conio.h>

/**
 * Returns the AI from the string
 *@param ai the ai to load
 *@param bw board width
 *@param bh board height
 *@param player which player this is for
 *@return the loaded ai
 */
AI* loadAI(std::string ai, int bw, int bh, int player) {
    if (ai == "random") {
        return new RandomAI(bw, bh);
    } else if (ai == "montecarlo") {
        return new MonteCarloAI(bw, bh);
    } else if (ai == "alphabeta") {
        return new AlphaBetaAI(bw, bh);
    }

    printf("Error: invalid AI for player %d\n", player);
    printf("Format is wrong.\n\n\t\tdotsandboxes w h p1 p2");
    printf("\n\n\tw - the width of the board");
    printf("\n\th - the height of the board");
    printf("\n\tp1, p2 - players. Available players:");
    printf("\n\t\trandom - the random ai");
    printf("\n\t\tmontecarlo - the Monte Carlo AI");
    printf("\n\t\talphabeta - the Alpha Beta AI");
    printf("\n\tvisual - (0 or 1) whether or not to print the visualization");
    printf("\n\tgames - how many games to play");
    printf("\n\n");
    return NULL;
}

/**
 * main entry point
 *@param argc number of args
 *@param argv the args
 */
int main(int argc, char* argv[])
{
    if (argc != 7) {
        printf("Format is wrong.\n\n\t\t%s w h p1 p2 visual games", argv[0]);
        printf("\n\n\tw - the width of the board");
        printf("\n\th - the height of the board");
        printf("\n\tp1, p2 - players. Available players:");
        printf("\n\t\trandom - the random ai");
        printf("\n\t\tmontecarlo - the Monte Carlo AI");
        printf("\n\t\talphabeta - the Alpha Beta AI");
        printf("\n\tvisual - (0 or 1) whether or not to print the visualization");
        printf("\n\tgames - how many games to play");
        printf("\n\n");
        return -1;
    }
    int w = atoi(argv[1]);
    int h = atoi(argv[2]);
    std::string p1 = argv[3];
    std::string p2 = argv[4];
    bool visual = atoi(argv[5]);
    int games = atoi(argv[6]);
    if (w <= 0) {
        printf("Error: width must be positive\n");
        return -1;
    } else if (h <= 0) {
        printf("Error: height must be positive\n");
        return -1;
    }

    int p1wins = 0;
    int p2wins = 0;

    for (int k = 0; k < games; k++) {
        printf("Playing game %d\n", k+1);

        Board* game = new Board(w, h);

        AI* agent1 = loadAI(p1, game->getBoardWidth(), game->getBoardHeight(), 1);
        AI* agent2 = loadAI(p2, game->getBoardWidth(), game->getBoardHeight(), 2);

        if (agent1 == NULL || agent2 == NULL) {
            return -1;
        }

        int* board = new int[game->getBoardWidth()*game->getBoardHeight()];

        while (game->getPointsRemaining() > 0) {
            if (visual) {
                for (int i = 0; i < game->getBoardHeight(); i++) {
                    for (int j = 0; j < game->getBoardWidth(); j++) {
                        printf("%d", game->getBoard()[i*game->getBoardWidth()+j]);
                    }
                    printf("\n");
                }
                printf("%d >< %d     | %d --> %d next", game->getScore(0), game->getScore(1), game->getPointsRemaining(), game->getCurrentPlayer()+1);
                printf("\n\n\n");
            }

            memcpy((void *)board, (void *)game->getBoard(), sizeof(int)*game->getBoardWidth()*game->getBoardHeight());
            if (game->getCurrentPlayer()) {
                if (!game->playBoardCoord(agent2->makeMove(board, game->getScore(1), game->getScore(0), game->getPointsRemaining()))) {
                    printf("Player 2 loses because he made an illegal move\n");
                    return -1;
                }
            } else {
                if (!game->playBoardCoord(agent1->makeMove(board, game->getScore(0), game->getScore(1), game->getPointsRemaining()))) {
                    printf("Player 1 loses because he made an illegal move\n");
                    return -1;
                }
            }
        }

        delete[] board;

        if (visual) {
            for (int i = 0; i < game->getBoardHeight(); i++) {
                for (int j = 0; j < game->getBoardWidth(); j++) {
                    printf("%d", game->getBoard()[i*game->getBoardWidth()+j]);
                }
                printf("\n");
            }
            printf("%d >< %d     | %d", game->getScore(0), game->getScore(1), game->getPointsRemaining());
            printf("\n\n\n");
        }

        if (game->getScore(0) > game->getScore(1)) {
            p1wins++;
        } else if (game->getScore(1) > game->getScore(0)) {
            p2wins++;
        }

        delete game;
        delete agent1;
        delete agent2;
    }

    printf("----------\nResults:\n\nPlayer 1 [%s]: %d wins\nPlayer 2 [%s]: %d wins\nDraws: %d\n\n", p1.c_str(), p1wins, p2.c_str(), p2wins, games-p1wins-p2wins);

    return 0;
}
