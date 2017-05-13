/***********************************************************************//**
 * A chess AI. Decide which move to make on a board by looking ahead some plies.
 * Uses negamax with alpha-beta pruning.
 ***************************************************************************/

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Board.h"
#include "BoardGUI.h"
#include "Player.h"


class AIPlayer : public Player
{
  public:
    /**
     * @param brd: the board to consider.
     * @param brdgui: the board GUI used to display the board (to keep gui responsive while calculating move).
     * @param difficulty: the number of half-move the AI can look ahead.
     */
    AIPlayer(Board* brd, BoardGUI* brdgui, int difficulty);

    ~AIPlayer();

    bool isHuman();
    int decideMove();

  private:
    Board* b;
    BoardGUI* bgui;
    Board bSave; /**< Saved board for debugging */
    int maxDepth; /**< Number of half-moves AI can search */

    int numNodes; /**< Number of moves made during search. For debugging purposes. */

    /***************************************************************************
     * Values used in static board evaluation
     ***************************************************************************/

    static const int MATE_VALUE; /**< The evaluation of a won board */
    /**
     * The values of each piece type.
     * The order of types: queen, king, rook, knight, bishop, pawn.
     */
    static const int pieceValues[6];
    /**
     * The positional values of pieces.
     * Indexes: piece type, row, column
     * Piece type 6th is king end game
     */
    static const int positionValues[7][8][8];

    /***************************************************************************
     * Search and Evaluation methods
     ***************************************************************************/
    /**
     * Search a move tree and return its value (with alpha beta prunning)
     */
    int negamax(int depth, int alpha, int beta, int color);

    /**
     * Reorder the moves so that the 5 best moves is placed first.
     */
    void reorderMoves(int* moveOrder, int numMoves, int color);

    /**
     * Static evaluation of the board. A positive score means white has advantage.
     * @param depth: the depth of the current board. 0 is the deepest board.
     * @return the score of the board (in white's perspective)
     */
    int heuristicEval();

    /***************************************************************************
     * Debug
     ***************************************************************************/
    /**
     * Save the current board's state
     */
    void saveBoard();
    /**
     * Compare the current board to the saved board, and print out the difference.
     */
    bool isBoardDifferent();
};

#endif // AIPLAYER_H
