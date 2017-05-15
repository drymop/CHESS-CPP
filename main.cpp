/******************************************************//**
 * Chess Game
 * @author lmtuan
 * @version 2.1
 **********************************************************/

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "AIPlayer.h"
#include "Board.h"
#include "BoardGUI.h"
#include "ChooseComGUI.h"
#include "EndGUI.h"
#include "GUI.h"
#include "HumanPlayer.h"
#include "Player.h"
#include "RandomPlayer.h"
#include "StartGUI.h"


/************************************
 * Sizes and title of the game window
 ************************************/
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Chess Game by lmtuan";

/**
 * Initialize window and renderer
 * @param window, renderer: window and renderer to be initialized
 * @return true if init successfully
 */
bool initGraphic(SDL_Window* &window, SDL_Renderer* &renderer);

/**
 * Deallocate window and renderer before quitting game
 * @param current window and renderer to be destroyed
 */
void quitGraphic(SDL_Window* window, SDL_Renderer* rederer);

/**
 * Play a chess game, and return the winner
 * @return 0 for white, 1 for black, 2 for draw, and -1 for premature quit (back to start screen or quit game)
 */
int playGame(Board* b, Player** players, BoardGUI* bgui, SDL_Renderer* renderer);



int main(int argc, char* argv[]) {
  SDL_Window* window;
  SDL_Renderer* renderer;
  if( !initGraphic(window, renderer) ) return 0; //quit if cannot initialize graphic


  // Init GUIs and board
  Board b;

  GUI::quit = false;
  StartGUI sgui(renderer);
  ChooseComGUI cgui(renderer);
  BoardGUI bgui(&b, renderer);
  EndGUI egui(renderer);

  Player** players = new Player*[2];


  int input;
  while (true) {
    b.initBoard(); //new board every game
    sgui.draw(renderer);
    do {
      input = sgui.getInput();
    } while (input == 0 && !GUI::quit);
    if (GUI::quit) break;

    if(input == StartGUI::INPUT_QUIT) {
      break;
    } else if (input == StartGUI::INPUT_MULTI_PLAYER) { // Human vs Human
      players[0] = new HumanPlayer(&bgui, &b);
      players[1] = new HumanPlayer(&bgui, &b);
      bgui.setPlayer(Board::BOTH_COLOR);
      egui.setPlayer(Board::BOTH_COLOR);
    } else {
      cgui.openingAnimation(renderer);
      cgui.draw(renderer);
      // get which player

      int comPlayer, difficulty;
      do {
        input = cgui.getInput();
      } while (input == 0 && !GUI::quit);
      if (GUI::quit) break;

      switch (input) {
        case ChooseComGUI::INPUT_WHITE_EASY  : comPlayer = Board::WHITE; difficulty = 2; break;
        case ChooseComGUI::INPUT_WHITE_MEDIUM: comPlayer = Board::WHITE; difficulty = 4; break;
        case ChooseComGUI::INPUT_WHITE_HARD  : comPlayer = Board::WHITE; difficulty = 6; break;
        case ChooseComGUI::INPUT_BLACK_EASY  : comPlayer = Board::BLACK; difficulty = 2; break;
        case ChooseComGUI::INPUT_BLACK_MEDIUM: comPlayer = Board::BLACK; difficulty = 4; break;
        case ChooseComGUI::INPUT_BLACK_HARD  : comPlayer = Board::BLACK; difficulty = 6; break;
      }
      players[comPlayer] = new AIPlayer(&b, &bgui, difficulty);
      players[1-comPlayer] = new HumanPlayer(&bgui, &b);
      bgui.setPlayer(1-comPlayer);
      egui.setPlayer(1-comPlayer);
      cgui.endingAnimation(renderer);
    }

    /*
     * Play the game
     */
    int winner = playGame(&b, players, &bgui, renderer);
    SDL_Delay(5000);
    delete players[0]; players[0] = NULL;
    delete players[1]; players[1] = NULL;
    /*
     * Congrat screen
     */
    if (winner == -1) {
      // press red x button
      if (GUI::quit) break;
      // press home button
      continue;
    } else {
      egui.setWinner(winner);
    }
    egui.draw(renderer);
    do {
      input = egui.getInput();
    } while (input == 0);
  }

  delete[] players;

  quitGraphic(window, renderer);
  return 0;
}

int playGame(Board* b, Player** players, BoardGUI* bgui, SDL_Renderer* renderer) {
  int numUndo[2] = {-1, -1};
  int curPlayer, input;
  bgui->draw(renderer);
  while (b->getNumMoves() != 0) { // continue as long as the game haven't ended
    curPlayer = b->getPlayer();
    input = players[ curPlayer ]->decideMove();

    // make the move
    if (players[curPlayer]->isHuman()) {
    // if player is human, the value returned is a chosen square. Choose the appropriate square.
      if (input == 0) { // user haven't selected move
      } else if (GUI::quit) { //user quit
        return -1;
      } else if (input >= BoardGUI::INPUT_MIN_SQUARE && input <= BoardGUI::INPUT_MAX_SQUARE) { //olayer chose a square in board
        // if there is a promotion event, cannot make move until player choose a promotion
        if (b->hasPromotion()) {
          //flash the promoted pawn
        } else {
          b->chooseSquare(input - 1);
        }
      } else {//player chose a button from side bar
        int promoteType;
        switch (input) {
          case BoardGUI::INPUT_UNDO: // undo
            if (numUndo[curPlayer] != 0) {
              b->undoMove();
              b->undoMove();
              numUndo[curPlayer]--;
            }
            break;
          case BoardGUI::INPUT_HOME: return -1; // return home
          case BoardGUI::INPUT_PROMOTE_QUEEN:  promoteType = Board::MOVE_PROMOTION_QUEEN; break;
          case BoardGUI::INPUT_PROMOTE_ROOK:   promoteType = Board::MOVE_PROMOTION_ROOK; break;
          case BoardGUI::INPUT_PROMOTE_BISHOP: promoteType = Board::MOVE_PROMOTION_BISHOP; break;
          case BoardGUI::INPUT_PROMOTE_KNIGHT: promoteType = Board::MOVE_PROMOTION_KNIGHT; break;
        }
        if (b->hasPromotion()) {
          b->promote(promoteType);
        }
      }
      bgui->updateMovePointers();
    } else {
    //if is AI, the value returned is a move number. Make the move, and process input queue to catch quitting event.
      if (input == -1) return -1; // AI returns -1 when user quit during AI thinking
      b->makeMove(input);
    }

    // draw board
    bgui->draw(renderer);
  }
  return (b->getWinner());
}

bool initGraphic(SDL_Window* &window, SDL_Renderer* &renderer) {
  // Init SDL
  if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    printf("Failed to initialize SDL. SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Create window
  window = SDL_CreateWindow(WINDOW_TITLE,
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH,
                            SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Failed to create window. SDL error: %s\n", SDL_GetError());
    return false;
  }

  // Create renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == NULL) {
    printf("Failed to create renderer. SDL error: %s\n", SDL_GetError());
    return false;
  }
  // Init rederer's color to white
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Init SDL_Image for Image loading
  if( IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0 ) {
    printf("Failed to initialize SDL_image. SDL_image error: %s\n", IMG_GetError());
    return false;
  }
  return true;
}

void quitGraphic(SDL_Window* window, SDL_Renderer* renderer) {
  // destroy window and renderer
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  // quit SDL
  IMG_Quit();
  SDL_Quit();
}
