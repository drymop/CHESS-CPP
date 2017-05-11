#include "Board.h"
#include <PieceType.h>
#include <stdio.h> // TO DO: remove after debug



Board::Board()
{
  initBoard();
}
Board::~Board() {};

bool Board::isDifferent(Board& b)
{
  printf("Comparing\n");
  bool rv = false;
  bool different = false;
  if (this->player != b.player)
  {
    rv = true;
    printf("  Player: %i - %i\n", this->player, b.player);
  }
  for (int s = 0; s < 64; s++)
  {
    if (this->board[s/8][s%8] != b.board[s/8][s%8])
    {
      different = true;
      break;
    }
  }
  if (different)
  {
    rv = true; different = false;
    printf("  Board:\n");
    for (int i = 7; i >= 0; i--)
    {
      printf("  ");
      this->printBoard(i);
      printf("    ");
      b.printBoard(i);
      printf("\n");
    }
  }

  if (this->moveList.size() != b.moveList.size())
  {
    different = true;
  }
  else
  {
    for (int i = 0; i < b.moveList.size(); i++)
    {
      if (this->moveList[i] != b.moveList[i])
      {
        different = true;
        break;
      }
    }
  }
  if (different)
  {
    different = false; rv = true;
    printf("  Move List\n");
    int i;
    for (i = 0; i < this->moveList.size(); i+=3)
    {
      printf("  ");
      this->printMoveList(i/3);
      printf("    ");
      if (i < b.moveList.size()) b.printMoveList(i/3);
      printf("\n");
    }
    for (i; i < b.moveList.size(); i+=3)
    {
      printf("    ");
      b.printMoveList(i/3);
      printf("\n");
    }
  }

  if (this->history.size() != b.history.size())
  {
    different = true;
  }
  else
  {
    for (int i = 0; i < b.history.size(); i+=4)
    {
      if (this->history[i/4] != b.history[i/4])
      {
        different = true;
        break;
      }
    }
  }
  if (different)
  {
    different = false; rv = true;
    printf("  History:\n");
    int i;
    for (i = 0; i < this->history.size(); i+= 4)
    {
      printf("  ");
      this->printHistory(i/4);
      printf("    ");
      if (i < b.history.size()) b.printHistory(i/4);
      printf("\n");
    }
    for (i; i < b.history.size(); i+4)
    {
      printf("    ");
      b.printHistory(i/4);
      printf("\n");
    }
  }

  if (this->chosenSquare != b.chosenSquare)
  {
    rv = true;
    printf("  Choose square: %i - %i\n", this->chosenSquare, b.chosenSquare);
  }

  if (this->kingSquares[0] != b.kingSquares[0] || this->kingSquares[1] != b.kingSquares[1])
  {
    rv = true;
    printf("  Kingsquare: %i %i - %i %i\n", this->kingSquares[0], this->kingSquares[1], b.kingSquares[0], b.kingSquares[1]);
  }

  for (int i = 0; i < 6; i++)
  {
    if (this->castlingFirstMove[i] != b.castlingFirstMove[i])
    {
      different = true;
      break;
    }
  }
  if (different)
  {
    different = false; rv = true;
    printf("  Castling:\n");
    for (int i = 0; i < 6; i++)
    {
      printf("  %i %i\n", this->castlingFirstMove[i], b.castlingFirstMove[i]);
    }
  }
  if (this->promotionSquare != b.promotionSquare)
  {
    rv = true;
    printf("  Promte: %i %i\n", this->promotionSquare, b.promotionSquare);
  }
  return rv;
}

Board::Board(const Board& b)
{
  this->player = b.player;
  for (int i = 0; i < 64; i++)
  {
    this->board[i/8][i%8] = b.board[i/8][i%8];
  }
  for (int i = 0; i < b.moveList.size(); i++)
  {
    this->moveList.push_back(b.moveList[i]);
  }
  for (int i = 0; i < b.history.size(); i++)
  {
    this->history.push_back(b.history[i]);
  }
  this->chosenSquare = b.chosenSquare;
  this->kingSquares[0] = b.kingSquares[0]; this->kingSquares[1] = b.kingSquares[1];
  for (int i = 0; i < 6; i++)
  {
    this->castlingFirstMove[i] = b.castlingFirstMove[i];
  }
  this->promotionSquare = b.promotionSquare;
}

