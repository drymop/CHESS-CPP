#ifndef PLAYER_H
#define PLAYER_H


class Player
{
public:
  Player();
  ~Player();

  /**
   * Human makes move through several stage (choose first square, choose second square, choose promotion (if applicable).
   * So human player need to be distinguished from computer player.
   * @return true if is human
   */
  virtual bool isHuman() = 0;

  /**
   * Decide which move to go
   * Must return almost immediately if user quits
   * @return the move number (according to moveList vector of current board).
   * In case of human player, return the square clicked.
   */
  virtual int decideMove() = 0;
};

#endif // PLAYER_H
