#ifndef MAPPERPSPAD_H_
#define MAPPERPSPAD_H_

#include "mappers/PsProfiles.h"
#include <memory>
#include <sensor_msgs/Joy.h>

namespace phros_remote
{

class MapperPsPad
{
public:
  MapperPsPad() {}
  virtual ~MapperPsPad() {}
  virtual void map(const sensor_msgs::Joy& joy) = 0;
  bool         buttonPressedX(void) const { return _buttonPressedX; }
  bool         buttonPressedC(void) const { return _buttonPressedC; }
  bool         buttonPressedT(void) const { return _buttonPressedT; }
  bool         buttonPressedS(void) const { return _buttonPressedS; }
  bool         buttonPressedL1(void) const { return _buttonPressedL1; }
  bool         buttonPressedR1(void) const { return _buttonPressedR1; }
  bool         buttonPressedL2(void) const { return _buttonPressedL2; }
  bool         buttonPressedR2(void) const { return _buttonPressedR2; }
  bool         buttonPressedSL(void) const { return _buttonPressedSL; }
  bool         buttonPressedST(void) const { return _buttonPressedST; }
  bool         buttonPressedPS(void) const { return _buttonPressedPS; }
  bool         buttonPressedA1(void) const { return _buttonPressedA1; }
  bool         buttonPressedA2(void) const { return _buttonPressedA2; }
  bool         crossLeft(void) const { return _crossLeft; }
  bool         crossRight(void) const { return _crossRight; }
  bool         crossUp(void) const { return _crossUp; }
  bool         crossDown(void) const { return _crossDown; }
  const float  stickLeftX(void) const { return _stickLeftX; }
  const float  stickLeftY(void) const { return _stickLeftY; }
  const float  stickRightX(void) const { return _stickRightX; }
  const float  stickRightY(void) const { return _stickRightY; }

protected:
  bool  _buttonPressedX  = false;
  bool  _buttonPressedC  = false;
  bool  _buttonPressedT  = false;
  bool  _buttonPressedS  = false;
  bool  _buttonPressedL1 = false;
  bool  _buttonPressedR1 = false;
  bool  _buttonPressedL2 = false;
  bool  _buttonPressedR2 = false;
  bool  _buttonPressedSL = false;
  bool  _buttonPressedST = false;
  bool  _buttonPressedPS = false;
  bool  _buttonPressedA1 = false;
  bool  _buttonPressedA2 = false;
  bool  _crossLeft       = false;
  bool  _crossRight      = false;
  bool  _crossUp         = false;
  bool  _crossDown       = false;
  float _stickLeftX      = 0.0f;
  float _stickLeftY      = 0.0f;
  float _stickRightX     = 0.0f;
  float _stickRightY     = 0.0f;
};

} // namespace phros_remotes
#endif