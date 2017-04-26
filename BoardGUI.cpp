#include "BoardGUI.h"
#include <TextureWrapper.h>
#include <PieceType.h>

#include <stdio.h>

BoardGUI::BoardGUI(Board* brd)
{
  this->b = brd;
  Box* boxArr = new Box[64];
  for(int i = 0; i < 64; i++) {
    int x = 54 + (i%8) * 590 / 8;
    int y = 55 + (7- i/8) * 590 / 8;
    boxArr[i] = {x, y , x+73, y+73, i+1};
  }
  setBoxes(boxArr, 64);
}

void BoardGUI::initGUI(SDL_Renderer* renderer)
{
  boardImg.loadFromFile(renderer, "img/board.jpg");
  //if (!loadBoard) printf("null chessboared");
  piecesSprite.loadFromFile(renderer, "img/pieces.png");
  //if(!loadPiece) printf("null piece");
  piecesSprite.setBlendMode(SDL_BLENDMODE_BLEND);

  //position of piece's image on sprite sheet
  for(int i = 0; i < 12; i++) {
    pieceClips[i] = { (i%6)*60, (i/6)*60, 60, 60 };
  }
  // positions of squares in chessboard
  for(int i = 0; i < 64; i++) {
    int x = 54 + (i%8) * 590 / 8;
    int y = 55 + (7 - i/8) * 590 / 8;
    boardSquares[i] = {x, y ,73, 73};
  }

}

void BoardGUI::draw(SDL_Renderer* renderer)
{
  printf("GUI drawing\n");
  // Clear screen with white color
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( renderer );

  // Draw chessboard
  boardImg.render(renderer, 0, 0, NULL, NULL);

  // Draw click boxes
  //SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
  //SDL_RenderDrawRects(renderer, boardSquares, 64);

  // Draw all chess pieces from board
  for (int i = 0; i < 64; i++)
  {
    int piece = b->getPiece(i);
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
  SDL_RenderPresent(renderer);
  if (renderer == NULL)
  {
    printf("gui draw null renderer\n");
  }
}
