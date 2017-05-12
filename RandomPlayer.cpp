#include "RandomPlayer.h"
#include <stdlib.h>
#include <ctime>

RandomPlayer::RandomPlayer(Board* brd)
{
  srand(time(0));
  b = brd;
}

bool RandomPlayer::isHuman()
{
  return false;
}

int RandomPlayer::decideMove()
{
  int numMoves = b->getNumMoves();

  int randomMove = rand() % numMoves;
  return randomMove;
}
