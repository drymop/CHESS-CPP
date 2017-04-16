#ifndef BOARDGUI_H
#define BOARDGUI_H
#include <GUI.h>
#include <TextureWrapper.h>


class BoardGUI : public GUI
{
  public:
    BoardGUI();
    /**
     * Load images to use in game
     * @param renderer the renderer of the window to be drawn on
     */
    void initGUI(SDL_Renderer* renderer);

    /**
     * Use to satisfy GUI requirement only
     */
    void draw() {};

    /**
     * draw the chessboard screen
     * @param renderer the renderer of the window to be drawn on
     */
    void draw(SDL_Renderer* renderer);

  private:
    /**
     * The TextureWrapper contains images used in display
     * Background image, image of chessboard, image of chess pieces
     */
    TextureWrapper bgImg, boardImg, piecesSprite;
};

#endif // BOARDGUI_H
