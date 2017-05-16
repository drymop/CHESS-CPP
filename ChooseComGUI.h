#ifndef CHOOSECOMGUI_H
#define CHOOSECOMGUI_H
#include "GUI.h"
#include "TextureWrapper.h"

class ChooseComGUI: public GUI
{
  public:

    enum inputs {
      INPUT_WHITE_EASY = 1,
      INPUT_WHITE_MEDIUM,
      INPUT_WHITE_HARD,
      INPUT_BLACK_EASY,
      INPUT_BLACK_MEDIUM,
      INPUT_BLACK_HARD,
    };



    ChooseComGUI(SDL_Renderer* renderer);


    void draw(SDL_Renderer* renderer);

    void openingAnimation(SDL_Renderer* renderer);

    void endingAnimation(SDL_Renderer* renderer);


  private:
    TextureWrapper leftSide, rightSide, bothSide;
    TextureWrapper background;

    SDL_Rect leftSideRect, rightSideRect;
    const float MOVING_SPEED = 4;




};

#endif // CHOOSECOMGUI_H
