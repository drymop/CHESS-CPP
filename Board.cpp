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
  player = 2;
  chosenSquare = -1;

  kingSquares[0] = 4;
  kingSquares[1] = 60;

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

  updateAll();
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
  // if click somewhere else, unchoose the square
  if (square < 0 || square > 63 || square == chosenSquare)
  {
    chosenSquare = -1;
  }
  else
  {
    if(chosenSquare == -1)
    {
      if (board[square/8][square%8] > 0)
      {
        chosenSquare = square;
      }
    }
    else
    {
      makeMove(chosenSquare/8, chosenSquare%8, square/8, square%8);
      chosenSquare = -1;
    }
  }
}

bool Board::makeMove(int x1,int y1,int x2,int y2)
{
    int pieceID = board[x1][y1];
    // choose an empty square
    if (pieceID <= 0) return false;
    // choose an opponent's piece
    if ((player == 1) != (pieceID <= 16)) return false;

    int moveType = hitboxes[pieceID][x2][y2];
    if (moveType == MOVE_NORMAL || moveType == MOVE_PAWN_DOUBLE_JUMP)
    {
      board[x2][y2] = board[x1][y1];
      board[x1][y1] = 0;
      updateAll();
      return true;
    }
    return false;
}

bool Board::isMoveLegal(int x1, int y1, int x2, int y2)
{
    return true;
}


void Board::updateAll()
{
  // change player
  player = 3 - player;
  printf("get to here\n");
  // update all hitboxes (without caring about check)
  for (int i = 0; i < 64; i++)
  {
    fillHitbox(i);
  }
  fillHitboxCurrentKing();
  // compare hitbox for check

}

void Board::fillHitbox(int square)
{
  int pID = board[square/8][square%8];

  // dont fill current player's king's hitbox
  if (player == 1 && pID == 5) return;
  if (player == 2 && pID == 29) return;
  // empty piece
  if(pID == 0) return;
  int pType = pieces[pID];
  switch(pType)
  {
    case WP: case BP: fillHitboxPawn(square, pID); break;
    case WN: case BN: fillHitboxKnight(square, pID); break;
    case WR: case BR:
    case WB: case BB:
    case WQ: case BQ: fillHitboxRayPieces(square, pID); break;
    case WK: case BK: fillHitboxKing(square, pID); break;
  }
}

