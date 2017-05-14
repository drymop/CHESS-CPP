#include "GUI.h"
#include <stdio.h>

GUI::GUI() {
  //empty constructor
}

GUI::~GUI() {
  //Deallocate memory of box array
  delete[] boxes;
  boxes = NULL;
}

bool GUI::quit = false;

GUI::Box::Box(int x1, int y1, int x2, int y2, int value) {
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  this->boxValue = value;
}

bool GUI::Box::contains(int x, int y) {
  return ((x > x1) && (x < x2) && (y > y1) && (y < y2));
}

void GUI::setBoxes(Box* box, int size) {
  this->boxes = box;
  this->numBox = size;
}

int GUI::getInput() {
  //Get mouse coordinate
  int boxClicked = 0;
  SDL_Event e;
  while(SDL_PollEvent(&e) != 0) { //iterate through all events in queue
    if(e.type == SDL_QUIT) { //if user quit, change quit flag and return immediately
      GUI::quit = true;
      return 0;
    } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) { // if user left-clicks
      int mouseX = e.button.x;
      int mouseY = e.button.y;

      //Get clicked box's value
      for(int i = 0; i < numBox; i++) {
        if(boxes[i].contains(mouseX, mouseY)) {
          boxClicked = boxes[i].boxValue;
          break;
        }
      }
    }
  }
  return boxClicked;
}
