/***********************************************************************//**
 * Abstract class declare an array of "box" (or click zones).
 * Each box has a distinct value.
 * Has method to get user's click and return the value of the box clicked
 * Derive this class to make different guis
 ***************************************************************************/

#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <vector>

class GUI
{
  protected:
    /**********************************************************//**
     * A rectangular region with a specific value assigned to it.
     * Used to create buttons in GUI.
     **************************************************************/
    class Box {
      public:
        int x1, y1, x2, y2;
        int boxValue;

        /**
         * Create a box
         * @param x1, y1, x2, y2 bounds of the box
         * @param value the value of the box. Value of 0 will causes undefined behavior.
         */
        Box(int x1, int y1, int x2, int y2, int value);

        /**
         * Check if a point is inside the box
         * @param x, y coordinates of the given point
         */
        bool contains(int x, int y);
    };

    std::vector<Box> boxes; /**< The array of boxes to get input from */

  public:
    GUI();
    virtual ~GUI();

    /**
     * Quit flag
     */
    static bool quit;

    /**
     * Get user's click.
     * If user quits, set GUI::quit to true.
     * @return value of the clicked box
     *         0 if there's no click or if user quits
     */
    int getInput();

    /**
     * Draw the GUI on the screen
     */
    virtual void draw(SDL_Renderer* renderer) = 0;
};

#endif // GUI_H
