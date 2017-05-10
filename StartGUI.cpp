#include "StartGUI.h"

const int StartGUI::singleButtonX = 254;
const int StartGUI::singleButtonY = 134;
const int StartGUI::multiButtonX = 254;
const int StartGUI::multiButtonY = 268;
const int StartGUI::quitButtonX = 254;
const int StartGUI::quitButtonY = 400;
const float StartGUI::speed = 2;

StartGUI::StartGUI(SDL_Renderer* renderer)
{
  singleButton.loadFromFile(renderer, "img/startGUI/single-player.png");
  multiButton.loadFromFile(renderer, "img/startGUI/multi-player.png");
  quitButton.loadFromFile(renderer, "img/startGUI/quit.png");
  background.loadFromFile(renderer, "img/startGUI/background.jpg");

  Box* boxArr = new Box[3];
  boxArr[0] = {singleButtonX, singleButtonY, singleButtonX + 292, singleButtonY + 94, 1};
  boxArr[1] = {multiButtonX, multiButtonY, multiButtonX + 292, multiButtonY + 94, 2};
  boxArr[2] = {quitButtonX, quitButtonY, quitButtonX + 292, quitButtonY + 94, 3};

  setBoxes(boxArr,3);
}

void StartGUI::draw(SDL_Renderer* renderer)
{
  openAnimation(renderer);
  background.render(renderer, NULL, NULL);
  singleButton.render(renderer, singleButtonX, singleButtonY, NULL, NULL);
  multiButton.render(renderer, multiButtonX, multiButtonY, NULL, NULL);
  quitButton.render(renderer, quitButtonX, quitButtonY, NULL, NULL);

  SDL_RenderPresent(renderer);
}

void StartGUI::openAnimation(SDL_Renderer* renderer)
{
  float sBX = -292;
  float mBX = 800;
  float qBX = -292;
  while(true)
  {
    background.render(renderer, NULL, NULL);
    singleButton.render(renderer, sBX, singleButtonY, NULL, NULL);
    multiButton.render(renderer, mBX, multiButtonY, NULL, NULL);
    quitButton.render(renderer, qBX, quitButtonY, NULL, NULL);
    SDL_RenderPresent(renderer);
    sBX += speed;
    mBX -= speed;
    qBX += speed;
    if (sBX > singleButtonX)
    {
      break;
    }
  }
}

