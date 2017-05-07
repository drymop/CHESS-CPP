#ifndef PLAYER_H
#define PLAYER_H


class Player
{
public:
  Player();
  ~Player();

  /**
   * Decide which move to go, and change the board
   */
  virtual void decideMove() = 0;
};

#endif // PLAYER_H