const int Board::dir[8][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0},
                               {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
const int Board::dirKnight[8][2] = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
                                     {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

void Board::initBoard()
{
  player = 0; // white player go first
  chosenSquare = -1; // no chosen square yet

  // variables to keep track of board
  kingSquares[0] = 4;
  kingSquares[1] = 60;
  castlingFirstMove[0] = 0;
  castlingFirstMove[1] = 0;
  castlingFirstMove[2] = 0;
  castlingFirstMove[3] = 0;
  castlingFirstMove[4] = 0;
  castlingFirstMove[5] = 0;
  promotionSquare = -1;

  history.clear();
  history.reserve(200); // 50 half-turns (4 ints/turn)
  moveList.clear();
  moveList.reserve(105); // 35 moves (3 ints/move)
  checkingPieces[0] = -1;
  checkingPieces[1] = -1;
  pinPieces.clear();
  pinPieces.reserve(8); // 4 pairs of pinned and pinning pieces

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
  updateMoveList();
}

int Board::getPiece(int square)
{
  return board[square/8][square%8];
}

int Board::getPieceGUI(int square)
{
  int piece = board[square/8][square%8];
  if (square == chosenSquare)
  {
    return piece + 12;
  }
  return piece;
}

int Board::getPlayer()
{
  return player;
}

int Board::getNumMoves()
{
  return moveList.size()/3;
}

int Board::getWinner()
{
  // in case game hasn't ended
  if (moveList.size() != 0) return -1;

  // if game ended, decide if checkmate or stalemate
  if (checkingPieces[0] != -1)
  {
    return 1-player;
  }
  else
  {
    return 2;
  }
}

std::vector<int> Board::getMoveList()
{
  return moveList;
}

std::vector<int> Board::getHistory()
{
  return history;
}

int Board::getChosenSquare()
{
  return chosenSquare;
}

int Board:: getKingSquare(int which)
{
  return kingSquares[which];
}

int Board::getCastling(int which)
{
  return castlingFirstMove[which];
}

int Board::getPromotionSquare()
{
  return promotionSquare;
}

void Board::getChosenSquareMoves(std::vector<int>& squareMoves)
{
  // no chosen square
  if (chosenSquare == -1) return;
  // iterate through the moveList vector to find all the moves starting at chosenSquare
  // All those moves should be next to each other, so
  bool foundStartSquare = false;
  for (int i = 0; i < moveList.size(); i+= 3)
  {
    if(moveList[i] == chosenSquare)
    {
      foundStartSquare = true;
      squareMoves.push_back(moveList[i+1]);
    }
    else if (foundStartSquare)
    {
      break;
    }
  }
}
void Board::printBoard(int line)
{
  for (int j = 0; j < 8; j++)
  {
    char c;
    switch(board[line][j])
    {
      case -1: c = '-'; break;
      case WP: c = 'p'; break;
      case WN: c = 'n'; break;
      case WB: c = 'b'; break;
      case WR: c = 'r'; break;
      case WQ: c = 'q'; break;
      case WK: c = 'k'; break;
      case BP: c = 'P'; break;
      case BN: c = 'N'; break;
      case BB: c = 'B'; break;
      case BR: c = 'R'; break;
      case BQ: c = 'Q'; break;
      case BK: c = 'K'; break;
    }
    printf("%c ", c);
  }
}

void Board::printMoveList(int moveNum)
{
  moveNum = 3 * moveNum;
  char x1 = 'a' + moveList[moveNum]%8;
  char y1 = '1' + moveList[moveNum]/8;
  char x2 = 'a' + moveList[moveNum+1]%8;
  char y2 = '1' + moveList[moveNum+1]/8;
  printf("%i) %c%c %c%c %i", (moveNum/3 + 1), x1, y1, x2, y2, moveList[moveNum+2]);
}

void Board::printHistory(int moveNum)
{
  moveNum = moveNum * 4;
  printf("%i) %c%i %c%i capture %i type %i", moveNum/4 + 1, history[moveNum]%8 + 'a', history[moveNum]/8 +1, history[moveNum+1]%8 + 'a', history[moveNum+1]/8 +1, history[moveNum+2], history[moveNum+3]);
}

void Board::makeMove(int moveIndex)
{
  if (chosenSquare != -1 || promotionSquare != -1) return;

  moveIndex = moveIndex * 3; //Each move use 3 indexes, so the nth move is at index 3n.
  int x1 = moveList[moveIndex] / 8;
  int y1 = moveList[moveIndex] % 8;
  int x2 = moveList[moveIndex + 1] / 8;
  int y2 = moveList[moveIndex + 1] % 8;
  int moveType = moveList[moveIndex + 2];

  // save move to history
  history.push_back(moveList[moveIndex]);
  history.push_back(moveList[moveIndex+1]);
  history.push_back(board[x2][y2]);
  history.push_back(moveType);

  // realize move
  board[x2][y2] = board[x1][y1];
  board[x1][y1] = -1;

  switch (moveType)
  {
    case MOVE_NORMAL: break;
    case MOVE_PAWN_DOUBLE_JUMP: break;
    case MOVE_PAWN_EN_PASSANT:
      board[x1][y2] = -1;
      break;
    case MOVE_CASTLING:
      if (y2 == 2) //left castling
      {
        board[x2][3] = board[x2][0];
        board[x2][0] = -1;
      }
      else //right castling
      {
        board[x2][5] = board[x2][7];
        board[x2][7] = -1;
      }
      break;
    case MOVE_PROMOTION_QUEEN:
      board[x2][y2] = player? BQ : WQ;
      break;
    case MOVE_PROMOTION_ROOK:
      board[x2][y2] = player? BR : WR;
      break;
    case MOVE_PROMOTION_KNIGHT:
      board[x2][y2] = player? BR : WR;
      break;
    case MOVE_PROMOTION_BISHOP:
      board[x2][y2] = player? BR : WR;
      break;
  }

  /*
   * Update variables for keeping track of the board
   */
  // If king moves, change square of king
  if (moveList[moveIndex] == kingSquares[player])
  {
    kingSquares[player] = moveList[moveIndex+1];
  }
  // If king or rook first move, update castling flags
  switch (moveList[moveIndex])
  {
    case 0:
      if (!castlingFirstMove[2]) castlingFirstMove[2] = history.size();
      break;
    case 4:
      if (!castlingFirstMove[0]) castlingFirstMove[0] = history.size();
      break;
    case 7:
      if (!castlingFirstMove[4]) castlingFirstMove[4] = history.size();
      break;
    case 56:
      if (!castlingFirstMove[3]) castlingFirstMove[3] = history.size();
      break;
    case 60:
      if (!castlingFirstMove[1]) castlingFirstMove[1] = history.size();
      break;
    case 63:
      if (!castlingFirstMove[5]) castlingFirstMove[5] = history.size();
      break;
  }

  /*
   * Change player and update move list
   */
  player = 1 - player;
  updateMoveList();
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
      if ( (chosenPiece >= 0) && ((chosenPiece < 6) == player) ) //if the piece chosen is player's piece
      {
        chosenSquare = square;
      }
    }
    else
    {
      makeMove(chosenSquare, square);
      chosenSquare = -1;
    }
  }
}

