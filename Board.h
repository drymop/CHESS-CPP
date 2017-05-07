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
   * For debugging purpose. Print to console the list of moves
   */
  void printMoveList();
  /**
   * For debugging purpose. Print to console the history of moves
   */
  void printHistory();


  /**
   * Starting board
   */
  void initBoard();

  /**
   * Get current player
   * @return 0 for white, 1 for black
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
   * Return the number of moves available
   */
  int getNumMoves();

  /**
   * Get the list of available moves
   */
  std::vector<int> getMoveList();

  /**
   * Execute a move at once.
   * Should be called when all parameters of the move (starting square, ending square, promotion if necessary) have already been decided.
   * @param moveIndex: the move number according to move list (starting from 0)
   */
  void makeMove(int moveIndex);

  /**
   * Execute a move in subsequent parts. To make a move, call this function twice for starting square and ending square.
   * If there is a promotion event, also need to call the promote function to set the promoted piece.
   * @param square: a square number between 0 and 63
   */
  void chooseSquare(int square);

  /**
   * Check if there is a promotion event
   * @return true if there is promotion
   */
  bool hasPromotion();

  /**
   * Choose which piece to promote to
   * @param piece: the piece type
   */
  void promote(int piece);

  /**
   * Undo the previous move
   */
  void undoMove();

private:
  /**
   * Contains the pieces on chessboard
   * Values of pieces are indicated in pieceTypes enum (in pieceType.h)
   * The first index is the row number, and the second index is the column (from 0 -> 7)
   */
  int board[8][8];

  /**
   * The current player. Can be 0 for white or 1 for black (false or true when casted to bool)
   */
  int player;

  /**
   * List of possible moves
   * Each moves use 3 ints: starting square, ending square (0 -> 63), and the type of move
   */
  std::vector<int> moveList;

  /**
   * The history of the game.
   * Each half-turn uses 4 int: starting square (0->63), ending square(0->63), captured piece (-1 if no piece captured), and move type.
   * Used for undoing.
   */
  std::vector<int> history;

  /**
   * The square chosen by current player.
   * Used to display chosen piece on screen.
   * -1 if no chosen square, else 0 to 63.
   */
  int chosenSquare;

  /**
   * Execute a move from square1 to square2
   * If the move is a promotion, set the promotion flag and return without promoting the pawn
   * @param square1, square2: the starting and ending square (from 0 to 63)
   */
  void makeMove(int square1, int square2);

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
   * Keep track of when the kings and rooks first move. Used for castling.
   * Order: king, left rook, right rook. The even flags are for white, and the odds are for black.
   * 0 if the piece has not moved, otherwise the size of the history when the piece first moved.
   */
  int castlingFirstMove[6];

  /**
   * The square that are having a promotion event (0 -> 63).
   * -1 if there is no promotion event.
   */
  int promotionSquare;

  /**
   * 8 basic directions.
   * The first 4 are vertical and horizontal. The last 4 are diagonal.
   */
  static const int dir[8][2];
  /**
   * 8 knight's directions (L shape)
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
    MOVE_PAWN_EN_PASSANT,
    MOVE_PROMOTION_QUEEN,
    MOVE_PROMOTION_ROOK,
    MOVE_PROMOTION_KNIGHT,
    MOVE_PROMOTION_BISHOP
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
