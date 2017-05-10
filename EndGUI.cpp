#include "EndGUI.h"

const int EndGUI::textX = 400;
const int EndGUI::textY = 105;
const int EndGUI::buttonX = 400;
const int EndGUI::buttonY = 400;

EndGUI::EndGUI(SDL_Renderer* renderer)
{
  youWinTxt.loadFromFile("img/endGUI/single-player.png")
}