void Board::makeMove(int square1,int square2)
{
  if (promotionSquare != -1) return;
  int moveType = -1;
  for (int i = 0; i < moveList.size(); i += 3)
  {
    if (moveList[i] == square1 && moveList[i+1] == square2)
    {
      moveType = moveList[i+2];
      break;
    }
  }

  if (moveType == -1) return;

  int x1 = square1 / 8;
  int y1 = square1 % 8;
  int x2 = square2 / 8;
  int y2 = square2 % 8;
  /*
   * Save move to history
   */
  history.push_back(square1);
  history.push_back(square2);
  history.push_back(board[x2][y2]);
  history.push_back(moveType);
  /*
   * Make the move
   */
  board[x2][y2] = board[x1][y1];
  board[x1][y1] = -1;

  switch (moveType)
  {
    case MOVE_NORMAL: break;
    case MOVE_PAWN_DOUBLE_JUMP: break;
    case MOVE_PAWN_EN_PASSANT:
      board[x1][y2] = -1;
      break;
    case MOVE_CASTLING:
      if (y2 == 2) //left castling
      {
        board[x2][3] = board[x2][0];
        board[x2][0] = -1;
      }
      else //right castling
      {
        board[x2][5] = board[x2][7];
        board[x2][7] = -1;
      }
      break;
    default:
      //if has promotion, set the promotion square and return
      promotionSquare = square2;
      return;
  }

  /*
   * Update variables for keeping track of the board
   */
  // If king moves, change square of king
  if (square1 == kingSquares[player])
  {
    kingSquares[player] = square2;
  }
  // If king or rook move, update castling flags
  switch (square1)
  {
    case 0:
      if (!castlingFirstMove[2]) castlingFirstMove[2] = history.size();
      break;
    case 4:
      if (!castlingFirstMove[0]) castlingFirstMove[0] = history.size();
      break;
    case 7:
      if (!castlingFirstMove[4]) castlingFirstMove[4] = history.size();
      break;
    case 56:
      if (!castlingFirstMove[3]) castlingFirstMove[3] = history.size();
      break;
    case 60:
      if (!castlingFirstMove[1]) castlingFirstMove[1] = history.size();
      break;
    case 63:
      if (!castlingFirstMove[5]) castlingFirstMove[5] = history.size();
      break;
  }

  /*
   * Change player and update move list
   */
  player = 1 - player;
  updateMoveList();
}

