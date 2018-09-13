#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"
#include "BoardGUI.h"

class HumanPlayer : public Player
{
  public:
    HumanPlayer(BoardGUI* brdGUI, Board* brd);
    ~HumanPlayer() {};

    bool isHuman();
    int decideMove();

  private:
    BoardGUI* bGUI;
    Board* b;
    bool* quitGame;
};

#endif // HUMANPLAYER_H
