#include "ChooseComGUI.h"

ChooseComGUI::ChooseComGUI(SDL_Renderer* renderer){
  leftSide.loadFromFile(renderer,"img/choosePlayerGUI/whiteCOM.png");
  rightSide.loadFromFile(renderer,"img/choosePlayerGUI/blackCOM.png");
  bothSide.loadFromFile(renderer,"img/choosePlayerGUI/both.png");
  background.loadFromFile(renderer,"img/choosePlayerGUI/background.jpg");

  Box* boxArr = new Box[6];
  boxArr[0] = {102, 354, 282, 409, INPUT_WHITE_EASY};
  boxArr[1] = {102, 422, 282, 480, INPUT_WHITE_MEDIUM};
  boxArr[2] = {102, 493, 282, 551, INPUT_WHITE_HARD};
  boxArr[3] = {506, 110, 686, 168, INPUT_BLACK_EASY};
  boxArr[4] = {506, 179, 686, 237, INPUT_BLACK_MEDIUM};
  boxArr[5] = {506, 247, 686, 305, INPUT_BLACK_HARD};

  setBoxes(boxArr,6);

}

void ChooseComGUI::draw(SDL_Renderer* renderer) {
  bothSide.render(renderer);
  SDL_RenderPresent(renderer);
}

void ChooseComGUI::openingAnimation(SDL_Renderer* renderer)
{
  leftSideRect = {-500, 0, 800, 600};
  rightSideRect = {500, 0, 800, 600};
  while(true){
    SDL_RenderClear(renderer);
    background.render(renderer);
    leftSide.render(renderer, NULL, &leftSideRect);
    rightSide.render(renderer, NULL, &rightSideRect);
    SDL_RenderPresent(renderer);
    leftSideRect.x += MOVING_SPEED;
    rightSideRect.x -= MOVING_SPEED;
    if (leftSideRect.x > 0) {
      break;
    }
  }
}

void ChooseComGUI::endingAnimation(SDL_Renderer* renderer){
  leftSideRect = {0, 0, 800, 600};
  rightSideRect = {0, 0, 800, 600};
  while(true){
    SDL_RenderClear(renderer);
    background.render(renderer);
    leftSide.render(renderer, NULL, &leftSideRect);
    rightSide.render(renderer, NULL, &rightSideRect);
    SDL_RenderPresent(renderer);
    leftSideRect.x -= MOVING_SPEED;
    rightSideRect.x += MOVING_SPEED;
    if (leftSideRect.x < -500) {
      break;
    }
  }
}
