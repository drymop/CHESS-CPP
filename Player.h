#ifndef PLAYER_H
#define PLAYER_H


class Player
{
public:
  Player();
  ~Player();

  /**
   * Decide which move to go, and make the move
   * Must return almost immediately if user quits
   */
  virtual void decideMove() = 0;
};

#endif // PLAYER_H
