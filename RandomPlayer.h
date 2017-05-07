#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H

#include <Player.h>
#include <Board.h>

class RandomPlayer : public Player
{
  public:
    RandomPlayer(Board* brd);
    ~RandomPlayer();

    void decideMove();

  private:
    Board* b;
};

#endif // RANDOMPLAYER_H
