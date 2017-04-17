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
     * @return piece according to enum pieceType, -1 if empty
     */
    int getPiece(int square);

    /**
     * Notify the board that a player has clicked a square
     * @param square the square number between 0 and 63, -1 if user click outside the board
     */
    void chooseSquare(int square);

  private:
    int board[8][8];
    int player;

    /**
     * square chosen when player click,
     * -1 if no chosen square, else 0 to 63
     */
    int chosenSquare;

    /**
     * move a piece from square (x1, y1) to (x2, y2)
     * @param x1, y1 row and column of the starting square (from 0 to 7)
     * @param x2, y2 row and column of the destination square (from 0 to 7)
     * e.g square a3 is (0, 2)
     */
    bool makeMove(int x1,int y1,int x2,int y2);



    bool isMoveLegal(int x1, int y1, int x2, int y2);
};

#endif // BOARD_H
