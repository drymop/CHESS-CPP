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

  Box* boxArr = new Box[1];
  boxArr[0] = {buttonX, buttonY, buttonX + 292, buttonY + 94, 1};
  setBoxes(boxArr, 1);
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
  if(winner == 2){
    backgroundLose.render(renderer, NULL, NULL);
    drawTxt.render(renderer, textX, textY, NULL, NULL);
    menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
  }
  else{
    if(player == 2){
      if(winner == 0){
        backgroundWin.render(renderer, NULL, NULL);
        whiteWinTxt.render(renderer, textX, textY, NULL, NULL);
        menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
      }
      else if(winner == 1){
        backgroundWin.render(renderer, NULL, NULL);
        blackWinTxt.render(renderer, textX, textY, NULL, NULL);
        menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
      }
    }
    else if((player == 0) == (winner == 0)){
        backgroundWin.render(renderer, NULL, NULL);
        youWinTxt.render(renderer, textX, textY, NULL, NULL);
        menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
    }
    else{
        backgroundLose.render(renderer, NULL, NULL);
        youLoseTxt.render(renderer, textX, textY, NULL, NULL);
        menuButton.render(renderer, buttonX, buttonY, NULL, NULL);
    }
  }
  SDL_RenderPresent(renderer);
}
