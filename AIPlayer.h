#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <Board.h>
#include <Player.h>


class AIPlayer : public Player
{
  public:
    AIPlayer(Board* brd, int difficulty);
    ~AIPlayer();

    void decideMove();

  private:
    Board* b;
    int lookAhead;
    int bestMove;
    int numNodes;

    static const int pieceValues[12];

    /**
     * Evaluate a move branch (with alpha beta prunning)
     */
    int negamax(int depth, int alpha, int beta, int color);

    /**
     * Static evaluation of the board. A positive score means white has advantage.
     * @return the score of the board (in white's perspective)
     */
    int heuristicEval();
};

#endif // AIPLAYER_H
