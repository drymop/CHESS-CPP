#include "Board.h"
#include <PieceType.h>
#include <stdio.h>
#include <vector>
Board::Board()
{
  player = 1;
  //board.resize(8, std::vector<int> (8, -1));
  initBoard();
}
Board::~Board() {};
void Board::initBoard(){
  int i,j;
  for (j=0;j<8;j++){
      board[1][j] = WP;
      board[6][j] = BP;
  }
  for (i = 2; i < 6; i++)
  {
    for (j = 0; j < 8; j++)
    {
      board[i][j] = -1;
    }
  }

  board[0][0]= WR;
  board[0][1]= WN;
  board[0][2]= WB;
  board[0][3]= WQ;
  board[0][4]= WK;
  board[0][5]= WB;
  board[0][6]= WN;
  board[0][7]= WR;

  board[7][0]= BR;
  board[7][1]= BN;
  board[7][2]= BB;
  board[7][3]= BQ;
  board[7][4]= BK;
  board[7][5]= BB;
  board[7][6]= BN;
  board[7][7]= BR;
}

int Board::getPiece(int square)
{
  return board[square/8][square%8];
}

void chooseSquare(int square)
{

}

bool Board::makeMove(int x1,int y1,int x2,int y2)
{
    if (isMoveLegal(x1, y1, x2, y2)){
        board[x1][y1]=board[x2][y2];
        return true;
    }
    return false;
}

bool Board::isMoveLegal(int x1, int y1, int x2, int y2)
{
    return true;
}

