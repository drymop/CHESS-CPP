#ifndef ENDGUI_H
#define ENDGUI_H

#include <SDL2/SDL_mixer.h>

#include "Board.h"
#include "GUI.h"
#include "TextureWrapper.h"

class EndGUI: public GUI {
  public:
    EndGUI(SDL_Renderer* renderer);

    void draw(SDL_Renderer* renderer);

    void setPlayer(int p);
    void setWinner(int w);

    void destroyMedia();
  private:
    TextureWrapper backgroundWin, backgroundLose, menuButton;
    TextureWrapper youWinTxt, youLoseTxt, whiteWinTxt, blackWinTxt, drawTxt;

    static const int textX;
    static const int textY;
    static const int buttonX;
    static const int buttonY;

    /**
     * 0: white single player
     * 1 black single player
     * 2 2 players
     */
    int player;

    /**
     * 0: white wins
     * 1: black wins
     * 2: draw
     */
    int winner;

    Mix_Chunk* failSFX;
    Mix_Chunk* winSFX;
};

#endif // ENDGUI_H
