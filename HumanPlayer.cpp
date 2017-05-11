#include "HumanPlayer.h"
#include <stdio.h>

HumanPlayer::HumanPlayer(BoardGUI* brdGUI, Board* brd)
{
  bGUI = brdGUI;
  b = brd;
}

void HumanPlayer::decideMove()
{

  int input = 0;
  while (input == 0)
  {
    input = bGUI->getInput();
    if (GUI::quit) return;
  }

  if (input > 0) //normal move
  {
    if (b->hasPromotion())
    {
      //flash promotion pawn
    }
    else
    {
      b->chooseSquare(input - 1); //square start from 0, so need to substract 1
    }
  }
  else //side bar move
  {
    printf("not normal move! %i \n", input);
    switch (input)
    {
      case -1: // undo
        b->undoMove();
        b->undoMove();
        break;
      case -2: // home


    }
  }

}