bool Board::hasPromotion()
{
  return promotionSquare != -1;
}

void Board::promote(int piece)
{
  piece = piece%6; //only care about the type
  // change the pawn in the promotion square to the piece
  switch (piece)
  {
    case BQ: board[promotionSquare/8][promotionSquare%8] = player? BQ:WQ;
    case BR: board[promotionSquare/8][promotionSquare%8] = player? BR:WR;
    case BN: board[promotionSquare/8][promotionSquare%8] = player? BN:WN;
    case BB: board[promotionSquare/8][promotionSquare%8] = player? BB:WB;
    default: return;
  }
  promotionSquare = -1;
  player = 1 - player;
  updateMoveList();
}

void Board::undoMove()
{
  promotionSquare = -1;
  chosenSquare = -1;

  int i = history.size();
  if (i == 0) return;

  int square1 = history[i-4];
  int square2 = history[i-3];

  int x1 = square1 / 8;
  int y1 = square1 % 8;
  int x2 = square2 / 8;
  int y2 = square2 % 8;
  int moveType = history[i-1];

  // Undo player
  player = 1 - player;

  // Undo move
  board[x1][y1] = board[x2][y2];
  board[x2][y2] = history[i-2];

  // delete history of the move
  history.pop_back();
  history.pop_back();
  history.pop_back();
  history.pop_back();

  // Undo special move
  switch(moveType)
  {
    case MOVE_NORMAL:
      break;
    case MOVE_PAWN_DOUBLE_JUMP:
      updateMoveList();
      return;
    case MOVE_PAWN_EN_PASSANT:
      board[x1][y2] = player? WP : BP;
      updateMoveList();
      return;
    case MOVE_CASTLING:
      if (y2 == 2) //left castling
      {
        board[x2][0] = board[x2][3];
        board[x2][3] = -1;
      }
      else
      {
        board[x2][7] = board[x2][5];
        board[x2][5] = -1;
      }
      break;
    default: //promotion
      board[x1][y1] = player? BP : WP;
      updateMoveList();
      return;
  }

  // Undo variables to keep track of board
  // King square
  if (kingSquares[player] == square2) kingSquares[player] = square1;
  for (i = 0; i < 6; i++)
  {
    if (castlingFirstMove[i] > history.size()) castlingFirstMove[i] = 0;
  }
  updateMoveList();
}

