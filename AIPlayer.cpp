#include "AIPlayer.h"
#include <stdio.h>

AIPlayer::AIPlayer(Board* brd, int difficulty)
{
  b = brd;
  lookAhead = 5;
}

void AIPlayer::decideMove()
{
  bestMove = -1;
  numNodes = 0;
  int color = (b->getPlayer() == 0)? 1 : -1;
  negamax(lookAhead, -100000, 100000, color);
  b->makeMove(bestMove);
  printf("Looked through %i nodes\n", numNodes);

}

int AIPlayer::negamax(int depth, int alpha, int beta, int color)
{
  numNodes++;
  /*
   * If out of depth or terminal node
   */
  int numMoves = b->getNumMoves();
  if (depth == 0 || numMoves == 0)
  {
    return color * heuristicEval();
  }

  /*
   * Evaluate each branch
   */
  int bestVal = -100000;
  for (int m = 0; m < numMoves; m++)
  {
    //printf("Depth %i, move %i\n", depth, m);
    b->makeMove(m);
    int val = -negamax(depth-1, -beta, -alpha, -color);
    b->undoMove();
    if (val > bestVal)
    {
      bestVal = val;
      if (depth == lookAhead) bestMove = m;
    }
    if (val > alpha) alpha = val;
    if (alpha >= beta) break;
  }
  return bestVal;
}

const int AIPlayer::pieceValues[12] = {-900, 0, -500, -320, -330, -100,
                                        900, 0, 500, 320, 330, 100};

int AIPlayer::heuristicEval()
{
  if (b->getNumMoves() == 0)
  {
    int winner = b->getWinner();
    switch (winner)
    {
      case 0: return 100000;
      case 1: return -100000;
      case 2: return 0;
    }
  }
  int score = 0;
  int piece;
  for (int s = 0; s < 64; s++)
  {
    piece = b->getPiece(s);
    if (piece == -1) continue;
    score += pieceValues[piece];
  }
  return score;
}
