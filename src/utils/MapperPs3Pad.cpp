#include "MapperPs3Pad.h"

namespace phros_remote
{

void MapperPs3Pad::map(const sensor_msgs::Joy &joy)
{

  _buttonPressedX = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_X)];
  _buttonPressedC = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_C)];
  _buttonPressedT = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_T)];
  _buttonPressedS = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_S)];

  _buttonPressedL1 = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_L1)];
  _buttonPressedR1 = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_R1)];
  _buttonPressedL2 = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_L2)];
  _buttonPressedR2 = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_R2)];

  _buttonPressedSL = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_SELECT)];
  _buttonPressedST = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_START)];
  _buttonPressedPS = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_PS)];
  _buttonPressedA1 = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_A1)];
  _buttonPressedA2 = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_A2)];

  _crossLeft  = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_LEFT)];
  _crossRight = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_RIGHT)];
  _crossUp    = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_UP)];
  _crossDown  = joy.buttons[static_cast<unsigned int>(ButtonsPs3::B_DOWN)];

  _stickLeftX  = joy.axes[static_cast<unsigned int>(AxesPs3::A1_X)];
  _stickLeftY  = joy.axes[static_cast<unsigned int>(AxesPs3::A1_Y)];
  _stickRightX = joy.axes[static_cast<unsigned int>(AxesPs3::A2_X)];
  _stickRightY = joy.axes[static_cast<unsigned int>(AxesPs3::A2_Y)];
  _L2          = joy.axes[static_cast<unsigned int>(AxesPs3::L2)];
  _R2          = joy.axes[static_cast<unsigned int>(AxesPs3::R2)];
}

}