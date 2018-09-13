#include <stdio.h>

#include "StartGUI.h"

const int StartGUI::singleButtonX = 254;
const int StartGUI::singleButtonY = 134;
const int StartGUI::multiButtonX = 254;
const int StartGUI::multiButtonY = 268;
const int StartGUI::quitButtonX = 254;
const int StartGUI::quitButtonY = 400;
const float StartGUI::speed = 2;

StartGUI::StartGUI(SDL_Renderer* renderer) {
  ///////////////////////////////////////////////////////////////////////
  //                               Visual
  ///////////////////////////////////////////////////////////////////////
  singleButton.loadFromFile(renderer, "img/startGUI/single-player.png");
  multiButton.loadFromFile(renderer, "img/startGUI/multi-player.png");
  quitButton.loadFromFile(renderer, "img/startGUI/quit.png");
  background.loadFromFile(renderer, "img/startGUI/background.jpg");

  boxes.push_back(Box(singleButtonX, singleButtonY, singleButtonX + 292, singleButtonY + 94, INPUT_SINGLE_PLAYER));
  boxes.push_back(Box(multiButtonX, multiButtonY, multiButtonX + 292, multiButtonY + 94, INPUT_MULTI_PLAYER));
  boxes.push_back(Box(quitButtonX, quitButtonY, quitButtonX + 292, quitButtonY + 94, INPUT_QUIT));

  ///////////////////////////////////////////////////////////////////////
  //                               Audio
  ///////////////////////////////////////////////////////////////////////
  startbmg = Mix_LoadMUS("sound/start bgm.mp3");
  if (!startbmg) {
    printf("Failed to load music; %s\n", Mix_GetError());
  }
}

StartGUI::~StartGUI() {}

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

void StartGUI::playMusic() {
  Mix_PlayMusic(startbmg, -1);
}

void StartGUI::stopMusic() {
  Mix_HaltMusic();
}

void StartGUI::destroyMedia() {
  Mix_FreeMusic(startbmg);
  startbmg = NULL;
}
