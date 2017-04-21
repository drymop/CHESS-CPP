#ifndef BOARD_H
#define BOARD_H


class Board
{
  public:
    Board();
    ~Board();
    void initBoard();

    /**
     * Get current player
     * @return 0 if game is ended, 1 if player 1, 2 if player 2
     */
    int getPlayer();

    /**
     * Get the piece in a square
     * @param the square number between 0 and 63
     * The square numbering is like this in a chessboard:
     *
     *   56 .. 63
     *   .. .. ..
     *   08 .. 15
     *   00 .. 07
     * @return type of piece in square according to enum pieceType, -1 if empty, +12 if square is chosen
     *
     */
    int getPiece(int square);

    /**
     * Call when a player clicks a square
     * @param square the square number between 0 and 63, -1 if user click outside the board
     */
    void chooseSquare(int square);

  private:
    /**
     * Index: row and column
     * Value: ID of piece
     */
    int board[8][8];
    /**
     * Index: ID of piece, index 0 is empty piece
     * Value: Type of piece
     */
    int pieces[33];

    /**
     * 1 is the first player, 2 is the second player
     */
    int player;

    /**
     * square chosen when player click,
     * -1 if no chosen square, else 0 to 63
     */
    int chosenSquare;



    int hitboxes[32][8][8];
    enum moveType {
      MOVE_ILLEGAL,
      MOVE_SELF, //the square where the piece is
      MOVE_NORMAL,
      MOVE_ENPASSANT,
      MOVE_CASTLING,
      MOVE_BEHIND_OBSTACLE //an illegal square of ray piece, directly after an obstacle
    };
    /**
     * 8 basic direction
     */
    static const int dir[8][2];
    /**
     * 8 knight's direction
     */
    static const int dirKnight[8][2];

    /**
     * move a piece from square (x1, y1) to (x2, y2)
     * @param x1, y1 row and column of the starting square (from 0 to 7)
     * @param x2, y2 row and column of the destination square (from 0 to 7)
     * e.g square a3 is (0, 2)
     */
    bool makeMove(int x1,int y1,int x2,int y2);



    bool isMoveLegal(int x1, int y1, int x2, int y2);

    /**
     * Fill a hitbox with move Types
     * @param pID: ID of piece
     */
    void fillHitbox(int square);

    /**
     * Fill a hitbox of a specific type of piece with move types
     */
    void fillHitboxPawn(int square, int pID);
    void fillHitboxKnight(int square, int pID);
    void fillHitboxKing(int square, int pID);
    void fillHitboxRayPieces(int square, int pID);
};

#endif // BOARD_H
