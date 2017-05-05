#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board
{
public:
  Board();
  ~Board();

  /**
   * For debugging purpose. Print to console board array
   */
  void printBoard();

  /**
   * Starting board
   */
  void initBoard();

  /**
   * Get current player
   * @return 0 if game is ended, 1 if player 1, 2 if player 2
   */
  int getPlayer();

  /**
   * Get the piece in a square
   * @param square: the square number between 0 and 63
   * @return type of the piece in that square (according to enum pieceTypes).
   * -1 if empty, +12 if square is chosen. +25 if square is potential move
   */
  int getPiece(int square);

  /**
   * Call when a player chooses a square (to make a move, call this function twice)
   * @param square: a square number between 0 and 63
   */
  void chooseSquare(int square);

private:
  /**
   * Contains the pieces on chessboard
   * Values of pieces are indicated in pieceTypes enum (in pieceType.h)
   * The first index is the row number, and the second index is the column (from 0 -> 7)
   */
  int board[8][8];

  /**
   * The current player. Can be 1 or 2 (for white or black)
   */
  int player;

  /**
   * List of possible moves
   * Each moves use 3 ints: starting square, ending square (0 -> 63), and the type of move
   */
  std::vector<int> moveList;

  /**
   * The square chosen by current player
   * Used to display chosen piece on screen
   * -1 if no chosen square, else 0 to 63
   */
  int chosenSquare;

   /**
   * move a piece from square (x1, y1) to (x2, y2)
   * @param x1, y1 row and column of the starting square (from 0 to 7)
   * @param x2, y2 row and column of the destination square (from 0 to 7)
   */
  void makeMove(int x1,int y1,int x2,int y2);

  /**
   * Position of white and black king, value from 0 to 63
   */
  int kingSquares[2];
  /**
   * Squares of the pieces that are checking king. There can be at most 2 pieces. -1 if no piece is checking.
   */
  int checkingPieces[2];
  /**
   * Squares of the pinned and pinning pieces alternately
   * Even index: pinned pieces, odd index: pinning pieces
   */
  std::vector<int> pinPieces;
  /**
   * The cols of last double jump pawns of opponent, value from 0 to 63, -1 if last move is not a double jump
   * Use for enpassant checking
   */
   int enPassantCols[2];

   /**
    * Order: king, left rook, right rook. The even flags are for white, and the odds black
    * True if the piece has not moved, false otherwise
    */
   bool castlingFlags[6];

  /**
   * 8 basic directions
   */
  static const int dir[8][2];
  /**
   * 8 knight's directions
   */
  static const int dirKnight[8][2];
  /**
   * Types of move
   */
  enum moveTypes
  {
    MOVE_NORMAL,
    MOVE_CASTLING,
    MOVE_PAWN_DOUBLE_JUMP,
    MOVE_PAWN_PROMOTION,
    MOVE_PAWN_EN_PASSANT
  };

  /**
   * Update the list of available move (stored in moveList vector).
   * Should be called after making a move.
   */
  void updateMoveList();

  /**
   * Check if king is checked by opponent, and if any piece is pinned.
   * The results are saved in checkingPieces and pinPieces array.
   * Should be called before updating individual piece's moves.
   */
  void findPinAndCheck();

  /**
   * Add all available king moves (including castling) to moveList vector.
   * @param r, c: the row and column of the king (0 -> 7).
   */
  void updateKingMoves(int r, int c);

  /**
   * Add all available moves of a ray piece to moveList vector.
   * @param r, c: the row and column of the ray piece (0 -> 7).
   */
  void updateRayMoves(int r, int c);

  /**
   * Add all available moves of a knight to moveList vector.
   * @param r, c: the row and column of the knight (0 -> 7).
   */
  void updateKnightMoves(int r, int c);

  /**
   * Add all available moves of a pawn to moveList vector.
   * @param r, c: the row and column of the pawn (0 -> 7).
   */
  void updatePawnMoves(int r, int c);

  /**
   * Check if a square is controlled by the opponent.
   * @param r, c: the row and column of the square (0 -> 7).
   * @return true if square is controlled.
   */
  bool isSquareControlled(int r, int c);

  /**
   * Check if 3 squares are in a line, in the given order.
   * Use to check for legal move when king is checked, or a piece is pinned.
   * @param x1 y1 x2 y2 x3 y3: the rows and columns of 3 squares.
   * @return true if square 2 is between square 1 and square 3 in a line, or if square 1 and 2 are the same square.
   */
  bool isInRay(int x1, int y1, int x2, int y2, int x3, int y3);

};

#endif // BOARD_H
