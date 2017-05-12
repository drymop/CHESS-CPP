/***********************************************************************//**
 * Draw the chessboard with side bar during the game,
 * and get input from user during game play.
 ***************************************************************************/

#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <GUI.h>
#include <TextureWrapper.h>
#include <Board.h>


class BoardGUI : public GUI
{
  public:

    /***************************************************************************
     * All input values returned by this gui's getInput method
     ***************************************************************************/

    static const int INPUT_MIN_SQUARE = 1; /**< The smallest value of a board square input, corresponds to square a1. */
    static const int INPUT_MAX_SQUARE = 64; /**< The smallest value of a board square input, corresponds to square h8. */
    static const int INPUT_UNDO = -1; /**< User chooses undo */
    static const int INPUT_HOME = -2; /**< User chooses to return to the start screen */
    static const int INPUT_PROMOTE_QUEEN = -3; /**< User promotes pawn to queen */
    static const int INPUT_PROMOTE_ROOK = -4; /**< User promotes pawn to rook */
    static const int INPUT_PROMOTE_BISHOP = -5; /**< User promotes pawn to bishop */
    static const int INPUT_PROMOTE_KNIGHT = -6; /**< User promotes pawn to knight */


    /***************************************************************************
     * Public methods
     ***************************************************************************/

    /**
     * Init the GUI.
     * @param brd: the pointer to the board that this GUI will be drawing.
     * @param renderer: the renderer to draw.
     */
    BoardGUI(Board* brd, SDL_Renderer* renderer);

    /**
     * Draw the chessboard on screen.
     * @param renderer: the renderer to draw.
     */
    void draw(SDL_Renderer* renderer);

    /**
     * Tell the GUI that user has chosen a new square.
     * Use to update move pointers
     */
    void updateMovePointers();
  private:
    Board* b; /**< Contains the board information. Used to draw the state of the board on screen */

    /****************************************************************************
     * Images used in displaying screen
     ****************************************************************************/

    TextureWrapper bgImg, boardImg, piecesSprite;
    TextureWrapper undoButton, undoButtonFaded, undoButtonGlow;
    TextureWrapper homeButton, homeButtonGlow;
    TextureWrapper playerTxt, comTxt, promoteTxt;
    TextureWrapper moveArrow;
    TextureWrapper colorSymbols[2];

    const Uint8 ALPHA_FADED = 100; /**< The alpha value to draw faded chess pieces */
    const Uint8 ALPHA_NORMAL = 255; /**<The alpha value to draw normal chess pieces */


    /****************************************************************************
     * Coordinates of all images
     ****************************************************************************/

    SDL_Rect pieceClips[12]; /**<Each piece is drawn by clipping a square from the piece sprite */
    SDL_Rect boardSquares[64]; /** Each SDL_Rect fits a square of the board. */
    SDL_Rect undoRect, homeRect, playerTxtRect, colorSymbolRect, promoteTxtRect;
    SDL_Rect promotePieceRects[4];

    /****************************************************************************
     * Move pointer animation
     ****************************************************************************/
    std::vector<int> availableMoves;
    const int ARROW_HIGH = 20;
    const int ARROW_LOW = 10;
    float arrowSpeed = 0.015;
    float arrowHeight = ARROW_LOW;
};
#endif // BOARDGUI_H