void Board::updateMoveList()
{
  moveList.clear();
  findPinAndCheck();
  //printf("Check: %i %i\n", checkingPieces[0], checkingPieces[1]);
  // loop through all squares, and update player's pieces in those squares
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      // if square is empty, or has opponent's piece, do not update
      if (board[i][j] < 0 || (player == (board[i][j] > 5))) continue;
      // if square has friendly piece, update
      int pType = board[i][j] %6; // don't care about color
      switch (pType)
      {
        case BP: updatePawnMoves(i, j); break;
        case BQ:
        case BR:
        case BB: updateRayMoves(i, j); break;
        case BN: updateKnightMoves(i, j); break;
        case BK: updateKingMoves(i, j);
      }
    }
  }
  //printMoveList();
  //printHistory();
}

void Board::findPinAndCheck()
{
  // Position of King
  int r = kingSquares[player] / 8;
  int c = kingSquares[player] % 8;
  int i,j;

  // Clear checking pieces and pin pieces
  checkingPieces[0] = -1;
  checkingPieces[1] = -1;
  pinPieces.clear();

  // Check 8 basic direction to see if there are ray pieces
  for(int d = 0; d <= 8; d++)
  {
    int pinnedSquare = -1;
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
      if (pinnedSquare == -1) // the first obstacle
      {
        if (player == (board[i][j] < 6)) // if it's a friendly piece
        {
          pinnedSquare = i * 8 + j;
        }
        else // if it's opponent's piece
        {
          // Check if it's checking current king
          int curPieceType = board[i][j]%6; // do not care about color
          if (curPieceType == BQ || (d < 4 && curPieceType == BR) || (d > 3 && curPieceType == BB))
          {
            int checkIndex = (checkingPieces[0] == -1)? 0 : 1;
            checkingPieces[checkIndex] = i * 8 + j;
          }
          break;
        }
      }
      else // the second obstacle
      {
        if ( player == (board[i][j] > 5) ) // if it's opponent's piece
        {
          int curPieceType = board[i][j]%6; // do not care about color
          if (curPieceType == BQ || (d < 4 && curPieceType == BR) || (d > 3 && curPieceType == BB)) // if it's opponent's ray piece
          {
            pinPieces.push_back(pinnedSquare);
            pinPieces.push_back(i* 8 + j);
          }
        }
        break;
      }
    }
  }

  if (checkingPieces[1] != -1) return;

  // Check 8 knight's direction to see if there are opponent's knight
  for (int d = 0; d < 8; d++)
  {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i > 7 || j < 0 || j > 7) continue;

    if (board[i][j] == (player? WN : BN) ) // if has opponent's knight
    {
      int t = (checkingPieces[0] == -1)? 0 : 1;
      checkingPieces[t] = i * 8 + j;
      // if there are 2 checking pieces, that's max
      if (t == 1) return;
    }
  }

  // Check 2 squares diagonally to the left and right to see if there are opponent pawns
  i = player? (r - 1) : (r + 1);
  j = c - 1; // The left column
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if ( board[i][j] == (player? WP:BP) )
    {
      int t = (checkingPieces[0] == -1)? 0 : 1;
      checkingPieces[t] = i * 8 + j;
      if (t == 1) return;
    }
  }
  j = c + 1; // The right column
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (player? WP:BP))
    {
      int t = (checkingPieces[0] == -1)? 0 : 1;
      checkingPieces[t] = i * 8 + j;
    }
  }
}

