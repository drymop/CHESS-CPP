/***********************************************************************//**
 * Contains the basic board logic, including:
 * 1) Legal moves (with en passant, castling and promotion);
 * 2) Making moves and undoing moves;
 * 3) End game conditions (check mate, stalemate).
 * Does NOT include:
 * 1) Draw by 50 moves rule (50 consecutive moves without pawn movement or capture);
 * 2) Draw by insufficient material;
 * 3) Draw by 3-fold repetitions.
 ***************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board
{
public:
  Board() {};

  /**
   * Refresh the board to standard starting position.
   */
  void initBoard();

  /***************************************************************************
   *                       Constants used in Board
   ***************************************************************************/

  static const int ROWS = 8; /**<The number of rows (or ranks) in a chessboard */
  static const int COLS = 8; /**<The number of columns (or files) in a chessboard */
  static const int NUM_SQUARES = 64; /**<The number of squares in a chessboard */
  /**
   * The values assigned to each piece type
   */
  enum PieceTypes {
    EMPTY = -1,
    BQ, BK, BR, BN, BB, BP,
    WQ, WK, WR, WN, WB, WP
  };

  static const int NUM_COLORED_TYPES = 12; /**< The number of colored piece types */
  static const int NUM_PIECE_TYPES = 6; /**< The number of uncolored piece types */
  //static const int MIN_BLACK_TYPE = 0; /**< The lowest value of a black piece */
  static const int MAX_BLACK_TYPE = 5; /**< The greatest value of a black piece */
  static const int MIN_WHITE_TYPE = 6; /**< The lowest value of a white piece */
  //static const int MAX_WHITE_TYPE = 11; /**< The greatest value of a white piece */

  /**
   * The values assigned to each player color
   */
  enum Colors {
    WHITE, BLACK, BOTH_COLOR
  };

  static const int MOVE_LENGTH_MOVE_LIST = 3; /**< The length of each move in move list */
  static const int MOVE_LENGTH_HISTORY = 4; /**< The length of each move in history */

  enum MoveTypes {
    MOVE_NORMAL,
    MOVE_CASTLING,
    MOVE_PAWN_DOUBLE_JUMP,
    MOVE_PAWN_EN_PASSANT,
    MOVE_PROMOTION_QUEEN,
    MOVE_PROMOTION_ROOK,
    MOVE_PROMOTION_KNIGHT,
    MOVE_PROMOTION_BISHOP
  };

  /***************************************************************************
   *                            Getter Methods
   ***************************************************************************/

  /**
   * Get current player
   * @return Board:WHITE or Board::BLACK
   */
  int getPlayer();

  /**
   * @param square: the square number between 0 and 63
   * @return the type of the piece currently in that square (according to enum Board::pieceTypes).
   */
  int getPiece(int square);

  /**
   * Get the piece in a square
   * @param square: the square number between 0 and 63
   * @return type of the piece in that square (according to enum pieceTypes).
   * -1 if empty, +12 if square is chosen. +25 if square is potential move
   */
  int getPieceGUI(int square);

  /**
   * Get the number of moves available for the current player.
   * @return number of available moves.
   */
  int getNumMoves();

  /**
   * @return the list of all available moves
   */
  std::vector<int> getMoveList();

  /**
   * Get the currently chosen square
   * @return the chosen square, -1 if no square is chosen
   */
  int getChosenSquare();

  /**
   * Get all squares that are legal moves from a starting square
   * @param squareMoves: the vector to put the result squares into
   * @param startSquare: the starting square
   */
  void getMovesFromSquare(std::vector<int>& squareMoves, const int startSquare);

  /**
   * Get the history
   */
  std::vector<int> getHistory();

  /**
   * Get the number of moves (or half-turn) made since game start
   */
  int getGameLength();

  /**
   * Return the winner of the game.
   * Should only be called after the game has ended.
   * @return WHITE, BLACK, BOTH_COLOR (for draw), or -1 if game hasn't ended.
   */
  int getWinner();

  /**
   * Get current king's position.
   * @param color: WHITE or BLACK
   */
  int getKingSquare(int color);

  /***************************************************************************
   *                         Move making and undoing
   ***************************************************************************/

  /**
   * Execute a move, and update the board accordingly.
   * Should be called when all parameters of the move
   * (starting square, ending square, promotion if necessary) have already been decided.
   * @param moveIndex: the move number according to move list (starting from 0)
   */
  void makeMove(int moveIndex);

  /**
   * Pick a square on the board, and execute part of a move.
   * As user input a move using at least 2 clicks,
   * this method allows the program to update the logic between clicks without having to wait till the user makes a complete move.
   * If there is a promotion event, user also needs to call the promote function to set the promoted piece.
   * @param square: a square number between 0 and 63
   */
  void chooseSquare(int square);

  /**
   * Execute a move from square1 to square2.
   * Automatically called by chooseSquare method when 2 appropriate squares are chosen.
   * If the move is a promotion, this method assumes that the promotion type is not yet known,
   * sets the promotion flag and returns without promoting the pawn.
   * @param square1, square2: the starting and ending square (from 0 to 63)
   */
  void makeMove(int square1, int square2);

  /**
   * @return true if there is promotion
   */
  bool hasPromotion();

  /**
   * Choose which piece to promote to
   * @param piece: the type of promotion (according to Move types enum)
   */
  void promote(int promotionType);

  /**
   * Undo the previous move
   */
  void undoMove();

  /***************************************************************************
   *                                Debug
   ***************************************************************************/

  /**
   * Compare 2 board and log any difference.
   * Used to find out if a board is the same after making moves then undoing.
   */
  bool isDifferent(Board& b);

  /**
   * Print to console a row of the board
   * @param line: the row of the board to be printed (0 is the lowest row, and 7 is the highest row)
   */
  void printBoard(int line);
  /**
   * Print to console the list of moves (in chess notation)
   * @param moveNum: the move's position in move list.
   */
  void printMoveList(int moveNum);
  /**
   * Print to console the history (in chess notation)
   * @param moveNum: the number of half-turn.
   */
  void printHistory(int moveNum);


