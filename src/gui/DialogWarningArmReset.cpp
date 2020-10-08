#include "DialogWarningArmReset.h"

namespace phros_remote
{
  DialogWarningArmReset::DialogWarningArmReset():
  _guiUi(std::make_unique<Ui::DialogWarningArmReset>())
  {
    _guiUi->setupUi(this);
  }

}