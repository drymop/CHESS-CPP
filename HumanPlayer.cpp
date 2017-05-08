#include "HumanPlayer.h"
#include <stdio.h>

HumanPlayer::HumanPlayer(BoardGUI* brdGUI, Board* brd)
{
  bGUI = brdGUI;
  b = brd;
}

void HumanPlayer::decideMove()
{
  // TO DO: add promotion gui
  int move = 0;
  while (move == 0)
  {
    move = bGUI->getInput();
    if (GUI::quit) return;
  }
  if (b->hasPromotion())
  {
    b->promote(move);
  }
  else
  {
    b->chooseSquare(move-1);
  }
}