void Board::updateKingMoves(int r, int c)
{
  // king's position
  int i, j;
  /*
   * 8 squares around king
   */
  for (int d = 0; d < 8; d++)
  {
    i = r + dir[d][0];
    j = c + dir[d][1];
    if (i < 0 || i > 7 || j < 0 || j > 7) continue; // out of bound

    if ((board[i][j] != -1) && (player == (board[i][j] < 6))) continue; // square has a friendly piece

    if (isSquareControlled(i, j)) continue; //square is controlled by opponent

    // if the king is between a square and opponent's ray piece, king cannot move to that square
    if (checkingPieces[0] != -1
        && isInRay(checkingPieces[0]/8, checkingPieces[0]%8, r, c, i, j) )
    {
      continue;
    }

    if (checkingPieces[1] != -1
        && isInRay(checkingPieces[1]/8, checkingPieces[1]%8, r, c, i, j) )
    {
      continue;
    }

    // If passes all tests above, the square is a legal move. Add to moveList
    moveList.push_back(r * 8 + c); //starting square
    moveList.push_back(i * 8 + j); //ending square
    moveList.push_back(MOVE_NORMAL); // move type
  }

  /*
   * Castling
   */
  if (!castlingFirstMove[player] && checkingPieces[0] == -1) //king has not moved and is not checked
  {
    // left rook has not moved, and the squares left of king are empty and not controlled by the opponent
    if ( !castlingFirstMove[player + 2] && (board[r][1] == -1) && (board[r][2] == -1) && (board[r][3] == -1)
         && !isSquareControlled(r, 2) && !isSquareControlled(r, 3) )
    {
      moveList.push_back(r * 8 + c); //starting square
      moveList.push_back(r * 8 + 2); //ending square
      moveList.push_back(MOVE_CASTLING); // move type
    }
    // right rook has not moved, and the 2 squares right of king are empty and not controlled by the opponent
    if ( !castlingFirstMove[player + 4] && (board[r][5] == -1) && (board[r][6] == -1)
         && !isSquareControlled(r, 5) && !isSquareControlled(r, 6) )
    {
      moveList.push_back(r * 8 + c); //starting square
      moveList.push_back(r * 8 + 6); //ending square
      moveList.push_back(MOVE_CASTLING); // move type
    }
  }
}

void Board::updateRayMoves(int r, int c)
{
  /*
   * Find if the king is checked or the piece is pinned
   */
  // if 2 pieces are checking the king, the ray piece cannot move
  if (checkingPieces[1] != -1) return;

  int checkingSquare = -1; // the square of the piece that is checking the king or pinning the ray piece
  int raySquare = r * 8 + c;

  if (checkingPieces[0] != -1) checkingSquare = checkingPieces[0];

  for (int i = 0; i < pinPieces.size(); i += 2)
  {
    if (pinPieces[i] == raySquare)
    {
      if (checkingSquare != -1) return; //if ray is pinned and king is checked, ray cannot move
      checkingSquare = pinPieces[i+1];
      break;
    }
  }

  /*
   * Update ray's move
   * If there is a piece checking king or pinning ray, ray can only move between the checking piece and the king
   */
  int i,j;

  /*
   * Choose direction (queen: 8 dirs, rook: first 4 dirs, bishop: 4 last dirs)
   */
  int minDir, maxDir;
  switch(board[r][c])
  {
    case WR: case BR:
      minDir = 0; maxDir = 3; break;
    case WB: case BB:
      minDir = 4; maxDir = 7; break;
    case WQ: case BQ:
      minDir = 0; maxDir = 7; break;
  }
  // Look into all directions
  for(int d = minDir; d <= maxDir; d++)
  {
    i = r;
    j = c;
    while(true)
    {
      // advance 1 square in the direction d
      i += dir[d][0];
      j += dir[d][1];

      // if out of bound, look in another direction
      if (i < 0 || i > 7 || j < 0 || j > 7) break;

      // if square has friendly piece, look in another direction
      if ((board[i][j] != -1) && (player == (board[i][j] < 6)))
      {
        break;
      }

      // if moving causes king danger, advance 1 more square in direction d
      if ( (checkingSquare != -1)
           && !isInRay(checkingSquare/8, checkingSquare%8, i, j, kingSquares[player]/8, kingSquares[player]%8) ) continue;

      /*
       * At this point:
       * + King is not endangered if move
       * + Square is empty or has opponent's piece
       */
      moveList.push_back(raySquare);
      moveList.push_back(i * 8 + j);
      moveList.push_back(MOVE_NORMAL);
      if (board[i][j] != -1) break;
    }
  }
}

