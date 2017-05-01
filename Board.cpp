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

  kingSquares[0] = 4;
  kingSquares[1] = 60;
  enPassantCols[0] = -1;
  enPassantCols[1] = -1;

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
  // Fill all square with ID = 0 (empty)
  for(int i = 0; i < 64; i++)
  {
    board[i/8][i%8] = 0;
  }
  // Fill all white ID
  for(int i = 0; i < 16; i++)
  {
    board[i/8][i%8] = i+1;
  }
  // Fill all black ID
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

void Board::printHitbox()
{
  for (int p = 1; p <= 32; p++)
  {
    printf("pieceID %i\n", p);
    for (int i = 7; i >= 0; i--)
    {
      for (int j = 0; j < 8; j++)
      {
        printf(" %i", hitboxes[p][i][j]);
      }
      printf("\n");
    }
  }
  printf("\n");
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
  printf("Board chooseSquare %i\n", square);
  // if choose the same square or click somewhere else, unchoose the square
  if (square == chosenSquare || square < 0 || square > 63)
  {
    chosenSquare = -1;
  }
  else
  {
    if(chosenSquare == -1) // if currently no square is selected
    {
      int pieceID = board[square/8][square%8];
      if ( (pieceID > 0) && ((pieceID <= 16) == (player == 1)) )
      {
        chosenSquare = square;
        printf("update chosen square\n");
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
  int pieceID = board[x1][y1];

  int moveType = hitboxes[pieceID][x2][y2];
  if (moveType == MOVE_NORMAL || moveType == MOVE_PAWN_DOUBLE_JUMP)
  {
    // make move
    board[x2][y2] = board[x1][y1];
    board[x1][y1] = 0;

    // update en passant possibilities
    // if there is a double jump, possible en passant move for opponent
    enPassantCols[2 - player] = (moveType == MOVE_PAWN_DOUBLE_JUMP)? y2 : -1;

    // update castling possibilities
    if (x1*8+y1 == kingSquares[player -1])
    {
      kingSquares[player-1] = x2*8+y2;
    }

    // change player
    player = 3 - player;
    // update hitboxes (all potential moves)
    updateAll();
  }
  else if (moveType == MOVE_PAWN_ENPASSANT)
  {
    board[x2][y2] = board[x1][y1];
    board[x1][y1] = 0;
    board[x1][y2] = 0;
    // change player
    player = 3 - player;
    updateAll();
  }
}


void Board::updateAll()
{
  // update all hitboxes (without caring about check)
  for (int i = 0; i < 64; i++)
  {
    fillHitbox(i);
  }
  fillHitboxCurrentKing();

  // find check
  findCheck();
  printHitbox();
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

  int moveForward = (pID <= 16)? 1: -1;
  int r = square/8;
  int c = square%8;

  /*
   * Move straight
   */
  // Consider one square ahead
  i = r + moveForward;
  j = c;

  bool canPromote = (i == 0) || (i == 7);
  bool canDoubleJump = (i == 2 || i == 5);;
  bool canEnPassant = (r == 3 && player == 2) || (r == 4 && player == 1);

  if (board[i][j] == 0) //empty square in front
  {
    hitboxes[pID][i][j] = canPromote? MOVE_PAWN_PROMOTION: MOVE_NORMAL;
  }
  else //have piece in front
  {
    canDoubleJump = false;
  }

  // Consider 2 square ahead - double jump
  i += moveForward;
  if (canDoubleJump && board[i][j] == 0) //if can double jump and the 2 square ahead is empty
  {
    hitboxes[pID][i][j] = MOVE_PAWN_DOUBLE_JUMP;
  }

  /*
   * Capture diagonally and en passant
   */
  // Diagonally to the left
  i = r + moveForward;
  j = c-1;
  if (j >= 0 && j < 8)
  {
    if (canEnPassant && j == enPassantCols[player-1]) // if in the correct row and correct col, can en passant
    {
      printf("Can enpassant!");
      hitboxes[pID][i][j] = MOVE_PAWN_ENPASSANT;
    }
    else if (board[i][j] == 0 || (board[i][j] <= 16) == (board[r][c] <= 16)) // if square is empty or has same color, can protect
    {
      hitboxes[pID][i][j] = MOVE_PROTECT;
    }
    else // if has opponent, can move
    {
      hitboxes[pID][i][j] = MOVE_NORMAL;
    }
  }
  // Diagonally to the right
  j = c+1;
  if (j >= 0 && j < 8)
  {
    if (canEnPassant && j == enPassantCols[player-1])
    {
      hitboxes[pID][i][j] = MOVE_PAWN_ENPASSANT;
      printf("Can enpassant!");
    }
    else if (board[i][j] == 0 || (board[i][j] <= 16) == (board[r][c] <= 16))
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
      minDir = 0; maxDir = 3; break;
    case WB: case BB:
      minDir = 4; maxDir = 7; break;
    case WQ: case BQ:
      minDir = 0; maxDir = 7; break;
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
  printf("Board currKing kingID %i\n", kingID);

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
    if (board[i][j] == 0 || (board[i][j] > 16) != (kingID > 16))
    {
      hitboxes[kingID][i][j] = MOVE_NORMAL;
    }
    else
    {
      hitboxes[kingID][i][j] = MOVE_PROTECT;
    }
  }
}
void Board::findCheck()
{
  /*
   * Find at most 2 checking pieces
   */
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
    // if opponent piece can capture king
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

  /*
   * Update hitboxes
   */

  if (checkingPieces[1] != 0) //has 2 checking pieces
  {
    updateDoubleCheck();
  }
  else if (checkingPieces[0] != 0) //has 1 checking piece
  {
    updateSingleCheck();
  }
}

void Board::updateDoubleCheck()
{
  int minPiece = (player == 1)? 1 : 17;
  int maxPiece = (player == 1)? 16 : 32;
  for (int p = minPiece; p <= maxPiece; p++)
  {
    // ignore king
    if (p == 5 || p == 29) continue;
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        hitboxes[p][i][j] = MOVE_ILLEGAL;
      }
    }
  }
}

void Board::updateSingleCheck()
{
  int minPiece = (player == 1)? 1 : 17;
  int maxPiece = (player == 1)? 16 : 32;

  bool rayCheckingPiece =  pieces[checkingPieces[0]] == WQ ||
                           pieces[checkingPieces[0]] == BQ ||
                           pieces[checkingPieces[0]] == WB ||
                           pieces[checkingPieces[0]] == BB ||
                           pieces[checkingPieces[0]] == WR ||
                           pieces[checkingPieces[0]] == BR;


  for (int p = minPiece; p <= maxPiece; p++)
  {
    if (p == 5 || p == 29) continue; //ignore king
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        if(hitboxes[p][i][j] == MOVE_NORMAL || hitboxes[p][i][j] == MOVE_PAWN_DOUBLE_JUMP || hitboxes[p][i][j] == MOVE_PAWN_PROMOTION) //i j is a possile move of p
        {
          if (board[i][j] = checkingPieces[0]) continue; //capture checking piece
          if (rayCheckingPiece) // if checking piece is a ray piece, can move between checking piece and king
          {
            //find coordinate of checking piece
            bool canInterrupt = false;
            for (int s = 0; s < 64; s++)
            {
              if (board[s/8][s%8] == checkingPieces[0])
              {
                canInterrupt = isInRay(s/8, s%8, i, j, kingSquares[player-1]/8, kingSquares[player-1]%8);
                break;
              }
            }
            if (canInterrupt) continue;
          }
          printf("Board singleCheck illegal move\n");
          hitboxes[p][i][j] = MOVE_PROTECT;
        }
      }
    }
  }
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
