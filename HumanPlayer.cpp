#include "HumanPlayer.h"
#include <stdio.h>

HumanPlayer::HumanPlayer(BoardGUI* brdGUI, Board* brd, int playerNum, bool* quit)
{
  bGUI = brdGUI;
  b = brd;
  player = playerNum;
  quitGame = quit;
}

void HumanPlayer::decideMove()
{
  int move = 0;
  while (move == 0)
  {
    move = bGUI->getMove();
  }
  if (move == -1)
  {
    *quitGame = true; //quit game
    return;
  }
  else
  {
    if (b->hasPromotion())
    {
      b->promote(move);
    }
    else
    {
      b->chooseSquare(move-1);
    }
  }
}
