#include "GUI.h"
#include <stdio.h>

GUI::GUI()
{
  //empty constructor
}

GUI::~GUI()
{
  /**
   * Deallocate memory of box array
   */
  delete[] boxes;
  boxes = NULL;
}

GUI::Box::Box(int x1, int y1, int x2, int y2, int value)
{
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  this->boxValue = value;
  //printf("Creating: ");
  //printBox();
}

void GUI::Box::printBox()
{
  printf("box %i %i %i %i with value %i.\n", this->x1, this->y1, this->x2, this->y2, this->boxValue);
}

bool GUI::Box::contains(int x, int y)
{
  bool containP = ((x > x1) && (x < x2) && (y > y1) && (y < y2));
  //printf("Checking box %i %i %i %i with value ", x1, y1, x2, y2);
  //printf(containP? "True.\n": "False.\n");
  return containP;
}

void GUI::setBoxes(Box box[], int size)
{
  this->boxes = box;
  this->numBox = size;
  /*for(int i = 0; i < this->numBox; i++) {
    printf("Setting: ");
    this->boxes[i].printBox();
  }*/
}

void GUI::printAllBox()
{
  //Print out all the box
  printf("All boxes are\n");
  for(int i = 0; i < this->numBox; i++) {
    this->boxes[i].printBox();
  }
}
int GUI::getMove()
{
  //printAllBox();
  //Get mouse coordinate
  SDL_Event e;
  while(SDL_PollEvent(&e) != 0)
  {
    if(e.type == SDL_QUIT)
    {
      return -1;
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
    {
      int mouseX = e.button.x;
      int mouseY = e.button.y;
      //printf("mouse click at %i %i\n", mouseX, mouseY);

      //Get box value
      for(int i = 0; i < numBox; i++)
      {
        if(boxes[i].contains(mouseX, mouseY))
        {
          return boxes[i].boxValue;
        }
      }
    }
  }
  return 0;
}
