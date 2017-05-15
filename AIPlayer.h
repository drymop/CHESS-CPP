/***********************************************************************//**
 * A chess AI. Decide which move to make on a board by.
 * Uses negamax with alpha-beta pruning.
 ***************************************************************************/

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Board.h"
#include "BoardGUI.h"
#include "Player.h"


class AIPlayer : public Player {
  public:
    /**
     * @param brd: the board to consider.
     * @param brdgui: the board GUI used to display the board.
     * @param difficulty: the number of moves (half-turn) the AI can look ahead.
     */
    AIPlayer(Board* brd, BoardGUI* brdgui, int difficulty);
    ~AIPlayer() {}

    bool isHuman();
    int decideMove();

  private:
    Board* b; /**< The board that the AI is playing on */
    BoardGUI* bgui; /**< The GUI used to display the board. Need this to keep GUI responsive while AI is thinking. */
    int maxDepth; /**< Number of half-moves AI can look ahead */

    /***************************************************************************
     * Values used in board evaluation
     ***************************************************************************/

    static const int MATE_VALUE; /**< The evaluation of a won board */
    /**
     * The number of best moves to be searched first.
     * Used in reordering moves.
     */
    static const int NUM_BEST_MOVES;
    /**
     * The values of each piece type.
     * The order of types: queen, king, rook, knight, bishop, pawn (indicated in pieceTypes enum).
     */
    static const int pieceValues[Board::NUM_PIECE_TYPES];
    /**
     * The positional values of pieces for black.
     * To get the positional values for white, flips the array upside down
     * Indexes: piece type, row, column.
     * The 6th piece type is king during end game.
     */
    static const int positionValues[Board::NUM_PIECE_TYPES+1][Board::NUM_SQUARES];

    /**
     * AI switches to end game strategy when the total material is smaller than this value
     */
    static const int LATE_GAME_MATERIAL;

    /***************************************************************************
     * Search and Evaluation methods
     ***************************************************************************/

    /**
     * Search a move tree and return its value. Has alpha-beta pruning with move ordering.
     * @param color: 1 if the next player to move is white,
     * and -1 if the next player to move is black.
     */
    int negamax(int depth, int alpha, int beta, int color);

    /**
     * Reorder the moves so that the best moves after 1 ply are searched first.
     * @param moveOrder: an empty array
     * @param numMoves: the size of moveOrder array
     * @param color: 1 if the next player to move is black, else -1.
     */
    void reorderMoves(int* moveOrder, int numMoves, int color);

    /**
     * Static evaluation of the board. A positive score means white has an advantage.
     * @param depth: the depth of the current board. 0 is the deepest board.
     * @return the score of the board (in white's perspective)
     */
    int heuristicEval();

    /***************************************************************************
     * Debug
     ***************************************************************************/

    int numNodes; /**< Number of moves made during search. Used to check pruning. */
    Board bSave; /**< Saved board for debugging */
    /**
     * Save the current board's state to bSave
     */
    void saveBoard();
    /**
     * Compare the current board to the saved board.
     * If the board is different after thinking, there is something wrong with either the AI or the board
     */
    bool isBoardDifferent();
};

#endif // AIPLAYER_H
