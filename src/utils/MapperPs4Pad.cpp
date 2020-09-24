#include "MapperPs4Pad.h"

namespace phros_remote {

void MapperPs4Pad::map(const sensor_msgs::Joy &joy) {
  _buttonPressedX = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_X)];
  _buttonPressedC = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_C)];
  _buttonPressedT = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_T)];
  _buttonPressedS = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_S)];

  _buttonPressedL1 = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_L1)];
  _buttonPressedR1 = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_R1)];
  _buttonPressedL2 = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_L2)];
  _buttonPressedR2 = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_R2)];

  _buttonPressedSL = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_SH)];
  _buttonPressedST = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_OPT)];
  _buttonPressedPS = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_PS)];
  _buttonPressedA1 = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_A1)];
  _buttonPressedA2 = joy.buttons[static_cast<unsigned int>(ButtonsPs4::B_A2)];

  _crossLeft = joy.axes[static_cast<unsigned int>(AxesPs4::X_X)] > AXE_MAX;
  _crossRight =
      joy.axes[static_cast<unsigned int>(AxesPs4::X_X)] < -1.0f * AXE_MAX;
  _crossUp = joy.axes[static_cast<unsigned int>(AxesPs4::X_Y)] > AXE_MAX;
  _crossDown =
      joy.axes[static_cast<unsigned int>(AxesPs4::X_Y)] < -1.0f * AXE_MAX;

  _stickLeftX = joy.axes[static_cast<unsigned int>(AxesPs4::A1_X)];
  _stickLeftY = joy.axes[static_cast<unsigned int>(AxesPs4::A1_Y)];
  _stickRightX = joy.axes[static_cast<unsigned int>(AxesPs4::A2_X)];
  _stickRightY = joy.axes[static_cast<unsigned int>(AxesPs4::A2_Y)];
  _L2 = joy.axes[static_cast<unsigned int>(AxesPs4::L2)];
  _R2 = joy.axes[static_cast<unsigned int>(AxesPs4::R2)];
}

} // namespace phros_remote