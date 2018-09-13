#include <stdio.h>

#include "EndGUI.h"

const int EndGUI::textX = 196;
const int EndGUI::textY = 105;
const int EndGUI::buttonX = 252;
const int EndGUI::buttonY = 450;

EndGUI::EndGUI(SDL_Renderer* renderer)
{
  youWinTxt.loadFromFile(renderer, "img/endGUI/youwintxt.png");
  youLoseTxt.loadFromFile(renderer, "img/endGUI/youlosetxt.png");
  whiteWinTxt.loadFromFile(renderer, "img/endGUI/whitewintxt.png");
  blackWinTxt.loadFromFile(renderer, "img/endGUI/blackwintxt.png");
  drawTxt.loadFromFile(renderer, "img/endGUI/drawtxt.png");
  menuButton.loadFromFile(renderer, "img/endGUI/backtomenu.png");
  backgroundWin.loadFromFile(renderer, "img/endGUI/backgroundwin.jpg");
  backgroundLose.loadFromFile(renderer, "img/endGUI/backgroundlose.jpg");

  boxes.push_back( Box(buttonX, buttonY, buttonX + 292, buttonY + 94, 1) );

  // load audio
  winSFX = Mix_LoadWAV("sound/victory sound.wav");
  if (!winSFX) {
    printf("Egui1 Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
  }
  failSFX = Mix_LoadWAV("sound/fail sound.wav");
  if (!failSFX) {
    printf("Egui2 Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
  }
}

void EndGUI::setPlayer(int p)
{
  player = p;
}

void EndGUI::setWinner(int w)
{
  winner = w;
}

void EndGUI::draw(SDL_Renderer* renderer)
{
  if(winner == Board::BOTH_COLOR){
    backgroundLose.render(renderer, NULL, NULL);
    drawTxt.render(renderer, textX, textY, NULL, NULL);
    menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
    Mix_PlayChannel(-1, failSFX, 0);
  }
  else{
    if(player == Board::BOTH_COLOR){
      if(winner == Board::WHITE){
        backgroundWin.render(renderer, NULL, NULL);
        whiteWinTxt.render(renderer, textX, textY, NULL, NULL);
        menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
      }
      else {
        backgroundWin.render(renderer, NULL, NULL);
        blackWinTxt.render(renderer, textX, textY, NULL, NULL);
        menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
      }
      Mix_PlayChannel(-1, winSFX, 0);
    } else if((player == Board::WHITE) == (winner == Board::WHITE)){
      backgroundWin.render(renderer, NULL, NULL);
      youWinTxt.render(renderer, textX, textY, NULL, NULL);
      menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
      Mix_PlayChannel(-1, winSFX, 0);
    } else {
      backgroundLose.render(renderer, NULL, NULL);
      youLoseTxt.render(renderer, textX, textY, NULL, NULL);
      menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
      Mix_PlayChannel(-1, failSFX, 0);
    }
  }
  SDL_RenderPresent(renderer);
}

void EndGUI::destroyMedia() {
  Mix_FreeChunk(winSFX);
  Mix_FreeChunk(failSFX);
}
