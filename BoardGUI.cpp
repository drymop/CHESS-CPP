#include "BoardGUI.h"
#include <TextureWrapper.h>

BoardGUI::BoardGUI()
{
  //Init boxes
  Box* boxArr = new Box[2] {Box(0,0,320,240,1), Box(320,240,640,480,2)};
  setBoxes(boxArr, 2);
}

void BoardGUI::initGUI(SDL_Renderer* renderer)
{
  bgImg.loadFromFile(renderer, "img/bg.jpg");
}

void BoardGUI::draw(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( renderer );

  bgImg.render(renderer, 0, 0);
  SDL_RenderPresent(renderer);
}
