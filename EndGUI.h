#ifndef ENDGUI_H
#define ENDGUI_H
#include "TextureWrapper.h"

class EndGUI
{
  public:
    EndGUI(SDL_Renderer* renderer);
    void draw(SDL_Renderer* renderer);
  private:
    TextureWrapper backgroundWin, backgroundLose;
    TextureWrapper youWinTxt, youLoseTxt, whiteWinTxt, blackWinTxt;

    static const int textX;
    static const int textY;
    static const int buttonX;
    static const int buttonY;



};

#endif // ENDGUI_H