private:
  /***************************************************************************
   *                          Board representation
   ***************************************************************************/

  /**
   * Contains all pieces on chessboard.
   * Values of pieces are indicated in PieceTypes enum.
   */
  int board[ROWS][COLS];

  /**
   * The current player. Can be WHITE or BLACK (false or true when casted to bool)
   */
  int player;

  /**
   * Position of white and black king, value from 0 to 63
   */
  int kingSquares[2];

  /**
   * Keep track of when the kings and rooks first move. Used for castling.
   * Order: king, left rook, right rook. The even flags are for white, and the odds are for black.
   * Value: 0 if the piece has not moved, otherwise the size of the history when the piece first moved.
   */
  int castlingFirstMove[6];

  /**
   * List of possible moves
   * Each move uses 3 ints: starting square, ending square (0 -> 63), and the type of move
   */
  std::vector<int> moveList;

  /**
   * The history of the game.
   * Each move uses 4 ints: starting square (0->63), ending square(0->63),
   * captured piece (Board::EMPTY if no piece captured), and move type.
   */
  std::vector<int> history;

  /**
   * The square chosen by current player.
   * Used to display chosen piece on screen.
   * -1 if no chosen square, else 0 to 63.
   */
  int chosenSquare;

  /**
   * The square that are having a promotion event (0 -> 63).
   * -1 if there is no promotion event.
   */
  int promotionSquare;

  /***************************************************************************
   *                         Legal move generation
   ***************************************************************************/

  /**
   * Squares of the pieces that are checking current player's king.
   * There can be at most 2 pieces. -1 if king is not checked
   */
  int checkingPieces[2];

  /**
   * Squares of the pinned pieces and their pinning pieces alternately.
   * A piece is pinned when it stands between friendly king and an opponent's ray piece.
   * Even index: pinned pieces, odd index: pinning pieces.
   */
  std::vector<int> pinPieces;

  /**
   * 8 basic directions.
   * The first 4 are cardinal (vertical and horizontal). The last 4 are diagonal.
   */
  static const int dir[8][2];
  static const int MAX_CARDINAL_DIR = 3; /**< The greatest value of a cardinal direction */
  static const int MIN_DIAGONAL_DIR = 4; /**< The smallest value of a diagonal direction */

  /**
   * 8 knight's directions (L shaped moves)
   */
  static const int dirKnight[8][2];

  /**
   * Generate the list of available move.
   * Should be called after making a move.
   */
  void updateMoveList();

  /**
   * Check if king is checked by opponent, and if any piece is pinned.
   * Should be called before updating individual piece's moves.
   */
  void findPinAndCheck();

  /**
   * Add all available king moves (including castling) to move list.
   * @param r, c: the row and column of the king (0 -> 7).
   */
  void updateKingMoves(int r, int c);

  /**
   * Add all available moves of a ray piece to move list.
   * @param r, c: the row and column of the ray piece (0 -> 7).
   */
  void updateRayMoves(int r, int c);

  /**
   * Add all available moves of a knight to move list.
   * @param r, c: the row and column of the knight (0 -> 7).
   */
  void updateKnightMoves(int r, int c);

  /**
   * Add all available moves of a pawn to move list.
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
   * @param s1, s2, s3: 3 squares.
   * @return true if square 2 is between square 1 and square 3 in a line (square 2 can be square 1 or square 3).
   */
  bool isInRay(int s1, int s2, int s3);
};

#endif // BOARD_H
