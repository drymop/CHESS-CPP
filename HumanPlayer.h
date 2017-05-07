#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include <Player.h>
#include <BoardGUI.h>

class HumanPlayer : public Player
{
  public:
    HumanPlayer(BoardGUI* brdGUI, Board* brd, int playerNum, bool* quit);
    ~HumanPlayer();

    void decideMove();

  private:
    int player;
    BoardGUI* bGUI;
    Board* b;
    bool* quitGame;
};

#endif // HUMANPLAYER_H
