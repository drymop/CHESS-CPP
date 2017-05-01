#ifndef BOARD_H
#define BOARD_H


class Board
{
public:
  Board();
  ~Board();

  /**
   * For debugging purpose. Print to console all hitboxes.
   */
  void printHitbox();
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
   * Contains the ID of pieces on chessboard
   * Index: row and column
   * Value: ID of piece
   */
  int board[8][8];
  /**
   * Contains the type of pieces on chessboard
   * Index: ID of piece, index 0 is empty piece
   * Value: Type of piece (according to enum pieceTypes)
   */
  int pieces[33];

  /**
   * Contains all potential moves of pieces
   * Index: ID of piece, row, collumn
   * Value: type of move
   */
  int hitboxes[33][8][8];

  /**
   * 1 is the first player, 2 is the second player
   */
  int player;

  /**
   * The square chosen by player
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
   * Types of move in hitbox
   */
  enum moveTypes {
    MOVE_ILLEGAL,
    MOVE_NORMAL,
    MOVE_PROTECT,
    MOVE_PAWN_DOUBLE_JUMP,
    MOVE_PAWN_PROMOTION,
    MOVE_PAWN_ENPASSANT,
    MOVE_CASTLING,
    MOVE_BEHIND_OBSTACLE
  };
  /**
   * Position of 2 kings, value from 0 to 63
   */
  int kingSquares[2];
  /**
   * ID of the pieces checking king. There can be at most 2 pieces. 0 if no piece is checking.
   */
  int checkingPieces[2];
  /**
   * The cols of last double jump pawns of opponent, value from 0 to 63, -1 if last move is not a double jump
   * Use for enpassant checking
   */
   int enPassantCols[2];

  /**
   * 8 basic direction
   */
  static const int dir[8][2];
  /**
   * 8 knight's direction
   */
  static const int dirKnight[8][2];

  /**
   * Fill all hitboxes with moves. Call after making a move.
   */
  public: void updateAll();

  /**
   * Fill a hitbox (disregard king checking)
   */
  void fillHitbox(int square);
  void fillHitboxPawn(int square, int pID);
  void fillHitboxKnight(int square, int pID);
  void fillHitboxKing(int square, int pID);
  void fillHitboxRayPieces(int square, int pID);

  /**
   * Fill a hitbox of the current player's king
   * Call after every other hitboxes have been filled
   */
  void fillHitboxCurrentKing();

  /**
   * Compare hitboxes to find checks and update hitboxes
   */
  void findCheck();

  /**
   * Call when there are 2 pieces checking king
   * All player's hitboxes become illegal
   */
  void updateDoubleCheck();
  /**
   *
   */
  void updateSingleCheck();

  /**
   * Check if 3 squares are in a line, in same order
   */
  bool isInRay(int x1, int y1, int x2, int y2, int x3, int y3);

};

#endif // BOARD_H