void Board::updateKnightMoves(int r, int c)
{
  /*
   * Find if the king is checked or the pawn is pinned
   */
  // if 2 pieces are checking king, knight cannot move
  if (checkingPieces[1] != -1) return;

  int knightSquare = r * 8 + c;
  for (int i = 0; i < pinPieces.size(); i += 2)
  {
    if (pinPieces[i] == knightSquare) return; // if knight is pinned by a ray piece, it cannot move (because it cannot return to the same ray)
  }

  /*
   * Update knight's move
   * If there is a piece checking king, knight can only move between the checking piece and the king
   */
  int i,j;
  //Look in 8 knight directions and add legal move
  for (int d = 0; d < 8; d++)
  {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i > 7 || j < 0 || j > 7) continue;
    // if moving causes king danger (not going between king and the checking piece), check other directions
    if ((checkingPieces[0] != -1) && !isInRay(checkingPieces[0]/8, checkingPieces[0]%8, i, j, kingSquares[player]/8, kingSquares[player]%8)) continue;

    if( board[i][j] == -1 || (player == (board[i][j] > 5)) ) //if square empty or has opponent, legal move
    {
      moveList.push_back(knightSquare);
      moveList.push_back(i * 8 + j);
      moveList.push_back(MOVE_NORMAL);
    }
  }
}

void Board::updatePawnMoves(int r, int c)
{
  /*
   * Find if the king is checked or the pawn is pinned
   */
  // if 2 pieces are checking king, pawn cannot move
  if (checkingPieces[1] != -1) return;

  int checkingSquare = -1; // the square of the piece that is checking king or pinning pawn
  int pawnSquare = r * 8 + c;

  checkingSquare = checkingPieces[0];

  for (int i = 0; i < pinPieces.size(); i += 2)
  {
    if (pinPieces[i] == pawnSquare)
    {
      if (checkingSquare != -1) return; //if both pawn is pinned and king is checked, pawn cannot move
      checkingSquare = pinPieces[i+1];
      break;
    }
  }

  /*
   * Update pawn's move
   * If there is a piece checking king or pinning pawn, pawn can only move between the checking piece and the king
   */
  int i,j;
  int moveForward = player? -1: 1; // white pawn moves up, black pawn moves down

  bool canPromote = (r == (player? 1 : 6)); // is in the correct row for promotion
  bool canDoubleJump = (r == (player? 6 : 1)); // is in the correct row for double jump
  bool canEnPassant = (r == (player? 3 : 4))
                      && (history.size() != 0) && (history[history.size()-1] == MOVE_PAWN_DOUBLE_JUMP); // is in the correct row for en passant, and the last move is a double jump


  /*
   * Move straight
   */
  // One square ahead
  i = r + moveForward;
  j = c;

  if (board[i][j] == -1 // empty square in front
      && ((checkingSquare == -1) || isInRay(checkingSquare/8, checkingSquare%8, i, j, kingSquares[player]/8, kingSquares[player]%8))) // no king danger if move there
  {
    // can jump 1 square forward
    if (canPromote)
    {
      // if can promote, add 4 moves (promote to queen, rook, knight, or bishop)
      int endSquare = i * 8 + j;
      moveList.push_back(pawnSquare);
      moveList.push_back(endSquare);
      moveList.push_back(MOVE_PROMOTION_QUEEN);
      moveList.push_back(pawnSquare);
      moveList.push_back(endSquare);
      moveList.push_back(MOVE_PROMOTION_ROOK);
      moveList.push_back(pawnSquare);
      moveList.push_back(endSquare);
      moveList.push_back(MOVE_PROMOTION_KNIGHT);
      moveList.push_back(pawnSquare);
      moveList.push_back(endSquare);
      moveList.push_back(MOVE_PROMOTION_BISHOP);
    }
    else
    {
      moveList.push_back(pawnSquare);
      moveList.push_back(i * 8 + j);
      moveList.push_back(MOVE_NORMAL);
    }
  }
  else //cannot double jump if has piece in front or king is in danger if pawn move forward
  {
    canDoubleJump = false;
  }

  // Two squares ahead
  i += moveForward;
   //if in the correct row and the 1st square ahead is empty, and the 2nd square ahead is empty, can jump 2 squares ahead
  if (canDoubleJump && board[i][j] == -1)
  {
    moveList.push_back(pawnSquare);
    moveList.push_back(i * 8 + j);
    moveList.push_back(MOVE_PAWN_DOUBLE_JUMP);
  }

  /*
   * Capture diagonally and en passant
   */
  i = r + moveForward;
  // check through the left and right column
  for (j = c - 1; j <= c+1; j += 2)
  {
    if (j >= 0 && j < 8  //not outside board
        && ((checkingSquare == -1) || isInRay(checkingSquare/8, checkingSquare%8, i, j, kingSquares[player]/8, kingSquares[player]%8))) //no king danger if move there
    {
      if (canEnPassant && j == history[history.size()-3] % 8) // if in the correct row and correct col, can en passant
      {
        moveList.push_back(pawnSquare);
        moveList.push_back(i * 8 + j);
        moveList.push_back(MOVE_PAWN_EN_PASSANT);
      }
      else if (board[i][j] != -1 && (player == (board[i][j] > 5))) // if has opponent, can capture
      {
        if (canPromote)
        {
          // if can promote, add 4 moves (promote to queen, rook, knight, or bishop)
          int endSquare = i * 8 + j;
          moveList.push_back(pawnSquare);
          moveList.push_back(endSquare);
          moveList.push_back(MOVE_PROMOTION_QUEEN);
          moveList.push_back(pawnSquare);
          moveList.push_back(endSquare);
          moveList.push_back(MOVE_PROMOTION_ROOK);
          moveList.push_back(pawnSquare);
          moveList.push_back(endSquare);
          moveList.push_back(MOVE_PROMOTION_KNIGHT);
          moveList.push_back(pawnSquare);
          moveList.push_back(endSquare);
          moveList.push_back(MOVE_PROMOTION_BISHOP);
        }
        else
        {
          moveList.push_back(pawnSquare);
          moveList.push_back(i * 8 + j);
          moveList.push_back(MOVE_NORMAL);
        }
      }
    }
  }
}

