#include "StartGUI.h"

StartGUI::StartGUI()
{
  Box* boxArr = new Box[1];
  boxArr[0] = {0,0,350,350, 1};

  setBoxes(boxArr, 1);
}
