#include "Board.h"
#include <PieceType.h>
#include <stdio.h> // TO DO: remove after debug



Board::Board()
{
  initBoard();
}
Board::~Board() {};

const int Board::dir[8][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0},
                               {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
const int Board::dirKnight[8][2] = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
                                     {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

void Board::initBoard(){
  player = 1;
  chosenSquare = -1;

  kingSquares[0] = 4;
  kingSquares[1] = 60;
  enPassantCols[0] = -1;
  enPassantCols[1] = -1;



  // Fill board
  // Fill all squares with -1 (empty)
  for(int i = 16; i < 48; i++)
  {
    board[i/8][i%8] = -1;
  }
  board[0][0] = WR;
  board[0][1] = WN;
  board[0][2] = WB;
  board[0][3] = WQ;
  board[0][4] = WK;
  board[0][5] = WB;
  board[0][6] = WN;
  board[0][7] = WR;

  board[7][0] = BR;
  board[7][1] = BN;
  board[7][2] = BB;
  board[7][3] = BQ;
  board[7][4] = BK;
  board[7][5] = BB;
  board[7][6] = BN;
  board[7][7] = BR;

  for (int j = 0; j < 8; j++)
  {
    board[1][j] = WP;
    board[6][j] = BP;
  }
}

int Board::getPiece(int square)
{
  int piece = board[square/8][square%8];
  if (square == chosenSquare)
  {
    return piece + 12;
  }
  return piece;
}

void Board::printBoard()
{
  printf("Printing Board\n");
  for (int i = 7; i >= 0; i--)
  {
    for (int j = 0; j < 8; j++)
    {
      printf(" %i", board[i][j]);
    }
    printf("\n");
  }
}

void Board::chooseSquare(int square)
{
  // if choose the same square, unchoose the square
  if (square == chosenSquare || square < 0 || square > 63)
  {
    chosenSquare = -1;
  }
  else
  {
    if(chosenSquare == -1) // if currently no square is selected
    {
      int chosenPiece = board[square/8][square%8];
      if ( (chosenPiece >= 0) && ((chosenPiece > 5) == (player == 1)) ) //if the piece chosen is player's piece
      {
        chosenSquare = square;
      }
    }
    else
    {
      makeMove(chosenSquare/8, chosenSquare%8, square/8, square%8);
      chosenSquare = -1;
    }
    printf("Board chosen square %i\n", chosenSquare);
  }
}

void Board::makeMove(int x1,int y1,int x2,int y2)
{
  board[x2][y2] = board[x1][y1];
  board[x1][y1] = -1;
}

void Board::kingSafety()
{
  int r = kingSquares[player-1] / 8;
  int c = kingSquares[player-1] % 8;
  int i,j;

  // Check 8 knight's move
  for (int d = 0; d < 8; d++)
  {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i > 7 || j < 0 || j > 7) continue;
    if (player == 1)
    {
      if (board[i][j] == WN)
      {
          //TO DO
      }
    }
    else
    {
      if (board[i][j] == BN)
      {
        //TO DO
      }
    }
  }
  // Check 8 basic direction
  for(int d = 0; d <= 8; d++)
  {
    i = r;
    j = c;
    while(true)
    {
      // advance 1 square in the direction d
      i += dir[d][0];
      j += dir[d][1];
      // if out of bound
      if (i < 0 || i > 7 || j < 0 || j > 7) break;
      // if empty square, advance 1 more square
      if (board[i][j] == -1) continue;
      // if there is a piece
      if (true)
      {
        switch(board[i][j])
        {
          case WQ: return true;
          case WR:
            if (d < 4) return true;
            break;
          case WB:
            if (d > 3) return true;
            break;
        }
      }
      else
      {
        switch(board[i][j])
        {
          case BQ: return true;
          case BR:
            if (d < 4) return true;
            break;
          case BB:
            if (d > 3) return true;
            break;
        }
      }
      // if get here, already met obstacle. Stop looking in this direction
      break;
    }
  }
  // Check pawn's move
  i = white? (r - 1) : (r + 1);
  j = c - 1;
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (white? WP:BP)) return true;
  }
  j = c + 1;
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (white? WP:BP)) return true;
  }
  // if pass all tests, square is not controlled
  return false;
}

bool Board::isSquareControlled(int square, bool white)
{
  int r = square / 8;
  int c = square % 8;
  int i,j;

  // Check 8 knight's move
  for (int d = 0; d < 8; d++)
  {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i > 7 || j < 0 || j > 7) continue;
    if (white)
    {
      if (board[i][j] == WN) return true;
    }
    else
    {
      if (board[i][j] == BN) return true;
    }
  }
  // Check 8 basic direction
  for(int d = 0; d <= 8; d++)
  {
    i = r;
    j = c;
    while(true)
    {
      // advance 1 square in the direction d
      i += dir[d][0];
      j += dir[d][1];
      // if out of bound
      if (i < 0 || i > 7 || j < 0 || j > 7) break;
      // if empty square, advance 1 more square
      if (board[i][j] == -1) continue;
      // if there is a piece
      if (white)
      {
        switch(board[i][j])
        {
          case WQ: return true;
          case WR:
            if (d < 4) return true;
            break;
          case WB:
            if (d > 3) return true;
            break;
        }
      }
      else
      {
        switch(board[i][j])
        {
          case BQ: return true;
          case BR:
            if (d < 4) return true;
            break;
          case BB:
            if (d > 3) return true;
            break;
        }
      }
      // if get here, already met obstacle. Stop looking in this direction
      break;
    }
  }
  // Check pawn's move
  i = white? (r - 1) : (r + 1);
  j = c - 1;
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (white? WP:BP)) return true;
  }
  j = c + 1;
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (white? WP:BP)) return true;
  }
  // if pass all tests, square is not controlled
  return false;
}

bool Board::isInRay(int x1, int y1, int x2, int y2, int x3, int y3)
{
  if(x1 == x3) // same row
  {
    if (x2 == x3 && ((y2 > y1 && y2 < y3) || (y2 < y1 && y2 > y3)) ) return true;//if in line and square 2 is in the middle
  }
  else if (y1 == y3) // same col
  {
    if (y2 == y3 && ((x2 > x1 && x2 < x3) || (x2 < x1 && x2 > x3)) ) return true;//if in line and square 2 is in the middle
  }
  else if ( (x2 - x1)*(y3-y1) == (y2 - y1)*(x3-x1) ) // same diagonal
  {
    if ((x2 > x1 && x2 < x3) || (x2 < x1 && x2 > x3)) return true;
  }
  return false;
}