bool Board::isSquareControlled(int r, int c)
{
  int i,j;

  /*
   * Check 8 knight's move
   */
  for (int d = 0; d < 8; d++)
  {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i > 7 || j < 0 || j > 7) continue;
    if (board[i][j] == (player? WN : BN) ) return true;
  }

  /*
   * Check 8 ray directions
   */
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
      // if it is opponent's piece
      if (player == (board[i][j] > 5))
      {
        // if it's opponent's ray piece
        int curPiece = board[i][j] % 6;
        if (curPiece == BQ || (d < 4 && curPiece == BR) || (d > 3 && curPiece == BB)) return true;
      }
      break;
    }
  }
  /*
   * Check pawn's move
   */
  i = player? (r - 1) : (r + 1);
  j = c - 1; // left column
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (player? WP : BP)) return true;
  }
  j = c + 1; // right column
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (player? WP : BP)) return true;
  }

  /*
   * Check if controlled by opponent's king
   */
  for (int d = 0; d < 8; d++)
  {
    i = r + dir[d][0];
    j = c + dir[d][1];
    if (i >= 0 && i < 8 && j >= 0 && j < 8)
    {
      if (board[i][j] == (player? WK : BK)) return true;
    }
  }

  // if pass all tests, square is not controlled by opponent
  return false;
}

bool Board::isInRay(int x1, int y1, int x2, int y2, int x3, int y3)
{
  // sai khi 3 con cung hang

  return ((x1 - x2) * (y1 - y3) == (x1 - x3) * (y1 - y2)) // 3 squares are in a line
         && ((x2 >= x1 && x2 <= x3) || (x2 <= x1 && x2 >= x3)) && ((y2 >= y1 && y2 <= y3) || (y2 <= y1 && y2 >= y3)); // square 2 is in the middle (or square 1 is square 2)
}
