#ifndef STARTGUI_H
#define STARTGUI_H
#include "GUI.h"
#include "TextureWrapper.h"
class StartGUI: public GUI
{
  public:
    enum inputs{
      INPUT_SINGLE_PLAYER = 1,
      INPUT_MULTI_PLAYER,
      INPUT_QUIT,
    };


    StartGUI(SDL_Renderer* renderer);

    void draw(SDL_Renderer* renderer);

  private:
    TextureWrapper singleButton;
    TextureWrapper multiButton;
    TextureWrapper quitButton;
    TextureWrapper background;

    static const int singleButtonX;
    static const int singleButtonY;
    static const int multiButtonX;
    static const int multiButtonY;
    static const int quitButtonX;
    static const int quitButtonY;
    static const float speed;

    void openAnimation(SDL_Renderer* renderer);

};

#endif // STARTGUI_H
