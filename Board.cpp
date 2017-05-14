#include "Board.h"

#include <stdio.h> // TO DO: remove after debug


const int Board::dir[8][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0},
                               {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
const int Board::dirKnight[8][2] = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
                                     {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

void Board::initBoard() {
  player = WHITE; // white player go first
  chosenSquare = -1; // no chosen square yet

  // variables to keep track of board
  kingSquares[WHITE] = 4;
  kingSquares[BLACK] = 60;
  castlingFirstMove[0] = 0;
  castlingFirstMove[1] = 0;
  castlingFirstMove[2] = 0;
  castlingFirstMove[3] = 0;
  castlingFirstMove[4] = 0;
  castlingFirstMove[5] = 0;
  promotionSquare = -1;

  // reserve memory for the vectors
  // so that they don't have to reallocate too many times
  history.clear();
  history.reserve(130); // 35 moves (4 ints/move)
  moveList.clear();
  moveList.reserve(105); // 35 moves (3 ints/move)
  checkingPieces[0] = -1;
  checkingPieces[1] = -1;
  pinPieces.clear();
  pinPieces.reserve(8); // 4 pairs of pinned and pinning pieces

  // Fill board
  for(int i = 16; i < 48; i++) {
    board[i/COLS][i%COLS] = EMPTY;
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

  for (int j = 0; j < COLS; j++) {
    board[1][j] = WP;
    board[6][j] = BP;
  }
  updateMoveList();
}

int Board::getPiece(int square) {
  return board[square/COLS][square%COLS];
}

int Board::getPieceGUI(int square) {
  int piece = board[square/COLS][square%COLS];
  if (square == chosenSquare) {
    return piece + NUM_COLORED_TYPES;
  }
  return piece;
}

int Board::getPlayer() {
  return player;
}

int Board::getNumMoves() {
  return moveList.size() / MOVE_LENGTH_MOVE_LIST;
}

std::vector<int> Board::getMoveList() {
  return moveList;
}

int Board::getWinner() {
  // in case game hasn't ended
  if (moveList.size() != 0) return -1;
  // if game has ended, decide if checkmate or stalemate
  if (checkingPieces[0] != -1) {
    return 1 - player;
  } else {
    return BOTH_COLOR;
  }
}

std::vector<int> Board::getHistory() {
  return history;
}

int Board::getGameLength() {
  return history.size() / MOVE_LENGTH_HISTORY;
}

int Board::getChosenSquare() {
  return chosenSquare;
}

int Board:: getKingSquare(int color) {
  return kingSquares[color];
}

void Board::getMovesFromSquare(std::vector<int>& squareMoves, const int startSquare) {
  // iterate through the moveList vector to find all the moves starting at chosenSquare
  // All those moves SHOULD be next to each other
  bool foundStartSquare = false;
  for (int i = 0; i < moveList.size(); i+= MOVE_LENGTH_MOVE_LIST) {
    if(moveList[i] == startSquare) {
      foundStartSquare = true; //
      squareMoves.push_back(moveList[i+1]);
    } else if (foundStartSquare) {
      break;
    }
  }
}

void Board::makeMove(int moveIndex) {
  // cannot make a whole move is the previous move has not finished
  if (promotionSquare != -1) return;

  //Each move use 3 indexes, so the nth move is at index 3n.
  moveIndex = moveIndex * MOVE_LENGTH_MOVE_LIST;
  int x1 = moveList[moveIndex] / COLS;
  int y1 = moveList[moveIndex] % COLS;
  int x2 = moveList[moveIndex + 1] / COLS;
  int y2 = moveList[moveIndex + 1] % COLS;
  int moveType = moveList[moveIndex + 2];

  // save move to history
  history.push_back(moveList[moveIndex]); //start square
  history.push_back(moveList[moveIndex+1]); //end square
  history.push_back(board[x2][y2]); //captured piece
  history.push_back(moveType); //move type

  // move piece from square 1 to square 2
  board[x2][y2] = board[x1][y1];
  board[x1][y1] = EMPTY;

  switch (moveType) {
    case MOVE_NORMAL: break;
    case MOVE_PAWN_DOUBLE_JUMP: break;
    case MOVE_PAWN_EN_PASSANT:
      board[x1][y2] = EMPTY;
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
      board[x2][y2] = player? BN : WN;
      break;
    case MOVE_PROMOTION_BISHOP:
      board[x2][y2] = player? BB : WB;
      break;
  }

  ////////////////////////////////////////////////////////
  //  Update variables for keeping track of the board
  ////////////////////////////////////////////////////////

  // Update square of king if king moves
  if (moveList[moveIndex] == kingSquares[player]) kingSquares[player] = moveList[moveIndex+1];

  // Update castling flags if king or rook move for the first time
  switch (moveList[moveIndex]) {
    case 0: //left white rook moved
      if (!castlingFirstMove[2]) castlingFirstMove[2] = history.size();
      break;
    case 4: //white king moved
      if (!castlingFirstMove[0]) castlingFirstMove[0] = history.size();
      break;
    case 7: //right white rook moved
      if (!castlingFirstMove[4]) castlingFirstMove[4] = history.size();
      break;
    case 56: //left black rook moved
      if (!castlingFirstMove[3]) castlingFirstMove[3] = history.size();
      break;
    case 60: //black king moved
      if (!castlingFirstMove[1]) castlingFirstMove[1] = history.size();
      break;
    case 63: //right black rook moved
      if (!castlingFirstMove[5]) castlingFirstMove[5] = history.size();
      break;
  }

  /*
   * Change player and update move list
   */
  player = 1 - player;
  updateMoveList();
}

void Board::chooseSquare(int square) {
  if (square == chosenSquare) { //if choose the same square, unchoose the square
    chosenSquare = -1;
  }
  else {
    if(chosenSquare == -1) { //currently no square is selected
      int chosenPiece = board[square/COLS][square%COLS];
      //if the piece chosen is player's piece, choose the given square
      if ( (chosenPiece != EMPTY) && ((chosenPiece < MIN_WHITE_TYPE) == player) ) {
        chosenSquare = square;
      }
    } else {
      makeMove(chosenSquare, square);
      chosenSquare = -1;
    }
  }
}

void Board::makeMove(int square1,int square2){
  // cannot make move if the last move is not completed
  if (promotionSquare != -1) return;

  int moveType = -1;
  for (int i = 0; i < moveList.size(); i += MOVE_LENGTH_MOVE_LIST) {
    if (moveList[i] == square1 && moveList[i+1] == square2) {
      moveType = moveList[i+2];
      break;
    }
  }
  // move not legal
  if (moveType == -1) return;

  int x1 = square1 / COLS;
  int y1 = square1 % COLS;
  int x2 = square2 / COLS;
  int y2 = square2 % COLS;

  // Save move to history
  history.push_back(square1);
  history.push_back(square2);
  history.push_back(board[x2][y2]);
  history.push_back(moveType);

  //Make the move
  board[x2][y2] = board[x1][y1];
  board[x1][y1] = EMPTY;
  switch (moveType) {
    case MOVE_NORMAL: break;
    case MOVE_PAWN_DOUBLE_JUMP: break;
    case MOVE_PAWN_EN_PASSANT:
      board[x1][y2] = EMPTY;
      break;
    case MOVE_CASTLING:
      if (y2 == 2) { //left castling
        board[x2][3] = board[x2][0];
        board[x2][0] = -1;
      } else { //right castling
        board[x2][5] = board[x2][7];
        board[x2][7] = -1;
      }
      break;
    default: // has promotion
      promotionSquare = square2;
      return;
  }
  //////////////////////////////////////////////////////
  //  Update variables for keeping track of the board
  //////////////////////////////////////////////////////

  // If king moves, change square of king
  if (square1 == kingSquares[player]) {
    kingSquares[player] = square2;
  }
  // If king or rook move, update castling flags
  switch (square1) {
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

  // Change player and update move list
  player = 1 - player;
  updateMoveList();
}

bool Board::hasPromotion() {
  return promotionSquare != -1;
}

void Board::promote(int promotionType) {
  switch (promotionType) {
    case MOVE_PROMOTION_QUEEN : board[promotionSquare/COLS][promotionSquare%COLS] = player? BQ:WQ; break;
    case MOVE_PROMOTION_ROOK  : board[promotionSquare/COLS][promotionSquare%COLS] = player? BR:WR; break;
    case MOVE_PROMOTION_BISHOP: board[promotionSquare/COLS][promotionSquare%COLS] = player? BB:WB; break;
    case MOVE_PROMOTION_KNIGHT: board[promotionSquare/COLS][promotionSquare%COLS] = player? BN:WN; break;
    default: return;
  }
  promotionSquare = -1;
  player = 1 - player;
  updateMoveList();
}

void Board::undoMove() {
  promotionSquare = -1;
  chosenSquare = -1;

  int i = history.size();
  if (i <= 0) return;

  int square1 = history[i-4];
  int square2 = history[i-3];

  int x1 = square1 / COLS;
  int y1 = square1 % COLS;
  int x2 = square2 / COLS;
  int y2 = square2 % COLS;
  int moveType = history[i-1];

  // Undo player
  player = 1 - player;

  // Undo move
  board[x1][y1] = board[x2][y2];
  board[x2][y2] = history[i-2]; //restore captured piece

  // delete history of the move
  history.pop_back();
  history.pop_back();
  history.pop_back();
  history.pop_back();

  // Undo special move
  switch(moveType) {
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
      if (y2 == 2) { //left castling
        board[x2][0] = board[x2][3];
        board[x2][3] = EMPTY;
      }
      else { //right castling
        board[x2][7] = board[x2][5];
        board[x2][5] = EMPTY;
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
  for (i = 0; i < 6; i++) {
    if (castlingFirstMove[i] > history.size()) castlingFirstMove[i] = 0;
  }
  updateMoveList();
}

////////////////////////////////////////////////////////////////////////////
//                         Legal move generation
////////////////////////////////////////////////////////////////////////////

void Board::updateMoveList() {
  moveList.clear();
  findPinAndCheck();

  // loop through all squares, and update current player's pieces in those squares
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      // if square is empty, or has opponent's piece, do not update
      if (board[i][j] == EMPTY || (player == (board[i][j] > MAX_BLACK_TYPE))) continue;

      // if square has friendly piece, update
      int pType = board[i][j] % NUM_PIECE_TYPES; // remove color factor
      switch (pType) {
        case BP: updatePawnMoves(i, j); break;
        case BR:
        case BB:
        case BQ: updateRayMoves(i, j); break;
        case BN: updateKnightMoves(i, j); break;
        case BK: updateKingMoves(i, j);
      }
    }
  }
}

void Board::findPinAndCheck() {
  // To find all pins and checks, this method do the following:
  // From the current player's king squares:
  // - Go in all 8 basic directions (vertical, horizontal, vertical) to find opponent's ray pieces.
  // - Go in all 8 knight directions to find opponent's knight.
  // - Check the two squares diagonally ahead of king to find opponent's pawn.
  // Save the opponent pieces found in pinPieces and checkingPieces.

  // Position of King
  int r = kingSquares[player] / COLS;
  int c = kingSquares[player] % COLS;

  // Position of the square we are checking
  int i,j;

  // Clear checking pieces and pin pieces
  checkingPieces[0] = -1;
  checkingPieces[1] = -1;
  int checkIndex = 0; //the current empty slot in checkingPieces
  pinPieces.clear();

  // Check 8 basic direction to see if there are ray pieces
  for(int d = 0; d < 8; d++) {
    int pinnedSquare = -1;
    i = r;
    j = c;
    while(true) {
      // advance 1 square in direction of d
      i += dir[d][0];
      j += dir[d][1];
      // if out of bound
      if (i < 0 || i >= ROWS || j < 0 || j >= COLS) break;
      // if empty square, advance 1 more square
      if (board[i][j] == EMPTY) continue;

      if (pinnedSquare == -1) { // encounter the first piece in ray
        if (player == (board[i][j] < MIN_WHITE_TYPE)) {
          // if the 1st piece is a friendly piece
          // it might be a pinned piece
          pinnedSquare = i * ROWS + j;
        }
        else {
          // if the 1st piece is am opponent's piece
          // check if it's a suitable ray piece
          int curPieceType = board[i][j] % NUM_PIECE_TYPES; // do not care about color
          if ( curPieceType == BQ
               ||(d <= MAX_CARDINAL_DIR && curPieceType == BR)
               || (d >= MIN_DIAGONAL_DIR && curPieceType == BB) ) {

            // if the piece is checking King, add it to checkingPieces array
            // if this is the first checking piece, put it in index 0
            // else put it in index 1.
            checkingPieces[checkIndex] = i * COLS + j;
            checkIndex++;
          }
          break; //already found an opponent's piece in this ray, so look into another direction.
        }
      } else {// encounter the second piece in ray (the first piece is a friendly piece)
        if ( player == (board[i][j] > MAX_BLACK_TYPE) ) {
          // if the 2nd piece is opponent's piece, it is a possible pinning piece
          int curPieceType = board[i][j] % NUM_PIECE_TYPES; // do not care about color
          if ( curPieceType == BQ
               || (d <= MAX_CARDINAL_DIR && curPieceType == BR)
               || (d >= MIN_DIAGONAL_DIR && curPieceType == BB) ) {
            // if it's opponent's ray piece, it is a pinning piece
            pinPieces.push_back(pinnedSquare);
            pinPieces.push_back(i* 8 + j);
          }
        }
        break; //already found 2 pieces in this ray, look into other directions
      }
    }
  }

  // if king is double checked, we have found all possible checks and pins
  if (checkIndex > 1) return;

  // Check 8 knight's direction to see if there are opponent's knight
  for (int d = 0; d < 8; d++) {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i >= ROWS || j < 0 || j >= COLS) continue;

    if (board[i][j] == (player? WN : BN) ) { // if has opponent's knight
      checkingPieces[checkIndex] = i * 8 + j;
      checkIndex++;
      // if there are 2 checking pieces, that's max
      if (checkIndex > 1) return;
    }
  }

  // Check 2 squares diagonally to the left and right to see if there are opponent pawns
  i = player? (r - 1) : (r + 1); //For black king, check the lower row for white pawn.
                                 //For white king, check the upper row for black pawn.
  j = c - 1; // The left column
  if (i >= 0 && i < ROWS && j >= 0 && j < COLS) {
    if (board[i][j] == (player? WP:BP)) {
      // has an opponent's pawn
      checkingPieces[checkIndex] = i * 8 + j;
      checkIndex++;
      if (checkIndex > 1) return; //no more possible check
    }
  }
  j = c + 1; // The right column
  if (i >= 0 && i < ROWS && j >= 0 && j < COLS) {
    if (board[i][j] == (player? WP:BP)) {
      // has an opponent's pawn
      checkingPieces[checkIndex] = i * 8 + j;
    }
  }
}

void Board::updateKingMoves(int r, int c) {
  // the square we are checking
  int i, j;

  //
  // 8 squares around king
  //

  for (int d = 0; d < 8; d++) {
    i = r + dir[d][0];
    j = c + dir[d][1];
    // out of bound
    if (i < 0 || i >= ROWS || j < 0 || j >= COLS) continue;
    // square has a friendly piece
    if ((board[i][j] != EMPTY) && (player == (board[i][j] < MIN_WHITE_TYPE))) continue;
    // square is controlled by opponent
    if (isSquareControlled(i, j)) continue;

    // if the king is between the considered square and opponent's ray piece, king cannot move to that square
    if (checkingPieces[0] != -1
        && isInRay(checkingPieces[0], kingSquares[player], i * COLS + j) ) continue;
    if (checkingPieces[1] != -1
        && isInRay(checkingPieces[1], kingSquares[player], i * COLS + j) ) continue;

    // If passes all tests above, the square is a legal king move. Add to moveList
    moveList.push_back(kingSquares[player]); //starting square
    moveList.push_back(i * COLS + j); //ending square
    moveList.push_back(MOVE_NORMAL); // move type
  }

  //
  // Castling
  //

  if (!castlingFirstMove[player] && checkingPieces[0] == -1) {//king has not moved and is not checked
    // if left rook has not moved,
    // and the squares left of king are empty and not controlled by the opponent,
    // can castling left
    if ( !castlingFirstMove[player + 2] && (board[r][1] == EMPTY) && (board[r][2] == EMPTY) && (board[r][3] == EMPTY)
         && !isSquareControlled(r, 2) && !isSquareControlled(r, 3) ) {
      moveList.push_back(kingSquares[player]); //starting square
      moveList.push_back(r * COLS + 2); //ending square
      moveList.push_back(MOVE_CASTLING); // move type
    }
    // if right rook has not moved,
    // and the 2 squares right of king are empty and not controlled by the opponent,
    // can castling right
    if ( !castlingFirstMove[player + 4] && (board[r][5] == -1) && (board[r][6] == -1)
         && !isSquareControlled(r, 5) && !isSquareControlled(r, 6) ) {
      moveList.push_back(kingSquares[player]); //starting square
      moveList.push_back(r * COLS + 6); //ending square
      moveList.push_back(MOVE_CASTLING); // move type
    }
  }
}

void Board::updateRayMoves(int r, int c) {
  // Find out whether the king is checked or this ray piece is pinned
  // If there are 2 pieces that are either checking kings or pinning this piece,
  // then piece cannot move

  if (checkingPieces[1] != -1) return; // king is double checked

  int checkingSquare = -1; //the square of the piece that is checking the king or pinning the ray piece
  int raySquare = r * COLS + c; //the square of the ray piece

  if (checkingPieces[0] != -1) checkingSquare = checkingPieces[0]; //king is checked

  for (int i = 0; i < pinPieces.size(); i += 2) {
    if (pinPieces[i] == raySquare) { // this ray piece is pinned
      //if the ray piece is pinned and king is checked, piece cannot move
      if (checkingSquare != -1) return;

      checkingSquare = pinPieces[i+1];
      break;
    }
  }

  // Update this ray piece's move
  // If there is a piece checking king or pinning ray,
  // this piece can only move between the checking piece and the king

  int i,j; // the square we are considering

  // choose directions (queen: 8 dirs, rook: first 4 dirs, bishop: 4 last dirs)
  int minDir, maxDir;
  switch(board[r][c]) {
    case WR: case BR:
      minDir = 0; maxDir = 4; break;
    case WB: case BB:
      minDir = 4; maxDir = 8; break;
    case WQ: case BQ:
      minDir = 0; maxDir = 8; break;
  }
  // Look into all directions
  for(int d = minDir; d < maxDir; d++) {
    i = r;
    j = c;
    while(true) {
      // advance 1 square in the direction d
      i += dir[d][0];
      j += dir[d][1];

      // if out of bound, look in another direction
      if (i < 0 || i >= ROWS || j < 0 || j >= COLS) break;
      // if square has friendly piece, look in another direction
      if ((board[i][j] != EMPTY) && (player == (board[i][j] < MIN_WHITE_TYPE))) break;

      // if moving causes king danger:
      // + if current square is empty, advance 1 more square
      // + if current square has piece, check other directions
      if ( (checkingSquare != -1)
           && !isInRay(checkingSquare, i * COLS + j, kingSquares[player]) ) {
        if (board[i][j] == EMPTY) continue;
        break;
      }
      // At this point:
      // + King is not endangered if move
      // + Square is empty or has opponent's piece
      // This square a legal move. Add this move.
      moveList.push_back(raySquare);
      moveList.push_back(i * COLS + j);
      moveList.push_back(MOVE_NORMAL);
      // If square is not empty, look at other directions
      if (board[i][j] != EMPTY) break;
    }
  }
}

void Board::updateKnightMoves(int r, int c) {
  // Find out whether the king is checked or the knight is pinned
  // If there are 2 pieces that are checking king or this knight is pinned,
  // then knight cannot move

  if (checkingPieces[1] != -1) return; //king is double checked

  int knightSquare = r * COLS + c;
  for (int i = 0; i < pinPieces.size(); i += 2) {
    if (pinPieces[i] == knightSquare) return;
    // if knight is pinned by a ray piece, it cannot move (because it cannot return to the same ray)
  }

  // Update knight's move
  // If there is a piece checking king, knight can only move between the checking piece and the king

  int i,j; // The square we are considering

  //Look into 8 knight directions
  for (int d = 0; d < 8; d++) {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i >= ROWS || j < 0 || j >= COLS) continue;
    // if moving causes king danger (not going between king and the checking piece), check other directions
    if ((checkingPieces[0] != -1) && !isInRay(checkingPieces[0], i * COLS + j, kingSquares[player])) continue;

    if( board[i][j] == EMPTY || (player == (board[i][j] > MAX_BLACK_TYPE)) ) {//if square empty or has opponent, legal move
      moveList.push_back(knightSquare);
      moveList.push_back(i * 8 + j);
      moveList.push_back(MOVE_NORMAL);
    }
  }
}

void Board::updatePawnMoves(int r, int c) {
  // find out whether the king is checked or the pawn is pinned
  // if there are 2 pieces that are either checking king or pinning pawn,
  // then pawn cannot move

  if (checkingPieces[1] != -1) return; // 2 checking pieces

  int pawnSquare = r * 8 + c;
  int checkingSquare = -1; // the square of the piece that is checking king or pinning pawn

  checkingSquare = checkingPieces[0];

  for (int i = 0; i < pinPieces.size(); i += 2) {
    if (pinPieces[i] == pawnSquare) {
      if (checkingSquare != -1) return; //if both pawn is pinned and king is checked, pawn cannot move
      checkingSquare = pinPieces[i+1];
      break;
    }
  }

  // Update pawn's move
  // If there is a piece checking king or pinning pawn, pawn can only move between the checking piece and the kings

  int i,j; // the square we are considering
  int moveForward = player? -1: 1; // white pawn moves up, black pawn moves down

  bool canPromote = (r == (player? 1 : 6)); // is in the correct row for promotion
  bool canDoubleJump = (r == (player? 6 : 1)); // is in the correct row for double jump
  bool canEnPassant = (r == (player? 3 : 4))
                      && (history.size() != 0) && (history[history.size()-1] == MOVE_PAWN_DOUBLE_JUMP); // is in the correct row for en passant, and the last move is a double jump

  //
  // Move straight
  //

  // One square ahead
  i = r + moveForward;
  j = c;

  if (board[i][j] == EMPTY // empty square in front
      && ((checkingSquare == -1) || isInRay(checkingSquare, i * COLS + j, kingSquares[player]))) {// no king danger if move there
    // this pawn can jump 1 square forward
    if (canPromote) {
      // if can promote, add 4 moves (promote to queen, rook, knight, or bishop)
      int endSquare = i * COLS + j;
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
    } else { // cannot promte, just a normal move
      moveList.push_back(pawnSquare);
      moveList.push_back(i * COLS + j);
      moveList.push_back(MOVE_NORMAL);
    }
  } else {
    // if the square in front pawn is not empty, cannot double jump
    if (board[i][j] != EMPTY) canDoubleJump = false;
  }

  // Two squares ahead
  i += moveForward;
   //if in the correct row and the 1st square ahead is empty, and the 2nd square ahead is empty, can jump 2 squares ahead
  if (canDoubleJump && board[i][j] == EMPTY
      && ((checkingSquare == -1) || isInRay(checkingSquare, i * COLS + j, kingSquares[player]))) {// no king danger if moves
    moveList.push_back(pawnSquare);
    moveList.push_back(i * COLS + j);
    moveList.push_back(MOVE_PAWN_DOUBLE_JUMP);
  }

  //
  // Capture diagonally and en passant
  //
  i = r + moveForward;
  // check through the left and right column
  for (j = c - 1; j <= c+1; j += 2) {
    if (j < 0 || j >= COLS) continue; //out of bound

    if ((checkingSquare == -1) || isInRay(checkingSquare, i * COLS + j, kingSquares[player])) {//no king danger if moves
      if (canEnPassant && (j == history[history.size()-3] % COLS)) {// if in the correct row and correct col, can en passant
        moveList.push_back(pawnSquare);
        moveList.push_back(i * COLS + j);
        moveList.push_back(MOVE_PAWN_EN_PASSANT);
      } else if (board[i][j] != EMPTY && (player == (board[i][j] > MAX_BLACK_TYPE))) { // if has opponent, can capture
        if (canPromote) {
          // if can promote by capturing diagonally
          int endSquare = i * COLS + j;
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
        } else {
          moveList.push_back(pawnSquare);
          moveList.push_back(i * COLS + j);
          moveList.push_back(MOVE_NORMAL);
        }
      }
    } else { //king danger if move
      // if a double jumped pawn is checking king,
      // en passant although doesn't go between the checking pawn and the king
      // can still remove the checking pawn and thus remove the check

      if (canEnPassant //last move is a double jump, and this pawn is in the correct row for en passant
          && (history[history.size() - 3] == checkingSquare) //the double jumped pawn is the checking piece
          && (j == history[history.size()-3] % COLS)) { // this pawn is in the correct col
        moveList.push_back(pawnSquare);
        moveList.push_back(i * COLS + j);
        moveList.push_back(MOVE_PAWN_EN_PASSANT);
      }
    }
  }
}

bool Board::isSquareControlled(int r, int c) {
  int i,j;

  //
  // Check 8 knight's move
  //
  for (int d = 0; d < 8; d++) {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i >= ROWS || j < 0 || j >= COLS) continue;
    // has opponent's knight
    if (board[i][j] == (player? WN : BN)) return true;
  }

  //
  // Check 8 ray directions
  //
  for(int d = 0; d < 8; d++) {
    i = r;
    j = c;
    while(true) {
      // advance 1 square in the direction d
      i += dir[d][0];
      j += dir[d][1];
      // if out of bound
      if (i < 0 || i > 7 || j < 0 || j > 7) break;
      // if empty square, advance 1 more square
      if (board[i][j] == -1) continue;
      // if it is opponent's piece
      if (player == (board[i][j] > MAX_BLACK_TYPE)) {
        // if it's opponent's ray piece
        int curPiece = board[i][j] % NUM_PIECE_TYPES; //remove color factor
        if (curPiece == BQ || (d < 4 && curPiece == BR) || (d > 3 && curPiece == BB)) return true;
      }
      break;
    }
  }

  //
  // Check pawn's control
  //
  i = player? (r - 1) : (r + 1);
  j = c - 1; // left column
  if (i >= 0 && i < ROWS && j >= 0 && j < COLS) {
    if (board[i][j] == (player? WP : BP)) return true;
  }
  j = c + 1; // right column
  if (i >= 0 && i < ROWS && j >= 0 && j < COLS)
  {
    if (board[i][j] == (player? WP : BP)) return true;
  }

  //
  // Check king's control
  //
  for (int d = 0; d < 8; d++) {
    i = r + dir[d][0];
    j = c + dir[d][1];
    if (i >= 0 && i < ROWS && j >= 0 && j < COLS) {
      if (board[i][j] == (player? WK : BK)) return true;
    }
  }

  // if pass all tests, square is not controlled by opponent
  return false;
}

bool Board::isInRay(int s1, int s2, int s3) {
  int x1 = s1 / COLS;
  int y1 = s1 % COLS;
  int x2 = s2 / COLS;
  int y2 = s2 % COLS;
  int x3 = s3 / COLS;
  int y3 = s3 % COLS;

  return ((x1 - x2) * (y1 - y3) == (x1 - x3) * (y1 - y2)) // 3 squares are in a line
         && ((x2 >= x1 && x2 <= x3) || (x2 <= x1 && x2 >= x3)) // square 2's row is in the middle of square 1 and square 2
         && ((y2 >= y1 && y2 <= y3) || (y2 <= y1 && y2 >= y3)); // square 2's col is in the middle of square 1 and square 2
}


////////////////////////////////////////////////////////////////////////////
//                                Debug
////////////////////////////////////////////////////////////////////////////

bool Board::isDifferent(Board& b) {
  bool rv = false; //return value
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

  if (rv) {
    for (int i = 0; i < history.size(); i+= 4) {
      printHistory(i/4); printf("\n");
    }
  }

  return rv;
}

void Board::printBoard(int line) {
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
