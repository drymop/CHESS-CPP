#ifndef STARTGUI_H
#define STARTGUI_H

#include "GUI.h"
#include "TextureWrapper.h"
#include <SDL2/SDL_mixer.h>


class StartGUI: public GUI {
  public:
    enum inputs{
      INPUT_SINGLE_PLAYER = 1,
      INPUT_MULTI_PLAYER,
      INPUT_QUIT,
    };

    StartGUI(SDL_Renderer* renderer);
    ~StartGUI();

    void draw(SDL_Renderer* renderer);

    void playMusic();
    void stopMusic();

    void destroyMedia();

  private:
    /************************************************************
     *                        Images
     ************************************************************/

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
    static const float speed; /**< The speed of the open animation */

    void openAnimation(SDL_Renderer* renderer);

    /************************************************************
     *                        Audio
     ************************************************************/
    Mix_Music* startbmg;
};

#endif // STARTGUI_H
