/***********************************************************************//**
 * Interface for all player classes.
 ***************************************************************************/


#ifndef PLAYER_H
#define PLAYER_H


class Player
{
public:
  Player();
  virtual ~Player();

  /**
   * @return true if player is human, false if player is COM.
   */
  virtual bool isHuman() = 0;

  /**
   * Decide which move to make.
   * @return If the player is human, return input from GUI.
   * @return If the player is COM, return the move number among all available moves.
   */
  virtual int decideMove() = 0;
};

#endif // PLAYER_H
