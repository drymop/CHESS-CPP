#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <Board.h>
#include <BoardGUI.h>
#include <Player.h>


class AIPlayer : public Player
{
  public:
    AIPlayer(Board* brd, BoardGUI* brdgui, int difficulty);
    ~AIPlayer();

    void decideMove();

  private:
    Board* b;
    BoardGUI* bgui;
    Board bSave;
    int lookAhead;
    int bestMove;
    int numNodes;

    static const int MATE_VALUE;
    static const int pieceValues[6];
    static const int positionValues[6][8][8];

    /**
     * Evaluate a move branch (with alpha beta prunning)
     */
    int negamax(int depth, int alpha, int beta, int color);

    /**
     * Static evaluation of the board. A positive score means white has advantage.
     * @return the score of the board (in white's perspective)
     */
    int heuristicEval(int depth);


    /**
     * For debugging
     * Find out if board changed after make move
     */
    void saveBoard();
    bool isBoardDifferent();
};

#endif // AIPLAYER_H