void Board::fillHitboxPawn(int square, int pID)
{
  int i,j;
  /*
   * Fill hitbox with illegal move
   */
  for (i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
      {
        hitboxes[pID][i][j] = MOVE_ILLEGAL;
      }
  }

  /*
   * Move straight, capture diagonal
   */
  int moveForward = (pID <= 16)? 1: -1;
  int r = square/8;
  int c = square%8;

  bool canPromote;
  bool canDoubleJump;
  //bool canEnpassant;

  //consider one square ahead
  i = r + moveForward;
  j = c;
  canPromote = (i == 0) || (i == 7);
  canDoubleJump = (i == 2 || i == 5);

  if (board[i][j] = 0) //empty square in front
  {
    hitboxes[pID][i][j] = canPromote? MOVE_PAWN_PROMOTION: MOVE_NORMAL;
  }
  else //have piece in front
  {
    canDoubleJump = false;
  }

  // double jump
  i += moveForward;
  if (canDoubleJump && board[i][j] == 0) //if can double jump and the 2 square ahead is empty
  {
    hitboxes[pID][i][j] = MOVE_PAWN_DOUBLE_JUMP;
  }

  //consider diagonal squares
  i = r + moveForward;
  j = c-1;
  if (j >= 0 && j < 8)
  {
    if (board[i][j] == 0 || (board[i][j] <= 16) == (board[r][c] <= 16))
    {
      hitboxes[pID][i][j] = MOVE_PROTECT;
    }
    else
    {
      hitboxes[pID][i][j] = MOVE_NORMAL;
    }
  }
  j = c+1;
  if (j >= 0 && j < 8)
  {
    if (board[i][j] == 0 || (board[i][j] <= 16) == (board[r][c] <= 16))
    {
      hitboxes[pID][i][j] = MOVE_PROTECT;
    }
    else
    {
      hitboxes[pID][i][j] = MOVE_NORMAL;
    }
  }
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
    if(board[i][j] == 0 || (board[r][c] <= 16) != (board[i][j] <= 16)) //if square empty or has opponent, legal move
    {
      hitboxes[pID][i][j] = MOVE_NORMAL;
    }
    else //if contains same color, can protect
    {
      hitboxes[pID][i][j] = MOVE_PROTECT;
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
   * Choose direction (queen: 8 dirs, rook: first 4 dirs, bishop: 4 last dirs)
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
    bool hasObstacle = false;
    while(true)
    {
      // advance 1 square in the direction d
      i += dir[d][0];
      j += dir[d][1];

      // if out of bound
      if (i < 0 || i > 7 || j < 0 || j > 7) break;

      // fill square with move
      if (board[i][j] == 0) // if empty square
      {
        hitboxes[pID][i][j] = hasObstacle? MOVE_BEHIND_OBSTACLE : MOVE_NORMAL;
      }
      else // if meet obstacle
      {
        if (hasObstacle) // if already have 1 obstacle, break
        {
          hitboxes[pID][i][j] = MOVE_BEHIND_OBSTACLE;
          break;
        }
        else // first obstacle
        {
          hasObstacle = true;
          hitboxes[pID][i][j] = ((pID <= 16) != (board[i][j] <= 16))? MOVE_NORMAL : MOVE_PROTECT;
        }
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
    if(board[i][j] == 0 || (board[i][j] <= 16) != (board[r][c] <= 16))
    {
      hitboxes[pID][i][j] = MOVE_NORMAL;
    }
    else
    {
      hitboxes[pID][i][j] = MOVE_PROTECT;
    }
  }
}

void Board::fillHitboxCurrentKing()
{
  int i,j, kingID;
  // current player's king's position
  int r = kingSquares[player-1]/8;
  int c = kingSquares[player-1]%8;
  //oponent pieces
  int minPiece, maxPiece;
  if (player == 1)
  {
    minPiece = 17;
    maxPiece = 32;
    kingID = 5;
  }
  else
  {
    minPiece = 1;
    maxPiece = 16;
    kingID = 29;
  }

  /*
   * Fill hitbox with illegal moves
   */
  for (i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
      {
        hitboxes[kingID][i][j] = MOVE_ILLEGAL;
      }
  }

  /*
   * Look into 8 directions and find legal move
   */
  for(int d = 0; d < 8; d++)
  {
    // advance 1 square in the chosen direction
    i = r + dir[d][0];
    j = c + dir[d][1];
    // if out of the board, finish looking in this direction
    if(i < 0 || i > 7 || j < 0 || j > 7) continue;

    // If Square is controlled by opponent, king protects that square
    bool squareIsControlled = false;
    for (int p = minPiece; p <= maxPiece; p++)
    {
      if (hitboxes[p][i][j] == MOVE_NORMAL || hitboxes[p][i][j] == MOVE_PROTECT)
      {
        squareIsControlled = true;
        break;
      }
    }
    if (squareIsControlled)
    {
      hitboxes[kingID][i][j] = MOVE_PROTECT;
      continue;
    }

    //If square is not controlled by the opponent, king can go as long as there is no same color piece there
    hitboxes[kingID][i][j] = ((board[i][j] <= 16) == (kingID <= 16))? MOVE_PROTECT : MOVE_NORMAL;
  }
}
void Board::findCheck()
{
  checkingPieces[0] = 0;
  checkingPieces[1] = 0;

  // pnly check current player's king
  int rK = kingSquares[player-1] / 8;
  int cK = kingSquares[player-1] % 8;

  // opponent's pieces
  int minPiece, maxPiece;
  if (player == 1)
  {
    minPiece = 17; maxPiece = 32;
  }
  else
  {
    minPiece = 1; maxPiece = 16;
  }

  // find checking pieces
  for(int p = minPiece; p <= maxPiece; p++)
  {
    if(hitboxes[p][rK][cK] == MOVE_NORMAL || hitboxes[p][rK][cK] == MOVE_PAWN_PROMOTION)
    {
      if (checkingPieces[0] != 0)
      {
        checkingPieces[1] = p;
        break; //max 2 checking pieces
      }
      else
      {
        checkingPieces[0] = p;
      }
    }
  }
}
