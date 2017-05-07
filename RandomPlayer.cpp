#include "RandomPlayer.h"
#include <stdlib.h>
#include <ctime>

RandomPlayer::RandomPlayer(Board* brd)
{
  srand(time(0));
  b = brd;
}

void RandomPlayer::decideMove()
{
  int numMoves = b->getNumMoves();

  int move = rand() % numMoves;
  b->makeMove(move);
}
