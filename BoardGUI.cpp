#include "BoardGUI.h"
#include <PieceType.h>

#include <stdio.h>



BoardGUI::BoardGUI(Board* brd, SDL_Renderer* renderer)
{
  /*
   * Set an array of buttons so that GUI can get input
   */
  this->b = brd;
  Box* boxArr = new Box[64];
  for(int i = 0; i < 64; i++) {
    int x = 48 + (i%8) * 507 / 8;
    int y = 50 + (7- i/8) * 507 / 8;
    boxArr[i] = {x, y , x+60, y+60, i+1};
  }
  setBoxes(boxArr, 64);

  /*
   * Init textures and rects
   */
  boardImg.loadFromFile(renderer, "img/boardGUI/board.jpg");
  piecesSprite.loadFromFile(renderer, "img/boardGUI/pieces.png");
  // piece can be made transparent
  piecesSprite.setBlendMode(SDL_BLENDMODE_BLEND);
  moveArrow.loadFromFile(renderer, "img/boardGUI/arrow.png");

  undoButton.loadFromFile(renderer, "img/boardGUI/undoButton.png");
  homeButton.loadFromFile(renderer, "img/boardGUI/homeButton.png");

  playerTxt.loadFromFile(renderer, "img/boardGUI/Player.png");
  comTxt.loadFromFile(renderer, "img/boardGUI/COM.png");
  promoteTxt.loadFromFile(renderer, "img/boardGUI/promotionTxt.png");

  colorSymbols[0].loadFromFile(renderer, "img/boardGUI/whiteSymbol.png");
  colorSymbols[1].loadFromFile(renderer, "img/boardGUI/blackSymbol.png");



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
  promotePieceRects[0] = {617, 350, 50, 50};
  promotePieceRects[1] = {617, 411, 50, 50};
  promotePieceRects[2] = {617, 475, 50, 50};
  promotePieceRects[3] = {617, 536, 50, 50};
}

void BoardGUI::draw(SDL_Renderer* renderer)
{
  // Clear screen with white color
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( renderer );

  // Draw chessboard
  boardImg.render(renderer, 0, 0);

  // Draw click boxes
  //SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
  //SDL_RenderDrawRects(renderer, boardSquares, 64);

  // Draw all chess pieces from board
  for (int i = 0; i < 64; i++)
  {
    int piece = b->getPieceGUI(i);
    if (piece >= 0 && piece < 12)
    {
      // if piece is not chosen, draw as normal
      piecesSprite.render(renderer, &pieceClips[piece], &boardSquares[i]);
    }
    else if (piece >= 12)
    {
      // if piece is chosen, draw it half transparent
      piecesSprite.setTransparency(ALPHA_FADED);
      piecesSprite.render(renderer, &pieceClips[piece-12], &boardSquares[i]);
      piecesSprite.setTransparency(ALPHA_NORMAL);
    }
  }

  /*******************************************************************************
   * Side Bar
   *******************************************************************************/
  undoButton.render(renderer, NULL, &undoRect);
  homeButton.render(renderer, NULL, &homeRect);
  playerTxt.render(renderer, NULL, &playerTxtRect);
  colorSymbols[1].render(renderer, NULL, &colorSymbolRect);
  promoteTxt.render(renderer, NULL, &promoteTxtRect);


  SDL_RenderPresent(renderer);
}
