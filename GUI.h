/***********************************************************************//**
 * Abstract class declare an array of "box" (or click zones).
 * Each box has a distinct value.
 * Has method to get user's click and return the value of the box clicked
 * Derive this class to make different guis
 ***************************************************************************/

#ifndef GUI_H
#define GUI_H

#include <SDL.h>

class GUI
{
  protected:
    /**
     * A box is a rectangular region with a value
     * A click in a box return the specific value of that box
     */
    class Box {
      public:
        int x1, y1, x2, y2;
        int boxValue;

        /**
         * Create a box
         * @param x1, y1, x2, y2 bounds of the box
         * @param value the value of the box. Value of 0 will causes undefined behavior.
         */
        Box() {};
        Box(int x1, int y1, int x2, int y2, int value);

        /**
         * Check if a point is inside the box
         * @param x, y coordinates of the given point
         */
        bool contains(int x, int y);
    };
  public:
    GUI();
    ~GUI();

    /**
     * Quit flag
     */
    static bool quit;

    /**
     * Set the box array of GUI
     * @param boxes[] an array of boxes to be used
     * @param size the size of the array
     */
    void setBoxes(Box* box, int size);

    /**
     * Get user's click
     * @return value of the clicked box
     *         0 if there's no click or if user quits
     */
    int getInput();

    /**
     * Draw the GUI on the screen
     */
    virtual void draw(SDL_Renderer* renderer) = 0;

  private:
    Box* boxes; /**< The array of boxes to get input from */
    int numBox; /**< The number of boxes */
};

#endif // GUI_H
