#include "Board.h"
#include <PieceType.h>
#include <stdio.h>


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

  /*
   * board after init
   *
   * 25 26 27 28 29 30 31 32
   * 17 18 19 20 21 22 23 24
   *  0  0  0  0  0  0  0  0
   * ...
   * ...
   *  9 10 11 12 13 14 15 16
   *  1  2  3  4  5  6  7  8
   *
   * piece after init
   * {-1, WR, WN, WB, WQ ... BN, BR}
   */

  // Fill board
  // Fill all square with ID = 0 empty
  for(int i = 0; i < 64; i++)
  {
    board[i/8][i%8] = 0;
  }
  for(int i = 0; i < 16; i++)
  {
    board[i/8][i%8] = i+1;
  }
  for(int i = 16; i < 32; i++)
  {
    board[4 + i/8][i%8] = i+1;
  }

  //Fill pieces
  pieces[0] = -1;
  pieces[1] = WR;
  pieces[2] = WN;
  pieces[3] = WB;
  pieces[4] = WQ;
  pieces[5] = WK;
  pieces[6] = WB;
  pieces[7] = WN;
  pieces[8] = WR;
  for (int i = 9; i <= 16; i++)
  {
    pieces[i] = WP;
  }
  for(int i = 17; i <= 24; i++)
  {
    pieces[i] = BP;
  }
  pieces[25] = BR;
  pieces[26] = BN;
  pieces[27] = BB;
  pieces[28] = BQ;
  pieces[29] = BK;
  pieces[30] = BB;
  pieces[31] = BN;
  pieces[32] = BR;
}

int Board::getPiece(int square)
{
  int piece = pieces[ board[square/8][square%8] ];
  if (square == chosenSquare)
  {
    return piece + 12;
  }
  return piece;
}

void Board::chooseSquare(int square)
{
  printf("board: choose square %i\n", square);
  // if click somewhere else, unchoose the square
  if (square < 0 || square > 63 || square == chosenSquare)
  {
    chosenSquare = -1;
  }
  else
  {
    if(chosenSquare == -1)
    {
      if (board[square/8][square%8] >= 0)
      {
        chosenSquare = square;
        printf("board: get piece pick up\n");
      }
    }
    else
    {
      makeMove(chosenSquare/8, chosenSquare%8, square/8, square%8);
      chosenSquare = -1;
      printf("board: get piece put down\n");
    }
  }
  printf("board: chosen square is %i\n"+chosenSquare);
}

bool Board::makeMove(int x1,int y1,int x2,int y2)
{
    if (isMoveLegal(x1, y1, x2, y2)){
        board[x2][y2] = board[x1][y1];
        board[x1][y1] = 0;
        return true;
    }
    return false;
}

bool Board::isMoveLegal(int x1, int y1, int x2, int y2)
{
    return true;
}

void Board::fillHitbox(int square)
{
  int pID = board[square/8][square%8];
  // empty piece
  if(pID == 0) return;
  int pType = pieces[pID];
  switch(pType)
  {
    //case WP: case BP: fillHitboxPawn(square, pID); break;
    case WR: case BR:
    case WB: case BB:
    case WQ: case BQ: fillHitboxRayPieces(square, pID); break;

  }
}

void Board::fillHitboxPawn(int square, int pID)
{

}

void Board::fillHitboxKnight(int square, int pID)
{
  int i,j;
  /*
   * Fill hitbox[pID] with illegal move
   */
  for (i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
      {
        hitboxes[pID][i][j] = MOVE_ILLEGAL;
      }
  }

  /*
   * Look in 8 knight direction and add legal move
   */
  // Coordinate of the piece
  int r = square / 8;
  int c = square % 8;
  // Look in 8 direction
  for (int d = 0; d < 8; d++)
  {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i > 7 || j < 0 || j > 7) continue;

    if(board[i][j] == 0) //if square empty, legal move
    {
      hitboxes[pID][i][j] = MOVE_NORMAL;
    }
    else if ( (board[r][c] <= 16) != (board[i][j] <= 16) ) // if square has opponent's piece, legal move
    {
      hitboxes[pID][i][j] = MOVE_NORMAL;
    }
  }
}

void Board::fillHitboxRayPieces(int square, int pID)
{
  int i,j;
  /*
   * Fill all with illegal move
   */
  for (i = 0; i < 8; i++)
  {
    for (j = 0; j < 8; j++)
    {
      hitboxes[pID][i][j] = MOVE_ILLEGAL;
    }
  }
  int minDir, maxDir;
  /*
   * Choose direction
   */
  switch(pieces[pID])
  {
    case WR: case BR:
      minDir = 0; maxDir = 3;
    case WB: case BB:
      minDir = 4; maxDir = 7;
    case WQ: case BQ:
      minDir = 0; maxDir = 7;
   }
   /*
    * Look into all directions
    * Fill hitbox
    */
  // coordinate of piece
  int r = square/8;
  int c = square%8;
  // Look into all directions
  for(int d = minDir; d <= maxDir; d++)
  {
    i = r;
    j = c;
    while(true)
    {
      // advance 1 square in the current direction
      i += dir[d][0];
      j += dir[d][1];
      if (i < 0 || i > 7 || j < 0 || j > 7) break;
      if (board[i][j] == 0) // if empty square, potential move
      {
        hitboxes[pID][i][j] = MOVE_NORMAL;
      }
      else // if meet obstacle
      {
        if( (pID <= 16) != (board[i][j] <= 16) ) //if opposite color, still legal
        {
          hitboxes[pID][i][j] = MOVE_NORMAL;
        }
        i += dir[d][0];
        j += dir[d][1];
        hitboxes[pID][i][j] = MOVE_BEHIND_OBSTACLE;
        break;
      }
    }
  }
}

void Board::fillHitboxKing(int square, int pID)
{
  int i,j;
  /*
   * Fill hitbox with illegal moves
   */
  for (i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
      {
        hitboxes[pID][i][j] = MOVE_ILLEGAL;
      }
  }

  /*
   * Look into 8 directions and find legal move
   */
  int r = square/8;
  int c = square%8;

  for(int d = 0; d < 8; d++)
  {
    i = r + dir[d][0];
    j = c + dir[d][1];

    if(i < 0 || i > 7 || j < 0 || j > 7) continue;
    if((board[i][j] <= 16) != (board[r][c] <= 16))
    {
      hitboxes[pID][i][j] = MOVE_NORMAL;
    }
  }
}
