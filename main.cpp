/**
 * Chess Game
 * @author lmtuan
 * @version 0.9
 */

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include <BoardGUI.h>
#include <Board.h>
#include <HumanPlayer.h>
#include <RandomPlayer.h>



/**
 * Sizes and title of the game window
 */
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;
const char* WINDOW_TITLE = "Chess Game by lmtuan";


/**
 * Initialize window and screen surface
 * @param window and screen to be initialized
 * @return true when init success
 */
bool initGraphic(SDL_Window* &window, SDL_Renderer* &rederer);

/**
 * Deallocate graphic memory when quitting game
 * @param current window and renderer to be destroyed
 */
void quitGraphic(SDL_Window* window, SDL_Renderer* rederer);


int main(int argc, char* argv[])
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  /**
   * Quit flag
   */
  bool quit = false;

  if( !initGraphic(window, renderer) ) return 0; //quit if cannot initialize graphic

  /**
   * Init GUIs
   */

  Board b;
  BoardGUI bgui(&b);

  bgui.initGUI(renderer);
  bgui.draw(renderer);

  Player** players = new Player*[2];
  players[0] = new RandomPlayer(&b);
  players[1] = new RandomPlayer(&b);

  int move = 0;
  while (!quit)
  {
    players[b.getPlayer()]->decideMove();
    bgui.draw(renderer);
    move = 0;
    while (move == 0)
    {
      move = bgui.getMove();
    }
    if (move == -1) break;
  }

  quitGraphic(window, renderer);
  return 0;
}


bool initGraphic(SDL_Window* &window, SDL_Renderer* &renderer)
{
  // Init SDL
  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
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
  if (window == NULL)
  {
    printf("Failed to create window. SDL error: %s\n", SDL_GetError());
    return false;
  }

  // Create renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == NULL)
  {
    printf("Failed to create renderer. SDL error: %s\n", SDL_GetError());
    return false;
  }
  // Init rederer's color to white
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Init SDL_Image for Image loading
  if( IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0 )
  {
    printf("Failed to initialize SDL_image. SDL_image error: %s\n", IMG_GetError());
    return false;
  }
  return true;
}

void quitGraphic(SDL_Window* window, SDL_Renderer* renderer)
{
  // destroy window and renderer
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;

  // quit SDL
  IMG_Quit();
  SDL_Quit();
}
