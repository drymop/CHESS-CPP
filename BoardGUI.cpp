#include "BoardGUI.h"

#include <stdio.h>


BoardGUI::BoardGUI(Board* brd, SDL_Renderer* renderer) {
  this->b = brd;

  ////////////////////////////////////////////////////////////////////////////
  //   Set an array of boxes so that GUI can return clicks on these boxes
  ////////////////////////////////////////////////////////////////////////////

  // buttons for 64 squares of chess board
  for(int i = INPUT_MIN_SQUARE - 1; i < INPUT_MAX_SQUARE; i++) {
    // coordinate of the top left corner of the square
    int x = 48 + (i%8) * 507 / 8;
    int y = 50 + (7- i/8) * 507 / 8;
    boxes.push_back( Box(x, y , x+60, y+60, i+1) );
  }
  // buttons on the side bar
  boxes.push_back( Box(626, 18, 682, 87, INPUT_UNDO) );//undo button
  boxes.push_back( Box(721, 18, 782, 87, INPUT_HOME) );//home button
  boxes.push_back( Box(611, 342, 790, 392, INPUT_PROMOTE_QUEEN) );//queen promote
  boxes.push_back( Box(611, 403, 790, 453, INPUT_PROMOTE_ROOK) );//rook promote
  boxes.push_back( Box(611, 465, 790, 515, INPUT_PROMOTE_BISHOP) );//bishop promote
  boxes.push_back( Box(611, 528, 790, 578, INPUT_PROMOTE_KNIGHT) );//knight promote
  // pass the array to GUI
  //setBoxes(boxArr, 70);

  /////////////////////////////////////////////////////////////////////////////
  //                 Load media and initialize position
  /////////////////////////////////////////////////////////////////////////////
  boardImg.loadFromFile(renderer, "img/boardGUI/board.jpg");
  piecesSprite.loadFromFile(renderer, "img/boardGUI/pieces.png");
  piecesSprite.setBlendMode(SDL_BLENDMODE_BLEND); // piece can be made transparent
  moveArrow.loadFromFile(renderer, "img/boardGUI/arrow.png");

  undoButton.loadFromFile(renderer, "img/boardGUI/undoButton.png");
  homeButton.loadFromFile(renderer, "img/boardGUI/homeButton.png");

  playerTxt.loadFromFile(renderer, "img/boardGUI/Player.png");
  comTxt.loadFromFile(renderer, "img/boardGUI/COM.png");
  promoteTxt.loadFromFile(renderer, "img/boardGUI/promotionTxt.png");

  colorSymbols[0].loadFromFile(renderer, "img/boardGUI/whiteSymbol.png");
  colorSymbols[1].loadFromFile(renderer, "img/boardGUI/blackSymbol.png");

  crosshair.loadFromFile(renderer, "img/boardGUI/crosshair.png");

  //position of piece's image on sprite sheet
  for(int i = 0; i < 12; i++) {
    pieceClips[i] = { (i%6)*60, (i/6)*60, 60, 60 };
  }
  // positions of squares in chessboard
  for(int i = 0; i < 64; i++) {
    int x = 48 + (i%8) * 507 / 8;
    int y = 50 + (7 - i/8) * 507 / 8;
    boardSquares[i] = {x, y ,60, 60};
  }

  undoRect = {626, 18, 56, 69};
  homeRect = {721, 18, 61, 58};
  playerTxtRect = {633, 106, 141, 52};
  colorSymbolRect = {620, 175, 164, 88};
  promoteTxtRect = {613, 290, 172, 290};
  promotePieceRects[0] = {617, 342, 50, 50};
  promotePieceRects[1] = {617, 403, 50, 50};
  promotePieceRects[2] = {617, 465, 50, 50};
  promotePieceRects[3] = {617, 528, 50, 50};
}

void BoardGUI::updateMovePointers() {
  availableMoves.clear();
  int chosenSquare = b->getChosenSquare();
  if (chosenSquare >= 0 && chosenSquare < Board::NUM_SQUARES) {
    b->getMovesFromSquare(availableMoves, chosenSquare);
  }
}

void BoardGUI::setPlayer(int color) {
  humanSide = color;
}

void BoardGUI::draw(SDL_Renderer* renderer) {
  // Clear screen
  SDL_RenderClear( renderer );

  // Draw chessboard
  boardImg.render(renderer, 0, 0);

  // Draw click boxes
  //SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
  //SDL_RenderDrawRects(renderer, boardSquares, 64);

  int kingSquare = b->getKingSquare(b->getPlayer());

  // Draw all chess pieces from board
  for (int i = 0; i < Board::NUM_SQUARES; i++) {
    int piece = b->getPieceGUI(i);
    if (piece == Board::EMPTY) continue;
    if (piece < Board::NUM_COLORED_TYPES) {
      // if piece is not chosen, draw as normal
      piecesSprite.render(renderer, &pieceClips[piece], &boardSquares[i]);
    } else {
      // if piece is chosen, draw it half transparent
      piece -= Board::NUM_COLORED_TYPES;
      piecesSprite.setTransparency(ALPHA_FADED);
      piecesSprite.render(renderer, &pieceClips[piece], &boardSquares[i]);
      piecesSprite.setTransparency(ALPHA_NORMAL);
    }
    if (i == kingSquare) {
      SDL_Rect rect = {boardSquares[i].x - 10, boardSquares[i].y - 10, 80, 80};
      crosshair.render(renderer, NULL, &rect);
    }
  }

  // Draw side Bar
  undoButton.render(renderer, NULL, &undoRect);
  homeButton.render(renderer, NULL, &homeRect);
  if (humanSide == Board::BOTH_COLOR || humanSide == b->getPlayer()) {
    playerTxt.render(renderer, NULL, &playerTxtRect);
  } else {
    comTxt.render(renderer, NULL, &playerTxtRect);
  }
  colorSymbols[b->getPlayer()].render(renderer, NULL, &colorSymbolRect);

  // Draw move pointers
  for (int i = 0; i < availableMoves.size(); i++) {
    int arrowX = boardSquares[availableMoves[i]].x;
    int arrowY = boardSquares[availableMoves[i]].y;
    arrowY -= arrowHeight;
    moveArrow.render(renderer, arrowX, arrowY);
  }
  arrowHeight += arrowSpeed;
  if (arrowHeight > ARROW_HIGH || arrowHeight < ARROW_LOW) arrowSpeed = -arrowSpeed;

  // Draw promotion panel
  if (b->hasPromotion()) {
    // Draw promotion options
    promoteTxt.render(renderer, NULL, &promoteTxtRect);
    int color = b->getPlayer()? 0 : 6;
    piecesSprite.render(renderer, &pieceClips[Board::BQ + color], &promotePieceRects[0]);
    piecesSprite.render(renderer, &pieceClips[Board::BR + color], &promotePieceRects[1]);
    piecesSprite.render(renderer, &pieceClips[Board::BB + color], &promotePieceRects[2]);
    piecesSprite.render(renderer, &pieceClips[Board::BN + color], &promotePieceRects[3]);
  }


  SDL_RenderPresent(renderer);
}
